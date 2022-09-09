#include "virtual-cursor-dispatcher.h"
#include "virtualcursoradaptor.h"

#include "functionals/virtual-cursor-processor.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::functional;

VirtualCursorDispatcher::VirtualCursorDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new VirtualcursorAdaptor(this);
}

VirtualCursorDispatcher::~VirtualCursorDispatcher() = default;

Adaptor VirtualCursorDispatcher::type() const { return Adaptor::VirtualCursor; }

void VirtualCursorDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (!is::wiimote(ev.first))
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](const dae::container::status v) {
					   if (!v.is_connected)
						   m_processors.erase(id);
				   },
				   [&](const dae::container::ir_points v) {
					   if (!m_processors.contains(id))
						   m_processors.emplace(id, std::make_unique<VirtualCursorProcessor>());

					   auto &&processor = m_processors[id];

					   const auto last = processor->previous();
					   const auto current = processor->calculate(v);

					   if (last.visible && !current.visible)
						   emit visibilityChanged(id, false);

					   if (!last.visible && current.visible)
						   emit visibilityChanged(id, true);

					   auto &&x = current.x;
					   auto &&y = current.y;
					   auto &&yaw = current.yaw;
					   auto &&roll = current.roll;
					   auto &&pitch = current.pitch;
					   auto &&distance = current.distance;
					   auto &&visible = current.visible;

					   emit dataChanged(id, x, y, yaw, roll, pitch, distance, visible);
				   }},
		ev.second);
}
