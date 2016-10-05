#include "gyroscope-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::container::structs;
using namespace dae::interface;
using namespace dae::enums;

GyroscopeContainer::GyroscopeContainer(const gyrodata &data)
		: m_data(data) {}

Event GyroscopeContainer::event() const { return Event::Gyroscope; }

gyrodata GyroscopeContainer::data() const { return m_data; }

Device GyroscopeContainer::device() const { return Device::Wiimote; }
