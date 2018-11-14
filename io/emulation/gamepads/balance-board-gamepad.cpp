#include "balance-board-gamepad.h"

using namespace common::enums;
using namespace io::container;
using namespace io::emulation::gamepad;
using namespace wiimotedev;

constexpr std::array<ButtonMapping, 0> BUTTONS{{}};

constexpr auto MAX = WIIMOTEDEV_STICK_MAX;
constexpr auto MIN = WIIMOTEDEV_STICK_MIN;

constexpr std::array<AxisPair, 1> AXISES{{
	AxisPair(Stick::Stick, {{{ABS_X, MAX, MIN}, {ABS_Y, MAX, MIN}}}),
}};

BalanceBoardGamepad::BalanceBoardGamepad(std::string &&name, const u32 id)
		: IGamepad(std::move(name), id, {BUTTONS.begin(), BUTTONS.end()}, {AXISES.begin(), AXISES.end()}) {}

Device BalanceBoardGamepad::type() const {
	return Device::BalanceBoard;
}
