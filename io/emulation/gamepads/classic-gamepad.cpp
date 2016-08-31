#include "emulation/gamepads/classic-gamepad.h"

#include <vector>

using namespace io::emulation::gamepad;

constexpr std::array<ButtonMapping, 15> CLASSIC_BUTTON_LAYOUT {{
	{CLASSIC_BTN_UP, BTN_DPAD_UP},
	{CLASSIC_BTN_DOWN, BTN_DPAD_DOWN},
	{CLASSIC_BTN_LEFT, BTN_DPAD_LEFT},
	{CLASSIC_BTN_RIGHT, BTN_DPAD_RIGHT},
	{CLASSIC_BTN_A, BTN_A},
	{CLASSIC_BTN_B, BTN_B},
	{CLASSIC_BTN_X, BTN_X},
	{CLASSIC_BTN_Y, BTN_Y},
	{CLASSIC_BTN_L, BTN_TL},
	{CLASSIC_BTN_R, BTN_TR},
	{CLASSIC_BTN_ZL, BTN_TL2},
	{CLASSIC_BTN_ZR, BTN_TR2},
	{CLASSIC_BTN_MINUS, BTN_SELECT},
	{CLASSIC_BTN_HOME, BTN_MODE},
	{CLASSIC_BTN_PLUS, BTN_START},
}};

constexpr std::array<AxisMapping, 6> CLASSIC_AXIS_LAYOUT {{
	{ABS_HAT0X, 1, -1, 1, -1},
	{ABS_HAT0Y, 1, -1, 1, -1},
	{ABS_X, 0x3B, 0x09, 0x3F, 0x00},
	{ABS_Y, 0x3B, 0x09, 0x3F, 0x00},
	{ABS_RX, 0x1B, 0x02, 0x1F, 0x00},
	{ABS_RY, 0x1B, 0x02, 0x1F, 0x00},
}};

ClassicGamepad::ClassicGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id) {
	m_axis.fill(0);
	centerStick(Stick::ClassicLStick);
	centerStick(Stick::ClassicRStick);
	centerStick(Stick::ClassicDPad);
}

IGamepad::Type ClassicGamepad::type() const { return Type::Classic; }

bool ClassicGamepad::input(const uint64_t buttons) {
	bool isValid = true;
	for (const auto &value : CLASSIC_BUTTON_LAYOUT)
		isValid &= report(EV_KEY, value.uinput_btn, (buttons & value.wiimotedev_btn) ? 1 : 0);

	centerStick(Stick::ClassicDPad);
	m_axis[0] = (buttons & CLASSIC_BTN_RIGHT) ? CLASSIC_AXIS_LAYOUT[0].max : m_axis[0];
	m_axis[0] = (buttons & CLASSIC_BTN_LEFT) ? CLASSIC_AXIS_LAYOUT[0].min : m_axis[0];
	m_axis[1] = (buttons & CLASSIC_BTN_DOWN) ? CLASSIC_AXIS_LAYOUT[1].max : m_axis[1];
	m_axis[1] = (buttons & CLASSIC_BTN_UP) ? CLASSIC_AXIS_LAYOUT[1].min : m_axis[1];
	syncSticks();

	return isValid;
}

bool ClassicGamepad::input(const IGamepad::Stick stick, const int32_t x, const int32_t y) {
	switch (stick) {
		case Stick::ClassicLStick:
			m_axis[2] = std::max(CLASSIC_AXIS_LAYOUT[2].min, std::min(x, CLASSIC_AXIS_LAYOUT[2].max));
			m_axis[3] = std::max(CLASSIC_AXIS_LAYOUT[3].min, std::min(0x41 - y, CLASSIC_AXIS_LAYOUT[3].max));
			break;

		case Stick::ClassicRStick:
			m_axis[4] = std::max(CLASSIC_AXIS_LAYOUT[4].min, std::min(x, CLASSIC_AXIS_LAYOUT[4].max));
			m_axis[5] = std::max(CLASSIC_AXIS_LAYOUT[5].min, std::min(0x1F - y, CLASSIC_AXIS_LAYOUT[5].max));
			break;

		default:
			break;
	}

	syncSticks();
}

void ClassicGamepad::centerStick(Stick stick) {
	switch (stick) {
		case Stick::ClassicDPad:
			m_axis[0] = 0;
			m_axis[1] = 0;
			break;

		case Stick::ClassicLStick:
			m_axis[2] = (CLASSIC_AXIS_LAYOUT[2].min + CLASSIC_AXIS_LAYOUT[2].max) / 2;
			m_axis[3] = (CLASSIC_AXIS_LAYOUT[3].min + CLASSIC_AXIS_LAYOUT[3].max) / 2;
			break;

		case Stick::ClassicRStick:
			m_axis[4] = (CLASSIC_AXIS_LAYOUT[4].min + CLASSIC_AXIS_LAYOUT[4].max) / 2;
			m_axis[5] = (CLASSIC_AXIS_LAYOUT[5].min + CLASSIC_AXIS_LAYOUT[5].max) / 2;
			break;

		default: break;
	}
}

void ClassicGamepad::syncSticks() {
	auto idx = 0;
	for (const auto &axis : CLASSIC_AXIS_LAYOUT)
		report(EV_ABS, axis.axis, m_axis[idx]);

	sync();
}

bool ClassicGamepad::configure() {
	bool isValid = true;
	isValid &= set_ev_bit(EV_KEY) == 0;
	isValid &= set_ev_bit(EV_ABS) == 0;

	for (const auto &value : CLASSIC_BUTTON_LAYOUT)
		isValid &= set_key_bit(value.uinput_btn) == 0;

	for (const auto &value : CLASSIC_AXIS_LAYOUT) {
		isValid &= set_abs_bit(value.axis) == 0;
		set_range(value.axis, value.max, value.min);
	}

	return isValid;
}

bool ClassicGamepad::centerAllAxis() {
	centerStick(Stick::ClassicDPad);
	centerStick(Stick::ClassicRStick);
	centerStick(Stick::ClassicLStick);
	syncSticks();
	return true;
}
