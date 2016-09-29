#include "pro-controller-dispatcher.h"
#include "procontrolleradaptor.h"

#include "containers/button-container.h"

using namespace service::container;
using namespace service::dbus;
using namespace service::enums;
using namespace service::interface;

ProControllerDispatcher::ProControllerDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new ProcontrollerAdaptor(this);
}

Device ProControllerDispatcher::device() const { return Device::ProController; }

void ProControllerDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_key = [this, id, &container]() {
		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
		emit procontrollerButtonDataChanged(id, state);
	};

	switch (container->event()) {
		case Event::Button: return process_key();
	}
}

QList<uint> ProControllerDispatcher::procontrollerList() {
	return {};
}
