#include "igamepad.h"
#include <map>

using namespace common::enums;
using namespace io::interface;

IGamepad::IGamepad(const std::string &name, const uint32_t id, ButtonMap &&buttons, AxisMap &&axises)
		: InputDevice(name, id)
		, m_buttons(buttons)
		, m_axises(axises)

{
}

Device IGamepad::fromString(const std::string &type) {
	static std::map<std::string, Device> str2type{
		{"classic", Device::Classic},
		{"nunchuk", Device::Nunchuk},
		{"wiimote", Device::Wiimote}};

	try {
		return str2type.at(type);
	} catch (std::out_of_range &e) {
		throw e;
	}

	return Device::Classic;
}

bool IGamepad::configure() {
	bool isValid = true;
	isValid &= set_ev_bit(EV_KEY) == 0;
	isValid &= set_ev_bit(EV_ABS) == 0;

	for (const auto &value : m_buttons)
		isValid &= set_key_bit(value.output) == 0;

	for (const auto &value : m_axises) {
		for (const auto &v : value.pair) {
			isValid &= set_abs_bit(v.axis) == 0;
			set_range(v.axis, v.max, v.min);
		}
	}

	return isValid;
}

bool IGamepad::input(const uint64_t buttons) {
	bool isValid = true;
	int32_t dpad_x = 0;
	int32_t dpad_y = 0;
	for (const auto &value : m_buttons) {
		const bool toggled = (buttons & value.input) ? 1 : 0;
		isValid &= report(EV_KEY, value.output, toggled);
		dpad_y = (toggled && (BTN_DPAD_UP == value.output)) ? 1 : dpad_y;
		dpad_y = (toggled && (BTN_DPAD_DOWN == value.output)) ? -1 : dpad_y;
		dpad_x = (toggled && (BTN_DPAD_RIGHT == value.output)) ? 1 : dpad_x;
		dpad_x = (toggled && (BTN_DPAD_LEFT == value.output)) ? -1 : dpad_x;
	}
	isValid &= input(Stick::Dpad, dpad_x, dpad_y);
	return isValid;
}

bool IGamepad::input(const Stick stick, const int32_t x, const int32_t y) {
	bool isValid = true;
	for (auto &axises : m_axises) {
		if (axises.type == stick)
			axises.setValue(x, y);

		isValid &= report(EV_ABS, axises.pair[X].axis, axises.pair[X].value);
		isValid &= report(EV_ABS, axises.pair[Y].axis, axises.pair[Y].value);
	}

	isValid &= sync();
	return isValid;
}
