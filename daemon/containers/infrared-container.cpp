#include "infrared-container.h"
#include <xwiimote.h>

using namespace common::enums;
using namespace dae::container;
using namespace dae::enums;
using namespace dae::interface;

InfraredContainer::InfraredContainer(const IrPoints &points)
		: m_points(points) {}

Event InfraredContainer::event() const { return Event::Infrared; }
Device InfraredContainer::device() const { return Device::Wiimote; }
const IrPoints &InfraredContainer::points() const { return m_points; }
