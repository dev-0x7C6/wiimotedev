#include "gyroscope-container.h"

using namespace daemon::container;
using namespace daemon::interface;

GyroscopeContainer::GyroscopeContainer(const gyrodata &data)
		: m_data(data) {}

IContainer::Type GyroscopeContainer::type() const { return Type::Gyroscope; }

IContainer::Source GyroscopeContainer::source() const { return Source::Wiimote; }
