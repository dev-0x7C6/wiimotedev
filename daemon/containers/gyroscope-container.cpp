#include "gyroscope-container.h"

using namespace service::container;
using namespace service::container::structs;
using namespace service::interface;
using namespace service::enums;

GyroscopeContainer::GyroscopeContainer(const gyrodata &data)
		: m_data(data) {}

Event GyroscopeContainer::event() const { return Event::Gyroscope; }

gyrodata GyroscopeContainer::data() const { return m_data; }

Device GyroscopeContainer::device() const { return Device::Wiimote; }
