#include "emulation/gamepads/classic-gamepad.h"

using namespace io::emulation::gamepad;

constexpr auto UINPUT_CLASSIC_BTN_DPAD_UP = BTN_DPAD_UP;
constexpr auto UINPUT_CLASSIC_BTN_DPAD_DOWN = BTN_DPAD_DOWN;
constexpr auto UINPUT_CLASSIC_BTN_DPAD_LEFT = BTN_DPAD_LEFT;
constexpr auto UINPUT_CLASSIC_BTN_DPAD_RIGHT = BTN_DPAD_RIGHT;
constexpr auto UINPUT_CLASSIC_BTN_A = BTN_A;
constexpr auto UINPUT_CLASSIC_BTN_B = BTN_B;
constexpr auto UINPUT_CLASSIC_BTN_X = BTN_X;
constexpr auto UINPUT_CLASSIC_BTN_Y = BTN_Y;
constexpr auto UINPUT_CLASSIC_BTN_L = BTN_TL;
constexpr auto UINPUT_CLASSIC_BTN_R = BTN_TR;
constexpr auto UINPUT_CLASSIC_BTN_ZL = BTN_TL2;
constexpr auto UINPUT_CLASSIC_BTN_ZR = BTN_TR2;
constexpr auto UINPUT_CLASSIC_BTN_SELECT = BTN_SELECT;
constexpr auto UINPUT_CLASSIC_BTN_HOME = BTN_MODE;
constexpr auto UINPUT_CLASSIC_BTN_START = BTN_START;
constexpr auto UINPUT_CLASSIC_LSTICK_AXIS_X = ABS_X;
constexpr auto UINPUT_CLASSIC_LSTICK_AXIS_Y = ABS_Y;
constexpr auto UINPUT_CLASSIC_RSTICK_AXIS_X = ABS_RX;
constexpr auto UINPUT_CLASSIC_RSTICK_AXIS_Y = ABS_RY;
constexpr auto UINPUT_CLASSIC_DPAD_AXIS_X = ABS_HAT0X;
constexpr auto UINPUT_CLASSIC_DPAD_AXIS_Y = ABS_HAT0Y;
constexpr auto UINPUT_CLASSIC_LSTICK_MAX = 0x3B;
constexpr auto UINPUT_CLASSIC_LSTICK_MIN = 0x09;
constexpr auto UINPUT_CLASSIC_RSTICK_MAX = 0x1B;
constexpr auto UINPUT_CLASSIC_RSTICK_MIN = 0x02;
constexpr auto UINPUT_CLASSIC_DPAD_MAX = 1;
constexpr auto UINPUT_CLASSIC_DPAD_MIN = -1;

ClassicGamepad::ClassicGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id)
		, m_last_r_stick_x(0)
		, m_last_r_stick_y(0)
		, m_last_l_stick_x(0)
		, m_last_l_stick_y(0)
		, m_last_dpad_x(0)
		, m_last_dpad_y(0)

{
	centerStick(ClassicGamepad::LeftStick);
	centerStick(ClassicGamepad::RightStick);
	centerStick(ClassicGamepad::DpadStick);
}

IGamepad::Type ClassicGamepad::type() const { return Type::ClassicController; }

bool ClassicGamepad::inputButtons(const uint64_t buttons) {
	report(EV_KEY, UINPUT_CLASSIC_BTN_DPAD_UP, (buttons & CLASSIC_BTN_UP) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_DPAD_DOWN, (buttons & CLASSIC_BTN_DOWN) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_DPAD_LEFT, (buttons & CLASSIC_BTN_LEFT) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_DPAD_RIGHT, (buttons & CLASSIC_BTN_RIGHT) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_A, (buttons & CLASSIC_BTN_A) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_B, (buttons & CLASSIC_BTN_B) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_X, (buttons & CLASSIC_BTN_X) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_Y, (buttons & CLASSIC_BTN_Y) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_L, (buttons & CLASSIC_BTN_L) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_R, (buttons & CLASSIC_BTN_R) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_ZL, (buttons & CLASSIC_BTN_ZL) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_ZR, (buttons & CLASSIC_BTN_ZR) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_SELECT, (buttons & CLASSIC_BTN_MINUS) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_HOME, (buttons & CLASSIC_BTN_HOME) ? 1 : 0);
	report(EV_KEY, UINPUT_CLASSIC_BTN_START, (buttons & CLASSIC_BTN_PLUS) ? 1 : 0);
	sync();

	centerStick(ClassicGamepad::DpadStick);
	m_last_dpad_x = (buttons & CLASSIC_BTN_RIGHT) ? UINPUT_CLASSIC_DPAD_MAX : m_last_dpad_x;
	m_last_dpad_x = (buttons & CLASSIC_BTN_LEFT) ? UINPUT_CLASSIC_DPAD_MIN : m_last_dpad_x;
	m_last_dpad_y = (buttons & CLASSIC_BTN_DOWN) ? UINPUT_CLASSIC_DPAD_MAX : m_last_dpad_y;
	m_last_dpad_y = (buttons & CLASSIC_BTN_UP) ? UINPUT_CLASSIC_DPAD_MIN : m_last_dpad_y;
	syncSticks();
}

