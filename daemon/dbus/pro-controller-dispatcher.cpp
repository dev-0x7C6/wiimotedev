#include "pro-controller-dispatcher.h"
#include "procontrolleradaptor.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

ProControllerDispatcher::ProControllerDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new ProcontrollerAdaptor(this);
}

Adaptor ProControllerDispatcher::type() const { return Adaptor::ProController; }

void ProControllerDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (!is::pro_controller(ev.first))
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
