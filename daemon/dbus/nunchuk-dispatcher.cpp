#include "nunchuk-dispatcher.h"
#include "nunchukadaptor.h"

#include <spdlog/spdlog.h>

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

NunchukDispatcher::NunchukDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new NunchukAdaptor(this);
}

Adaptor NunchukDispatcher::type() const { return Adaptor::Nunchuk; }

void NunchukDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (!is::nunchuk(ev.first))
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](dae::container::stick v) {
					   emit stickDataChanged(id, v.x, v.y);
				   },
				   [&](dae::container::accdata v) {
					   emit accelerometerDataChanged(id, v.raw.x, v.raw.y, v.raw.z, v.angles.pitch(), v.angles.roll());
				   },
				   [&](dae::container::button v) {
					   emit buttonDataChanged(id, v.states);
				   },
				   [&](dae::container::status v) {
					   ids.set(id, v.is_connected);
					   emit connectionChanged(id, v.is_connected);
				   }},

		ev.second);
}
