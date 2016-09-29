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

IContainerProcessor::Type ProControllerDispatcher::type() const { return IContainerProcessor::Type::ProController; }

void ProControllerDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {

	auto process_key = [this, id, &container]() {
		if (container->deviceType() != Device::ProController)
			return;

		emit procontrollerButtonDataChanged(id, static_cast<ButtonContainer *>(container.get())->state());
	};

	switch (container->type()) {
		case IContainer::Type::Button: return process_key();
	}
}

QList<uint> ProControllerDispatcher::procontrollerList() {
	return {};
}
