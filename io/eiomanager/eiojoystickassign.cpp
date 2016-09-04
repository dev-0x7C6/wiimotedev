#include "manager.h"

#include "emulation/gamepads/classic-gamepad.h"
#include "emulation/gamepads/nunchuk-gamepad.h"
#include "emulation/gamepads/wiimote-gamepad.h"

using namespace io::emulation::gamepad;

void UInputProfileManager::setupClassicJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<ClassicGamepad>(name.toStdString(), assign);
	device->open();
	device->configure();

	if (device->create())
		m_gamepads.emplace_back(std::move(device));
}

void UInputProfileManager::setupWiimoteJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<WiimoteGamepad>(name.toStdString(), assign);
	device->open();
	device->configure();

	if (device->create())
		m_gamepads.emplace_back(std::move(device));
}

void UInputProfileManager::setupNunchukJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<NunchukGamepad>(name.toStdString(), assign);
	device->open();
	device->configure();

	if (device->create())
		m_gamepads.emplace_back(std::move(device));
}

void UInputProfileManager::assignJoystickEvents(const QString &key, QSettings &settings) {
	settings.beginGroup(key);
	uint32_t assign = settings.value("assign").toULongLong();
	QString device = settings.value("device").toString().toLower();
	QString name = settings.value("name").toString();

	if (device == QString::fromUtf8("classic"))
		setupClassicJoystick(assign, name, settings);
	else if (device == QString::fromUtf8("nunchuk"))
		setupNunchukJoystick(assign, name, settings);
	else if (device == QString::fromUtf8("wiimote"))
		setupWiimoteJoystick(assign, name, settings);

	settings.endGroup();
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
