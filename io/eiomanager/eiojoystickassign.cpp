#include "manager.h"
#include "factories/gamepad-factory.h"

#include <iostream>

using namespace io::factory;
using namespace io::interface;

void UInputProfileManager::assignJoystickEvents(const QString &key, QSettings &settings) {
	settings.beginGroup(key);
	auto assign = settings.value("assign").toUInt();
	auto device = settings.value("device").toString().toLower().toStdString();
	auto name = settings.value("name").toString().toStdString();

	try {
		setup(IGamepad::fromString(device), name, assign);
	} catch (std::out_of_range &) {
		std::cout << "unknown gamepad module \"" << device << "\"" << std::endl;
	}

	settings.endGroup();
}

bool UInputProfileManager::setup(const IGamepad::Type type, const std::string &name, uint32_t id) {
	auto device = GamepadFactory::create(type, name, id);
	auto isValid = GamepadFactory::configure(device);
	m_gamepads.emplace_back(std::move(device));
	return isValid;
}

void UInputProfileManager::gamepad_iterator(const IGamepad::Type type, const uint32_t id, std::function<void(const std::unique_ptr<IGamepad> &)> &&function) {
	for (const auto &gamepad : m_gamepads) {
		if (gamepad->id() == id && gamepad->type() == type && gamepad->isCreated()) {
			function(gamepad);
		}
	}
}

void UInputProfileManager::dbusWiimoteButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Wiimote, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void UInputProfileManager::dbusNunchukButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void UInputProfileManager::dbusNunchukStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&stick](const auto &gamepad) { gamepad->input(Stick::Stick, stick.x, stick.y); });
}

void UInputProfileManager::dbusClassicControllerButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void UInputProfileManager::dbusClassicControllerLStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&stick](const auto &gamepad) { gamepad->input(Stick::LStick, stick.x, stick.y); });
}

void UInputProfileManager::dbusClassicControllerRStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&stick](const auto &gamepad) { gamepad->input(Stick::RStick, stick.x, stick.y); });
}
