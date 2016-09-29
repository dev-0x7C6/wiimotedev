#include "infrared-container.h"
#include <xwiimote.h>

using namespace service::container;
using namespace service::enums;
using namespace service::interface;

InfraredContainer::InfraredContainer(const IrPoints &points)
		: m_points(points) {}

IContainer::Type InfraredContainer::type() const { return Type::Infrared; }
Device InfraredContainer::deviceType() const { return Device::Wiimote; }
const IrPoints &InfraredContainer::points() const { return m_points; }
