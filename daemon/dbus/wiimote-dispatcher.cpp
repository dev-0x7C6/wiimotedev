#include "wiimote-dispatcher.h"

#include <QDBusConnection>

#include "containers/accelerometer-container.h"
#include "containers/gyroscope-container.h"
#include "containers/infrared-container.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "wiimoteadaptor.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::enums;
using namespace dae::dbus;
using namespace dae::interface;

WiimoteDispatcher::WiimoteDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {

	new WiimoteAdaptor(this);
}

Adaptor WiimoteDispatcher::type() const { return Adaptor::Wiimote; }
QList<uint> WiimoteDispatcher::list() { return m_ids.toList(); }

bool WiimoteDispatcher::ledStatus(uint id, uint led_id) { return std::get<bool>(generateEvent({CommandType::GetLedState, id, GetLedStateEvent{led_id}}).value_or(false)); }
bool WiimoteDispatcher::rumbleStatus(uint id) { return std::get<bool>(generateEvent({CommandType::GetRumbleState, id, {}}).value_or(false)); }
bool WiimoteDispatcher::setLedStatus(uint id, uint led_id, bool status) { return std::get<bool>(generateEvent({CommandType::SetLedState, id, SetLedStateEvent{led_id, status}}).value_or(false)); }
bool WiimoteDispatcher::setRumbleStatus(uint id, bool status) { return std::get<bool>(generateEvent({CommandType::SetRumbleState, id, SetRumbleStateEvent{status}}).value_or(false)); }

void WiimoteDispatcher::process(const Device device, const u32 id, const std::unique_ptr<IContainer> &container) {
	if (Device::Wiimote != device)
		return;

	auto process_ir = [this, id, &container]() -> void {
		const auto &ir = static_cast<InfraredContainer *>(container.get())->points();
		emit infraredDataChanged(id, ir[0].x, ir[0].y, ir[1].x, ir[1].y, ir[2].x, ir[2].y, ir[3].x, ir[3].y);
	};

	auto process_acc = [this, id, &container]() -> void {
		const auto &data = static_cast<AccelerometerContainer *>(container.get())->data();
		emit accelerometerDataChanged(id, data.x, data.y, data.z, data.pitch, data.roll);
	};

	auto process_gyro = [this, id, &container]() -> void {
		const auto *gyro = static_cast<GyroscopeContainer *>(container.get());
		emit gyroscopeDataChanged(id, gyro->x(), gyro->y(), gyro->z());
	};

	auto process_button = [this, id, &container]() -> void {
		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
		emit buttonDataChanged(id, state);
	};

	auto process_status = [this, id, &container]() -> void {
		const auto state = static_cast<StatusContainer *>(container.get())->state();

		if (container->device() != Device::Wiimote)
			return;

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit connected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.remove(id);
			emit disconnected(id);
		}
	};

	switch (container->event()) {
		case Event::Accelerometer: return process_acc();
		case Event::Button: return process_button();
		case Event::Gyroscope: return process_gyro();
		case Event::Infrared: return process_ir();
		case Event::Status: return process_status();
		default:
			break;
	}
}
