#include "pro-controller-gamepad.h"

using namespace common::enums;
using namespace io::emulation::gamepad;
using namespace wiimotedev;

constexpr std::array<ButtonMapping, 15> BUTTONS{{
	{WIIMOTEDEV_BTN_UP, BTN_DPAD_UP},
	{WIIMOTEDEV_BTN_DOWN, BTN_DPAD_DOWN},
	{WIIMOTEDEV_BTN_LEFT, BTN_DPAD_LEFT},
	{WIIMOTEDEV_BTN_RIGHT, BTN_DPAD_RIGHT},
	{WIIMOTEDEV_BTN_A, BTN_A},
	{WIIMOTEDEV_BTN_B, BTN_B},
	{WIIMOTEDEV_BTN_X, BTN_X},
	{WIIMOTEDEV_BTN_Y, BTN_Y},
	{WIIMOTEDEV_BTN_TL, BTN_TL},
	{WIIMOTEDEV_BTN_TR, BTN_TR},
	{WIIMOTEDEV_BTN_ZL, BTN_TL2},
	{WIIMOTEDEV_BTN_ZR, BTN_TR2},
	{WIIMOTEDEV_BTN_MINUS, BTN_SELECT},
	{WIIMOTEDEV_BTN_HOME, BTN_MODE},
	{WIIMOTEDEV_BTN_PLUS, BTN_START},
}};

constexpr std::array<AxisPair, 3> AXISES{{
	AxisPair(Stick::Dpad, {{{ABS_HAT0X, 1, -1, 1, -1}, {ABS_HAT0Y, 1, -1, 1, -1}}}),
	AxisPair(Stick::LStick, {{{ABS_X, 0x3A, 0x05, 0x3F, 0x00}, {ABS_Y, 0x3A, 0x05, 0x3F, 0x00}}}),
	AxisPair(Stick::RStick, {{{ABS_RX, 0x1B, 0x04, 0x1F, 0x00}, {ABS_RY, 0x1B, 0x04, 0x1F, 0x00}}}),
}};

ProControllerGamepad::ProControllerGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

Device ProControllerGamepad::type() const { return Device::ProController; }
