#include "pressure-container.h"

using namespace service::interface;
using namespace service::container;
using namespace service::enums;

PressureContainer::PressureContainer(const pressdata &data)
		: m_data(data) {
}

Device PressureContainer::device() const { return enums::Device::BalanceBoard; }
Event PressureContainer::event() const { return Event::Pressure; }
const pressdata &PressureContainer::data() const { return m_data; }
