#include "gyroscope-container.h"

using namespace service::container;
using namespace service::interface;
using namespace service::enums;

GyroscopeContainer::GyroscopeContainer(const gyrodata &data)
		: m_data(data) {}

IContainer::Type GyroscopeContainer::type() const { return Type::Gyroscope; }

gyrodata GyroscopeContainer::data() const { return m_data; }

Device GyroscopeContainer::deviceType() const { return Device::Wiimote; }
