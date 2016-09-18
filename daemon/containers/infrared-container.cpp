#include "infrared-container.h"
#include <xwiimote.h>

using namespace service::container;

InfraredContainer::InfraredContainer(const IrPoints &points)
		: m_points(points) {}

service::interface::IContainer::Type InfraredContainer::type() const { return Type::Infrared; }

service::interface::IContainer::Source InfraredContainer::source() const { return Source::Wiimote; }

const IrPoints &InfraredContainer::points() const { return m_points; }
