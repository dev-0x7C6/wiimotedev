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
QList<uint> NunchukDispatcher::list() const { return m_ids.values(); }

void NunchukDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (Device::Nunchuk != ev.first)
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](dae::container::stick v) {
					   emit stickDataChanged(id, v.x, v.y);
				   },
				   [&](dae::container::accdata v) {
					   emit accelerometerDataChanged(id, v.x, v.y, v.z, v.pitch, v.roll);
				   },
				   [&](dae::container::button v) {
					   emit buttonDataChanged(id, v.states);
				   },
				   [&](dae::container::status v) {
					   if (v.is_connected)
						   m_ids.insert(id);
					   else
						   m_ids.remove(id);
					   emit connectionChanged(id, v.is_connected);
				   }},

		ev.second);
}
