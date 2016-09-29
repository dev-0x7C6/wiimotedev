#include "infrared-container.h"
#include <xwiimote.h>

using namespace service::container;
using namespace service::enums;
using namespace service::interface;

InfraredContainer::InfraredContainer(const IrPoints &points)
		: m_points(points) {}

Event InfraredContainer::event() const { return Event::Infrared; }
Device InfraredContainer::device() const { return Device::Wiimote; }
const IrPoints &InfraredContainer::points() const { return m_points; }
