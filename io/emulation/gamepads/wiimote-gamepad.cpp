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

constexpr std::array<AxisPair, 1> AXISES{{
	AxisPair(Stick::Dpad, {{{ABS_HAT0X, 1, -1, 1, -1}, {ABS_HAT0Y, 1, -1, 1, -1}}}),
}};

WiimoteGamepad::WiimoteGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

IGamepad::Type WiimoteGamepad::type() const { return Type::Wiimote; }
