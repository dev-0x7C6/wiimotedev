#include "infrared-container.h"
#include <xwiimote.h>

using namespace daemon::container;

InfraredContainer::InfraredContainer(const IrPoints &points)
		: m_points(points) {}

InfraredContainer::InfraredContainer(const xwii_event &event) {
	for (std::size_t i = 0; i < m_points.size(); ++i) {
		m_points[i].x = event.v.abs[i].x;
		m_points[i].y = event.v.abs[i].y;
		m_points[i].size = event.v.abs[i].z;
		m_points[i].valid = xwii_event_ir_is_valid(&event.v.abs[i]);
	}
}

daemon::interface::IContainer::Type InfraredContainer::type() const { return Type::Infrared; }

daemon::interface::IContainer::Source InfraredContainer::source() const { return Source::Wiimote; }

const IrPoints &InfraredContainer::points() const { return m_points; }
