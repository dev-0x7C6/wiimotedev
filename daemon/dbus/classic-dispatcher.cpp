#include "classic-dispatcher.h"

#include "classicadaptor.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "containers/stick-container.h"

using namespace service::container;
using namespace service::dbus;
using namespace service::enums;
using namespace service::interface;

ClassicDispatcher::ClassicDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new ClassicAdaptor(this);
}

Device ClassicDispatcher::device() const { return Device::Classic; }
QList<uint> ClassicDispatcher::classicList() const { return m_ids.toList(); }

void ClassicDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_key = [this, id, &container]() {
		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
		emit classicButtonDataChanged(id, state);
	};

	auto process_stick = [this, id, &container]() {
		const auto lx = static_cast<const StickContainer *>(container.get())->lx();
		const auto ly = static_cast<const StickContainer *>(container.get())->ly();
		const auto rx = static_cast<const StickContainer *>(container.get())->rx();
		const auto ry = static_cast<const StickContainer *>(container.get())->ry();
		emit classicStickDataChanged(id, lx, ly, rx, ry);
	};

	auto process_status = [this, id, &container]() {
		const auto state = static_cast<const StatusContainer *>(container.get())->state();

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit classicConnected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.remove(id);
			emit classicDisconnected(id);
		}
	};

	switch (container->event()) {
		case Event::Button: return process_key();
		case Event::Stick: return process_stick();
		case Event::Status: return process_status();
	}
}
