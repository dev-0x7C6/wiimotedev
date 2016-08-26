#include "iinput-device.h"

IInputDevice::IInputDevice(const std::string &name, uint32_t id)
		: m_name(name)
		, m_id(id) {
}

uint32_t IInputDevice::id() const { return m_id; }
std::string IInputDevice::name() const { return m_name; }
