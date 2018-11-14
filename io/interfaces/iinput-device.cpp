#include "iinput-device.h"

using namespace io::interface;

IInputDevice::IInputDevice(std::string &&name, const u32 id)
		: m_name(std::move(name))
		, m_id(id) {
}
