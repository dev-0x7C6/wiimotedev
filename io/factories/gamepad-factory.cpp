#include "gamepad-factory.h"
#include "emulation/gamepads/classic-gamepad.h"
#include "emulation/gamepads/nunchuk-gamepad.h"
#include "emulation/gamepads/wiimote-gamepad.h"
#include "emulation/gamepads/balance-board-gamepad.h"

using namespace common::enums;
using namespace io::emulation::gamepad;
using namespace io::factory;
using namespace io::interface;

std::unique_ptr<IGamepad> GamepadFactory::create(const Device type, const std::string &name, const uint32_t id) {
	switch (type) {
		case Device::Classic: return std::make_unique<ClassicGamepad>(name, id);
		case Device::Nunchuk: return std::make_unique<NunchukGamepad>(name, id);
		case Device::Wiimote: return std::make_unique<WiimoteGamepad>(name, id);
		case Device::BalanceBoard: return std::make_unique<BalanceBoardGamepad>(name, id);
		default:
			return nullptr;
	}

	return nullptr;
}

bool GamepadFactory::configure(std::unique_ptr<IGamepad> &interface) {
	bool isValid = true;
	isValid &= interface->open();
	isValid &= interface->configure();
	isValid &= interface->create();
	return isValid;
}
