#include "classic-dispatcher.h"

#include "classicadaptor.h"

using namespace service::dbus;
using namespace service::enums;
using namespace service::interface;

ClassicDispatcher::ClassicDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new ClassicAdaptor(this);
}

Device ClassicDispatcher::device() const { return Device::Classic; }
QList<uint> ClassicDispatcher::classicList() const { return m_ids.toList(); }

void ClassicDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {}
