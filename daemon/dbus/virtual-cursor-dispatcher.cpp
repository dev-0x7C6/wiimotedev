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
				   [&](const dae::container::ir_points v) {
					   if (!m_processors.count(id)) {
						   m_processors.emplace(id, std::make_unique<VirtualCursorProcessor>());
					   }

					   QList<QPair<int, int>> points;
					   for (const auto &point : v)
						   if (point.x != 1023 && point.y != 1023)
							   points.append({point.x, point.y});

					   const auto vc = m_processors[id]->calculate(points);
					   const auto wasVisible = visibility.contains(id);
					   const auto isVisible = vc.visible;

					   if (wasVisible && !isVisible)
						   emit visibilityChanged(id, false);

					   if (!wasVisible && isVisible)
						   emit visibilityChanged(id, true);

					   emit dataChanged(id, vc.x, vc.y, vc.yaw, vc.roll, vc.pitch, vc.distance, vc.visible);
				   }},
		ev.second);
}
