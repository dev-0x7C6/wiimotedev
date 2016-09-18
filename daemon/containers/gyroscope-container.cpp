#include "gyroscope-container.h"

using namespace service::container;
using namespace service::interface;

GyroscopeContainer::GyroscopeContainer(const gyrodata &data)
		: m_data(data) {}

IContainer::Type GyroscopeContainer::type() const { return Type::Gyroscope; }

IContainer::Source GyroscopeContainer::source() const { return Source::Wiimote; }
