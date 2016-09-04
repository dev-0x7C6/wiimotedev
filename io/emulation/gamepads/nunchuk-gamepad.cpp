#include "emulation/gamepads/nunchuk-gamepad.h"

using namespace io::emulation::gamepad;

constexpr std::array<ButtonMapping, 2> BUTTONS{{
	{NUNCHUK_BTN_Z, BTN_A},
	{NUNCHUK_BTN_C, BTN_X},
}};

constexpr std::array<AxisPair, 1> AXISES{{AxisPair(Stick::Stick, {{{ABS_X, 0xFF - 0x1F, 0x1F, 0xFF, 0x00}, {ABS_Y, 0xFF - 0x1F, 0x1F, 0xFF, 0x00}}})}};

NunchukGamepad::NunchukGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

IGamepad::Type NunchukGamepad::type() const { return Type::Nunchuk; }
