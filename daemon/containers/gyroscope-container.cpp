#include "gyroscope-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::interface;
using namespace dae::enums;

GyroscopeContainer::GyroscopeContainer(const int32_t x, const int32_t y, const int32_t z)
		: m_x(x)
		, m_y(y)
		, m_z(z) {
}

Event GyroscopeContainer::event() const { return Event::Gyroscope; }
Device GyroscopeContainer::device() const { return Device::Wiimote; }
