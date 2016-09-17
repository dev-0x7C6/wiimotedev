#include "pressure-container.h"

using namespace daemon::interface;
using namespace daemon::container;

PressureContainer::PressureContainer(const pressdata &data)
		: m_data(data) {
}

IContainer::Source PressureContainer::source() const { return Source::BalanceBoard; }
IContainer::Type PressureContainer::type() const { return Type::Pressure; }
const pressdata &PressureContainer::data() const { return m_data; }