bool ClassicGamepad::inputStick(const IGamepad::Stick stick, const int32_t x, const int32_t y) {
	switch (stick) {
		case Stick::ClassicControllerLStick:
			m_last_l_stick_x = std::max(UINPUT_CLASSIC_LSTICK_MIN, std::min(x, UINPUT_CLASSIC_LSTICK_MAX));
			m_last_l_stick_y = std::max(UINPUT_CLASSIC_LSTICK_MIN, std::min(0x41 - y, UINPUT_CLASSIC_LSTICK_MAX));
			break;

		case Stick::ClassicControllerRStick:
			m_last_r_stick_x = std::max(UINPUT_CLASSIC_RSTICK_MIN, std::min(x, UINPUT_CLASSIC_RSTICK_MAX));
			m_last_r_stick_y = std::max(UINPUT_CLASSIC_RSTICK_MIN, std::min(0x1F - y, UINPUT_CLASSIC_RSTICK_MAX));
			break;
		default:
			break;
	}

	syncSticks();
}

bool ClassicGamepad::inputAccelerometer(const double pitch, const double roll) {
	static_cast<void>(pitch);
	static_cast<void>(roll);
	return false;
}

void ClassicGamepad::centerStick(Sticks stick) {
	switch (stick) {
		case ClassicGamepad::LeftStick:
			m_last_l_stick_x = (UINPUT_CLASSIC_LSTICK_MIN + UINPUT_CLASSIC_LSTICK_MAX) / 2;
			m_last_l_stick_y = (UINPUT_CLASSIC_LSTICK_MIN + UINPUT_CLASSIC_LSTICK_MAX) / 2;
			break;

		case ClassicGamepad::RightStick:
			m_last_r_stick_x = (UINPUT_CLASSIC_RSTICK_MIN + UINPUT_CLASSIC_RSTICK_MAX) / 2;
			m_last_r_stick_y = (UINPUT_CLASSIC_RSTICK_MIN + UINPUT_CLASSIC_RSTICK_MAX) / 2;
			break;

		case ClassicGamepad::DpadStick:
			m_last_dpad_x = 0;
			m_last_dpad_y = 0;
			break;
	}
}

void ClassicGamepad::syncSticks() {
	report(EV_ABS, UINPUT_CLASSIC_DPAD_AXIS_X, m_last_dpad_x);
	report(EV_ABS, UINPUT_CLASSIC_DPAD_AXIS_Y, m_last_dpad_y);
	report(EV_ABS, UINPUT_CLASSIC_RSTICK_AXIS_X, m_last_r_stick_x);
	report(EV_ABS, UINPUT_CLASSIC_RSTICK_AXIS_Y, m_last_r_stick_y);
	report(EV_ABS, UINPUT_CLASSIC_LSTICK_AXIS_X, m_last_l_stick_x);
	report(EV_ABS, UINPUT_CLASSIC_LSTICK_AXIS_Y, m_last_l_stick_y);
	sync();
}

bool ClassicGamepad::configure() {
	bool isValid = true;
	isValid &= set_ev_bit(EV_KEY) == 0;
	isValid &= set_ev_bit(EV_MSC) == 0;
	isValid &= set_ev_bit(EV_ABS) == 0;

	static_assert(BTN_A == BTN_GAMEPAD, "BTN_A should be equal with BTN_GAMEPAD.");

	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_DPAD_UP) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_DPAD_DOWN) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_DPAD_LEFT) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_DPAD_RIGHT) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_A) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_B) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_X) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_Y) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_L) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_R) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_ZL) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_ZR) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_SELECT) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_HOME) == 0;
	isValid &= set_key_bit(UINPUT_CLASSIC_BTN_START) == 0;

	isValid &= set_abs_bit(UINPUT_CLASSIC_LSTICK_AXIS_X) == 0;
	isValid &= set_abs_bit(UINPUT_CLASSIC_LSTICK_AXIS_Y) == 0;
	set_range(UINPUT_CLASSIC_LSTICK_AXIS_X, UINPUT_CLASSIC_LSTICK_MAX, UINPUT_CLASSIC_LSTICK_MIN);
	set_range(UINPUT_CLASSIC_LSTICK_AXIS_Y, UINPUT_CLASSIC_LSTICK_MAX, UINPUT_CLASSIC_LSTICK_MIN);

	isValid &= set_abs_bit(UINPUT_CLASSIC_RSTICK_AXIS_X) == 0;
	isValid &= set_abs_bit(UINPUT_CLASSIC_RSTICK_AXIS_Y) == 0;
	set_range(UINPUT_CLASSIC_RSTICK_AXIS_X, UINPUT_CLASSIC_RSTICK_MAX, UINPUT_CLASSIC_RSTICK_MIN);
	set_range(UINPUT_CLASSIC_RSTICK_AXIS_Y, UINPUT_CLASSIC_RSTICK_MAX, UINPUT_CLASSIC_RSTICK_MIN);

	isValid &= set_abs_bit(UINPUT_CLASSIC_DPAD_AXIS_X) == 0;
	isValid &= set_abs_bit(UINPUT_CLASSIC_DPAD_AXIS_Y) == 0;
	set_range(UINPUT_CLASSIC_DPAD_AXIS_X, UINPUT_CLASSIC_DPAD_MAX, UINPUT_CLASSIC_DPAD_MIN);
	set_range(UINPUT_CLASSIC_DPAD_AXIS_Y, UINPUT_CLASSIC_DPAD_MAX, UINPUT_CLASSIC_DPAD_MIN);

	return isValid;
}
