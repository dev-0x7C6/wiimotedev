#include "emulation/gamepads/classic-gamepad.h"

using namespace io::emulation::gamepad;

constexpr std::array<ButtonMapping, 15> BUTTONS{{
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

constexpr std::array<AxisPair, 3> AXISES{{
	AxisPair(Stick::Dpad, {{{ABS_HAT0X, 1, -1, 1, -1}, {ABS_HAT0Y, 1, -1, 1, -1}}}),
	AxisPair(Stick::LStick, {{{ABS_X, 0x3A, 0x05, 0x3F, 0x00}, {ABS_Y, 0x3A, 0x05, 0x3F, 0x00}}}),
	AxisPair(Stick::RStick, {{{ABS_RX, 0x1B, 0x04, 0x1F, 0x00}, {ABS_RY, 0x1B, 0x04, 0x1F, 0x00}}}),
}};

ClassicGamepad::ClassicGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}
