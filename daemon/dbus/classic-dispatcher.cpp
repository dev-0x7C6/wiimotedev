#include "classic-dispatcher.h"

#include "classicadaptor.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

ClassicDispatcher::ClassicDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new ClassicAdaptor(this);
}

Adaptor ClassicDispatcher::type() const { return Adaptor::Classic; }
QList<uint> ClassicDispatcher::list() const { return m_ids.values(); }

void ClassicDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (Device::Classic != ev.first)
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](const dae::container::button v) {
					   emit buttonDataChanged(id, v.states);
				   },
				   [&](const dae::container::stick_pair v) {
					   emit stickDataChanged(id, v.first.x, v.second.y, v.second.x, v.second.y);
				   },

				   [&](const dae::container::status v) {
					   if (v.is_connected)
						   m_ids.insert(id);
					   else
						   m_ids.remove(id);
					   emit connectionChanged(id, v.is_connected);
				   }},
		ev.second);
}
