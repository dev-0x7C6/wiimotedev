#include "classic-dispatcher.h"

#include "classicadaptor.h"

using namespace service::dbus;
using namespace service::enums;
using namespace service::interface;

ClassicDispatcher::ClassicDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new ClassicAdaptor(this);
}

QList<uint> ClassicDispatcher::classicList() const {
	return {};
}

Device ClassicDispatcher::device() const { return Device::Classic; }

void ClassicDispatcher::process(const uint32_t id, const std::unique_ptr<service::interface::IContainer> &container) {
}
