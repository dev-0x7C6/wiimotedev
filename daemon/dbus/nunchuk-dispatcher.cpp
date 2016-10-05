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

NunchukDispatcher::NunchukDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new NunchukAdaptor(this);
}

Device NunchukDispatcher::device() const { return Device::Nunchuk; }
QList<uint> NunchukDispatcher::nunchukList() const { return m_ids.toList(); }

void NunchukDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_acc = [this, id, &container]() -> void {
		const auto &data = static_cast<const AccelerometerContainer *>(container.get())->data();
		emit nunchukAccelerometerDataChanged(id, data.x, data.y, data.z, data.pitch, data.roll);
	};

	auto process_key = [this, id, &container]() {
		emit nunchukButtonDataChanged(id, static_cast<ButtonContainer *>(container.get())->state());
	};

	auto process_status = [this, id, &container]() {
		const auto state = static_cast<const StatusContainer *>(container.get())->state();

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit nunchukConnected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.remove(id);
			emit nunchukDisconnected(id);
		}
	};

	auto process_stick = [this, id, &container]() {
		const auto lx = static_cast<const StickContainer *>(container.get())->lx();
		const auto ly = static_cast<const StickContainer *>(container.get())->ly();
		emit nunchukStickDataChanged(id, lx, ly);
	};

	switch (container->event()) {
		case Event::Accelerometer: return process_acc();
		case Event::Button: return process_key();
		case Event::Status: return process_status();
		case Event::Stick: return process_stick();
	}
}
