#include "gamepad-factory.h"
#include "emulation/gamepads/classic-gamepad.h"
#include "emulation/gamepads/nunchuk-gamepad.h"
#include "emulation/gamepads/wiimote-gamepad.h"

using namespace io::factory;
using namespace io::emulation::gamepad;

std::unique_ptr<IGamepad> GamepadFactory::create(const IGamepad::Type type, const std::string &name, const uint32_t id) {
	switch (type) {
		case IGamepad::Type::Classic: return std::make_unique<ClassicGamepad>(name, id);
		case IGamepad::Type::Nunchuk: return std::make_unique<NunchukGamepad>(name, id);
		case IGamepad::Type::Wiimote: return std::make_unique<WiimoteGamepad>(name, id);
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
