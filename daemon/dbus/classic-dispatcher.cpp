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

void ClassicDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (!is::classic_controller(ev.first))
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
					   ids.set(id, v.is_connected);
					   emit connectionChanged(id, v.is_connected);
				   }},
		ev.second);
}
