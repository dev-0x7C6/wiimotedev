#include "emulation/gamepads/wiimote-gamepad.h"

using namespace io::emulation::gamepad;
using namespace io::interface;
using namespace wiimotedev;

constexpr std::array<ButtonMapping, 11> BUTTONS{{
	{WIIMOTEDEV_BTN_A, BTN_A},
	{WIIMOTEDEV_BTN_B, BTN_B},
	{WIIMOTEDEV_BTN_ONE, BTN_X},
	{WIIMOTEDEV_BTN_TWO, BTN_Y},
	{WIIMOTEDEV_BTN_PLUS, BTN_START},
	{WIIMOTEDEV_BTN_HOME, BTN_MODE},
	{WIIMOTEDEV_BTN_MINUS, BTN_SELECT},
	{WIIMOTEDEV_BTN_UP, BTN_DPAD_UP},
	{WIIMOTEDEV_BTN_DOWN, BTN_DPAD_DOWN},
	{WIIMOTEDEV_BTN_LEFT, BTN_DPAD_LEFT},
	{WIIMOTEDEV_BTN_RIGHT, BTN_DPAD_RIGHT},
}};

constexpr std::array<AxisPair, 1> AXISES{{
	AxisPair(Stick::Dpad, {{{ABS_HAT0X, 1, -1, 1, -1}, {ABS_HAT0Y, 1, -1, 1, -1}}}),
}};

WiimoteGamepad::WiimoteGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

IGamepad::Type WiimoteGamepad::type() const { return Type::Wiimote; }
