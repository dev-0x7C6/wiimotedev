#include "emulation/gamepads/wiimote-gamepad.h"

using namespace io::emulation::gamepad;

constexpr std::array<ButtonMapping, 11> BUTTONS{{
	{WIIMOTE_BTN_A, BTN_A},
	{WIIMOTE_BTN_B, BTN_B},
	{WIIMOTE_BTN_1, BTN_X},
	{WIIMOTE_BTN_2, BTN_Y},
	{WIIMOTE_BTN_PLUS, BTN_START},
	{WIIMOTE_BTN_HOME, BTN_MODE},
	{WIIMOTE_BTN_MINUS, BTN_SELECT},
	{WIIMOTE_BTN_UP, BTN_DPAD_UP},
	{WIIMOTE_BTN_DOWN, BTN_DPAD_DOWN},
	{WIIMOTE_BTN_LEFT, BTN_DPAD_LEFT},
	{WIIMOTE_BTN_RIGHT, BTN_DPAD_RIGHT},
}};

constexpr std::array<AxisMapping, 2> AXISES{{
	{ABS_HAT0X, 1, -1, 1, -1},
	{ABS_HAT0Y, 1, -1, 1, -1},
}};

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
	bool isValid = toggle_buttons<decltype(BUTTONS)>(BUTTONS, buttons);

	if (!m_home_switch_position)
		report(EV_KEY, WIIMOTE_BTN_HOME, (buttons & WIIMOTE_BTN_HOME) ? 1 : 0);

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
	centerStick(Stick::WiimoteDPad);
	m_last_dpad_x = (buttons & WIIMOTE_BTN_RIGHT) ? AXISES[0].max : m_last_dpad_x;
	m_last_dpad_x = (buttons & WIIMOTE_BTN_LEFT) ? AXISES[0].min : m_last_dpad_x;
	m_last_dpad_y = (buttons & WIIMOTE_BTN_DOWN) ? AXISES[1].max : m_last_dpad_y;
	m_last_dpad_y = (buttons & WIIMOTE_BTN_UP) ? AXISES[1].min : m_last_dpad_y;
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
			report(EV_ABS, AXISES[0].axis, m_last_dpad_y);
			report(EV_ABS, AXISES[1].axis, m_last_dpad_x * -1);
			break;

		case WiimoteGamepad::GamepadVertical:
			report(EV_ABS, AXISES[0].axis, m_last_dpad_x);
			report(EV_ABS, AXISES[1].axis, m_last_dpad_y);
			break;
	}

	sync();
}

bool WiimoteGamepad::configure() {
	return configure_gamepad<decltype(BUTTONS), decltype(AXISES)>(BUTTONS, AXISES);
}

bool WiimoteGamepad::centerAllAxis() {
	centerStick(Stick::WiimoteDPad);
	syncSticks();
	return true;
}
