#include "pro-controller-dispatcher.h"

using namespace service::dbus;
using namespace service::interface;

ProControllerDispatcher::ProControllerDispatcher(QObject *parent)
		: IContainerProcessor(parent) {}

IContainerProcessor::Type ProControllerDispatcher::type() const { return IContainerProcessor::Type::ProController; }

void ProControllerDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
}
