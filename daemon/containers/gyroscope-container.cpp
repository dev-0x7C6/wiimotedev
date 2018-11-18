#include "gyroscope-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::interface;
using namespace dae::enums;

GyroscopeContainer::GyroscopeContainer(const i32 x, const i32 y, const i32 z)
		: m_x(x)
		, m_y(y)
		, m_z(z) {
}

Event GyroscopeContainer::event() const { return Event::Gyroscope; }
Device GyroscopeContainer::device() const { return Device::Wiimote; }
