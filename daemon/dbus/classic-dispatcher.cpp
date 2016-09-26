#include "classic-dispatcher.h"

#include "classicadaptor.h"

using namespace service::dbus;
using namespace service::interface;

ClassicDispatcher::ClassicDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new ClassicAdaptor(this);
}

QList<uint> ClassicDispatcher::classicList() const {
	return {};
}

IContainerProcessor::Type ClassicDispatcher::type() const {
	return Type::Classic;
}

void ClassicDispatcher::process(const uint32_t id, const std::unique_ptr<service::interface::IContainer> &container) {
}
