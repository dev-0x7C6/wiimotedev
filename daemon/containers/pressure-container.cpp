#include "pressure-container.h"

using namespace service::interface;
using namespace service::container;
using namespace service::enums;

PressureContainer::PressureContainer(const pressdata &data)
		: m_data(data) {
}

Device PressureContainer::deviceType() const { return enums::Device::BalanceBoard; }
IContainer::Type PressureContainer::type() const { return Type::Pressure; }
const pressdata &PressureContainer::data() const { return m_data; }
