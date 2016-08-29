#include "emulation/gamepads/wiimote-gamepad.h"

using namespace io::emulation::gamepad;

constexpr auto UINPUT_WIIMOTE_DPAD_UP = BTN_DPAD_UP;
constexpr auto UINPUT_WIIMOTE_DPAD_DOWN = BTN_DPAD_DOWN;
constexpr auto UINPUT_WIIMOTE_DPAD_LEFT = BTN_DPAD_LEFT;
constexpr auto UINPUT_WIIMOTE_DPAD_RIGHT = BTN_DPAD_RIGHT;
constexpr auto UINPUT_WIIMOTE_BTN_A = BTN_A;
constexpr auto UINPUT_WIIMOTE_BTN_B = BTN_B;
constexpr auto UINPUT_WIIMOTE_BTN_1 = BTN_X;
constexpr auto UINPUT_WIIMOTE_BTN_2 = BTN_Y;
constexpr auto UINPUT_WIIMOTE_BTN_START = BTN_START;
constexpr auto UINPUT_WIIMOTE_BTN_SELECT = BTN_SELECT;
constexpr auto UINPUT_WIIMOTE_BTN_HOME = BTN_MODE;

constexpr auto UINPUT_WIIMOTE_DPAD_AXIS_X = ABS_HAT0X;
constexpr auto UINPUT_WIIMOTE_DPAD_AXIS_Y = ABS_HAT0Y;
constexpr auto UINPUT_WIIMOTE_DPAD_MAX = 1;
constexpr auto UINPUT_WIIMOTE_DPAD_MIN = -1;

WiimoteGamepad::WiimoteGamepad(const std::string &name, const uint32_t id, Mode mode, Position horizontal)
		: IGamepad(name, id)
		, m_horizontal(horizontal)
		, m_mode(mode)
		, m_home_switch_position(1)

{
}

IGamepad::Type WiimoteGamepad::type() const { return Type::Wiimote; }

void WiimoteGamepad::setHomeSwitchPosition(bool option) { m_home_switch_position = option; }

bool WiimoteGamepad::input(const uint64_t buttons) {
	report(EV_KEY, UINPUT_WIIMOTE_BTN_A, (buttons & WIIMOTE_BTN_A) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_BTN_B, (buttons & WIIMOTE_BTN_B) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_BTN_1, (buttons & WIIMOTE_BTN_1) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_BTN_2, (buttons & WIIMOTE_BTN_2) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_BTN_START, (buttons & WIIMOTE_BTN_PLUS) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_BTN_SELECT, (buttons & WIIMOTE_BTN_MINUS) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_DPAD_UP, (buttons & WIIMOTE_BTN_RIGHT) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_DPAD_DOWN, (buttons & WIIMOTE_BTN_LEFT) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_DPAD_LEFT, (buttons & WIIMOTE_BTN_DOWN) ? 1 : 0);
	report(EV_KEY, UINPUT_WIIMOTE_DPAD_RIGHT, (buttons & WIIMOTE_BTN_UP) ? 1 : 0);

	if (!m_home_switch_position)
		report(EV_KEY, UINPUT_WIIMOTE_BTN_HOME, (buttons & WIIMOTE_BTN_HOME) ? 1 : 0);

	sync();

	if (m_home_switch_position) {
		if (((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME) && (m_home_pressed == false))
			switch (m_horizontal) {
				case WiimoteGamepad::GamepadHorizontal:
					m_horizontal = WiimoteGamepad::GamepadVertical;
					break;

				case WiimoteGamepad::GamepadVertical:
					m_horizontal = WiimoteGamepad::GamepadHorizontal;
					break;
			}
	}

	m_home_pressed = ((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME);
	m_last_dpad_x = 0;
	m_last_dpad_y = 0;

	if (buttons & WIIMOTE_BTN_RIGHT)
		m_last_dpad_x = UINPUT_WIIMOTE_DPAD_MAX;
	else if (buttons & WIIMOTE_BTN_LEFT)
		m_last_dpad_x = UINPUT_WIIMOTE_DPAD_MIN;

	if (buttons & WIIMOTE_BTN_DOWN)
		m_last_dpad_y = UINPUT_WIIMOTE_DPAD_MAX;
	else if (buttons & WIIMOTE_BTN_UP)
		m_last_dpad_y = UINPUT_WIIMOTE_DPAD_MIN;

	syncSticks();
	return true;
}

void WiimoteGamepad::centerStick(Stick id) {
	switch (id) {
		case Stick::WiimoteDPad:
			m_last_dpad_x = 0;
			m_last_dpad_y = 0;
			break;

		default:
			break;
	}
}

void WiimoteGamepad::syncSticks() {
	switch (m_horizontal) {
		case WiimoteGamepad::GamepadHorizontal:
			report(EV_ABS, UINPUT_WIIMOTE_DPAD_AXIS_X, m_last_dpad_y);
			report(EV_ABS, UINPUT_WIIMOTE_DPAD_AXIS_Y, m_last_dpad_x * -1);
			break;

		case WiimoteGamepad::GamepadVertical:
			report(EV_ABS, UINPUT_WIIMOTE_DPAD_AXIS_X, m_last_dpad_x);
			report(EV_ABS, UINPUT_WIIMOTE_DPAD_AXIS_Y, m_last_dpad_y);
			break;
	}

	sync();
}

bool WiimoteGamepad::configure() {
	set_ev_bit(EV_KEY);
	set_ev_bit(EV_MSC);
	set_ev_bit(EV_ABS);

	set_key_bit(UINPUT_WIIMOTE_DPAD_UP);
	set_key_bit(UINPUT_WIIMOTE_DPAD_DOWN);
	set_key_bit(UINPUT_WIIMOTE_DPAD_LEFT);
	set_key_bit(UINPUT_WIIMOTE_DPAD_RIGHT);
	set_key_bit(UINPUT_WIIMOTE_BTN_A);
	set_key_bit(UINPUT_WIIMOTE_BTN_B);
	set_key_bit(UINPUT_WIIMOTE_BTN_1);
	set_key_bit(UINPUT_WIIMOTE_BTN_2);
	set_key_bit(UINPUT_WIIMOTE_BTN_SELECT);
	set_key_bit(UINPUT_WIIMOTE_BTN_START);

	if (!m_home_switch_position)
		set_key_bit(UINPUT_WIIMOTE_BTN_HOME);

	set_abs_bit(UINPUT_WIIMOTE_DPAD_AXIS_X);
	set_abs_bit(UINPUT_WIIMOTE_DPAD_AXIS_Y);
	set_range(UINPUT_WIIMOTE_DPAD_AXIS_X, UINPUT_WIIMOTE_DPAD_MAX, UINPUT_WIIMOTE_DPAD_MIN);
	set_range(UINPUT_WIIMOTE_DPAD_AXIS_Y, UINPUT_WIIMOTE_DPAD_MAX, UINPUT_WIIMOTE_DPAD_MIN);
}

bool WiimoteGamepad::centerAllAxis() {
	centerStick(Stick::WiimoteDPad);
	syncSticks();
	return true;
}
