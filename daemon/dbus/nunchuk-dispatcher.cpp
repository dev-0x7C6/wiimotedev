#include "nunchuk-dispatcher.h"
#include "nunchukadaptor.h"

#include "containers/accelerometer-container.h"
#include "containers/button-container.h"
#include "containers/status-container.h"

using namespace service::container;
using namespace service::dbus;
using namespace service::enums;
using namespace service::interface;

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

	switch (container->event()) {
		case Event::Accelerometer: return process_acc();
		case Event::Button: return process_key();
		case Event::Status: return process_status();
	}
}
