#include "gamepad-factory.h"
#include "emulation/gamepads/balance-board-gamepad.h"
#include "emulation/gamepads/classic-gamepad.h"
#include "emulation/gamepads/nunchuk-gamepad.h"
#include "emulation/gamepads/pro-controller-gamepad.h"
#include "emulation/gamepads/wiimote-gamepad.h"

using namespace common::enums;
using namespace io::emulation::gamepad;
using namespace io::factory;
using namespace io::interface;

std::unique_ptr<IGamepad> GamepadFactory::create(const device type, std::string &&name, const u32 id) {
	switch (type) {
		case device::classic_controller: return std::make_unique<ClassicGamepad>(std::move(name), id);
		case device::nunchuk: return std::make_unique<NunchukGamepad>(std::move(name), id);
		case device::wiimote: return std::make_unique<WiimoteGamepad>(std::move(name), id);
		case device::balance_board: return std::make_unique<BalanceBoardGamepad>(std::move(name), id);
		case device::pro_controller: return std::make_unique<ProControllerGamepad>(std::move(name), id);
		case device::motion_plus: return nullptr;
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
