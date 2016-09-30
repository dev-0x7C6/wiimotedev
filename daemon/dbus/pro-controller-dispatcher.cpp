#include "pro-controller-dispatcher.h"
#include "procontrolleradaptor.h"

#include "containers/button-container.h"
#include "containers/status-container.h"

using namespace service::container;
using namespace service::dbus;
using namespace service::enums;
using namespace service::interface;

ProControllerDispatcher::ProControllerDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new ProcontrollerAdaptor(this);
}

Device ProControllerDispatcher::device() const { return Device::ProController; }
QList<uint> ProControllerDispatcher::procontrollerList() { return m_ids.toList(); }

void ProControllerDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_key = [this, id, &container]() {
		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
		emit procontrollerButtonDataChanged(id, state);
	};

	auto process_status = [this, id, &container]() {
		const auto state = static_cast<const StatusContainer *>(container.get())->state();

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit procontrollerConnected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.insert(id);
			emit procontrollerDisconnected(id);
		}
	};

	switch (container->event()) {
		case Event::Status: return process_status();
		case Event::Button: return process_key();
	}
}
