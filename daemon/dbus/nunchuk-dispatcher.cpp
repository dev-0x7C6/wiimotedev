#include "nunchuk-dispatcher.h"
#include "nunchukadaptor.h"

#include "containers/accelerometer-container.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "containers/stick-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

NunchukDispatcher::NunchukDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new NunchukAdaptor(this);
}

Adaptor NunchukDispatcher::type() const { return Adaptor::Nunchuk; }
QList<uint> NunchukDispatcher::list() const { return m_ids.toList(); }

void NunchukDispatcher::process(const Device device, const u32 id, const std::unique_ptr<IContainer> &container) {
	if (Device::Nunchuk != device)
		return;

	auto process_acc = [this, id, &container]() -> void {
		const auto &data = static_cast<const AccelerometerContainer *>(container.get())->data();
		emit accelerometerDataChanged(id, data.x, data.y, data.z, data.pitch, data.roll);
	};

	auto process_key = [this, id, &container]() {
		emit buttonDataChanged(id, static_cast<ButtonContainer *>(container.get())->state());
	};

	auto process_status = [this, id, &container]() {
		const auto state = static_cast<const StatusContainer *>(container.get())->state();

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit connected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.remove(id);
			emit disconnected(id);
		}
	};

	auto process_stick = [this, id, &container]() {
		const auto lx = static_cast<const StickContainer *>(container.get())->lx();
		const auto ly = static_cast<const StickContainer *>(container.get())->ly();
		emit stickDataChanged(id, lx, ly);
	};

	const auto event = container->event();

	if (event == Event::Accelerometer)
		return process_acc();
	else if (event == Event::Button)
		return process_key();
	else if (event == Event::Status)
		return process_status();
	else if (event == Event::Stick)
		return process_stick();
}
