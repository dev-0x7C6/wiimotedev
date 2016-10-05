#include "pressure-container.h"

using namespace common::enums;
using namespace dae::interface;
using namespace dae::container;
using namespace dae::container::structs;
using namespace dae::enums;

PressureContainer::PressureContainer(const pressdata &data)
		: m_data(data) {
}

Device PressureContainer::device() const { return common::enums::Device::BalanceBoard; }
Event PressureContainer::event() const { return Event::Pressure; }
const pressdata &PressureContainer::data() const { return m_data; }
