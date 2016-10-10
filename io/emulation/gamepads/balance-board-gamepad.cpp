#include "balance-board-gamepad.h"

using namespace common::enums;
using namespace io::emulation::gamepad;

constexpr std::array<ButtonMapping, 0> BUTTONS{{}};

constexpr std::array<AxisPair, 1> AXISES{{
	AxisPair(Stick::Stick, {{{ABS_X, 0x1FFF, 0, 0x1FFF, 0}, {ABS_Y, 0x1FFF, 0, 0x1FFF, 0}}}),
}};

BalanceBoardGamepad::BalanceBoardGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

Device BalanceBoardGamepad::type() const {
	return Device::BalanceBoard;
}
