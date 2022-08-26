#include "nunchuk-gamepad.h"

using namespace common::enums;
using namespace io::container;
using namespace io::emulation::gamepad;
using namespace io::interface;
using namespace wiimotedev;

constexpr std::array<ButtonMapping, 2> BUTTONS{{
	{WIIMOTEDEV_BTN_Z, BTN_A},
	{WIIMOTEDEV_BTN_C, BTN_X},
}};

constexpr auto MAX = WIIMOTEDEV_STICK_MAX;
constexpr auto MIN = WIIMOTEDEV_STICK_MIN;

constexpr std::array<AxisPair, 1> AXISES{{AxisPair(Stick::Stick, {{{ABS_X, MAX, MIN}, {ABS_Y, MAX, MIN}}})}};

NunchukGamepad::NunchukGamepad(std::string &&name, const u32 id)
		: IGamepad(std::move(name), id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

device NunchukGamepad::type() const { return device::nunchuk; }
