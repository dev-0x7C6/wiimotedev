#include "infrared-container.h"
#include <xwiimote.h>

using namespace daemon::container;

InfraredContainer::InfraredContainer(const IrPoints &points)
		: m_points(points) {}

daemon::interface::IContainer::Type InfraredContainer::type() const { return Type::Infrared; }

daemon::interface::IContainer::Source InfraredContainer::source() const { return Source::Wiimote; }

const IrPoints &InfraredContainer::points() const { return m_points; }
