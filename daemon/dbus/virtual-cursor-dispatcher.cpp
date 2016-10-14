#include "virtual-cursor-dispatcher.h"
#include "virtualcursoradaptor.h"

using namespace common::enums;
using namespace dae::dbus;
using namespace dae::enums;

VirtualCursorDispatcher::VirtualCursorDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new VirtualcursorAdaptor(this);
}

Adaptor VirtualCursorDispatcher::type() const { return Adaptor::VirtualCursor; }
QList<uint> VirtualCursorDispatcher::list() const { return m_ids.toList(); }

void VirtualCursorDispatcher::process(const Device device, const uint32_t id, const std::unique_ptr<dae::interface::IContainer> &container) {
	if (Device::Wiimote != device)
		return;
}
