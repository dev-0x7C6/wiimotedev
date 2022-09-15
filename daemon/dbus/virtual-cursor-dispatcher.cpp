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

	auto getVirtualCursor = [&](const u32 id) -> std::unique_ptr<VirtualCursorProcessor> & {
		if (!m_processors.contains(id))
			m_processors.emplace(id, std::make_unique<VirtualCursorProcessor>());
		return m_processors[id];
	};

	auto updateStatus = [&](const u32 id, const dae::container::ir_points &v) {
		auto &&vc = getVirtualCursor(id);

		const auto last = vc->previous();
		const auto current = vc->calculate(v);

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
	};

	std::visit(overloaded{
				   [&](auto) {},
				   [&](const dae::container::status &v) {
					   if (!v.is_connected) {
						   updateStatus(id, {});
						   m_processors.erase(id);
					   }
				   },
				   [&](const dae::container::reconfigure &) {
					   m_processors.erase(id);
				   },
				   [&](const dae::container::accdata &v) {
					   getVirtualCursor(id)->input(v);
				   },
				   [&](const dae::container::gyro &v) {
					   getVirtualCursor(id)->input(v);
				   },
				   [&](const dae::container::ir_points &v) {
					   updateStatus(id, v);
				   }},
		ev.second);
}
