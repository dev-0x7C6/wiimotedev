#include "igamepad.h"
#include <map>

using namespace common::enums;
using namespace io::container;
using namespace io::interface;

IGamepad::IGamepad(std::string &&name, const u32 id, ButtonMap &&buttons, AxisMap &&axises)
		: InputDevice(std::move(name), id)
		, m_buttons(buttons)
		, m_axises(axises)

{
}

Device IGamepad::fromString(const std::string &type) {
	const auto result = convert(type);
	if (result == Device::Last)
		throw std::out_of_range("unknown device type");

	return result;
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

bool IGamepad::input(const int tl, const int tr, const int bl, const int br) {
	static_cast<void>(tl);
	static_cast<void>(tr);
	static_cast<void>(bl);
	static_cast<void>(br);
	return true;
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

int32_t IGamepad::axisMax(const int32_t axis) {
	for (const auto &data : m_axises)
		for (const auto &pair : data.pair)
			if (pair.axis == axis)
				return pair.max;
	return 0;
}

int32_t IGamepad::axisMin(const int32_t axis) {
	for (const auto &data : m_axises)
		for (const auto &pair : data.pair)
			if (pair.axis == axis)
				return pair.min;
	return 0;
}

void IGamepad::setConfiguration(const QJsonObject &json) { m_configuration = json; }
const QJsonObject &IGamepad::configuration() { return m_configuration; }
