#include "wiimote-dispatcher.h"

#include <QDBusConnection>

#include "containers/accelerometer-container.h"
#include "containers/gyroscope-container.h"
#include "containers/infrared-container.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "wiimoteadaptor.h"

using namespace service::container;
using namespace service::enums;
using namespace service::dbus;
using namespace service::interface;

WiimoteDispatcher::WiimoteDispatcher(QObject *parent)
		: IContainerProcessor(parent) {

	new WiimoteAdaptor(this);
}

Device WiimoteDispatcher::device() const { return Device::Wiimote; }

void WiimoteDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_ir = [this, id, &container]() -> void {
		const auto &ir = static_cast<InfraredContainer *>(container.get())->points();
		emit wiimoteInfraredDataChanged(id, ir[0].x, ir[0].y, ir[1].x, ir[1].y, ir[2].x, ir[2].y, ir[3].x, ir[3].y);
	};

	auto process_acc = [this, id, &container]() -> void {
		const auto &data = static_cast<AccelerometerContainer *>(container.get())->data();
		emit wiimoteAccelerometerDataChanged(id, data.x, data.y, data.z, data.pitch, data.roll);
	};

	auto process_gyro = [this, id, &container]() -> void {
		const auto &data = static_cast<GyroscopeContainer *>(container.get())->data();
		emit wiimoteGyroscopeDataChanged(id, data.x, data.y, data.z, data.lowX, data.lowY, data.lowZ);
	};

	auto process_button = [this, id, &container]() -> void {
		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
		emit wiimoteButtonDataChanged(id, state);
	};

	auto process_status = [this, id, &container]() -> void {
		const auto state = static_cast<StatusContainer *>(container.get())->state();

		if (container->device() != Device::Wiimote)
			return;

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit wiimoteConnected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.remove(id);
			emit wiimoteDisconnected(id);
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

QList<uint> WiimoteDispatcher::wiimoteList() const { return m_ids.toList(); }
