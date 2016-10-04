#include "profile.h"

#include <iostream>
#include <QSettings>
#include <QDBusConnection>

#include "factories/gamepad-factory.h"
#include "interfaces/igamepad.h"

#include "dbus/interfaces/balanceboard.h"
#include "dbus/interfaces/classic.h"
#include "dbus/interfaces/nunchuk.h"
#include "dbus/interfaces/procontroller.h"
#include "dbus/interfaces/wiimote.h"

using namespace io::factory;
using namespace io::interface;
using namespace io::functional;

Profile::Profile(const std::string &configurationFilePath)
		: QObject(nullptr)
		, IProfile(configurationFilePath)

{
	org::wiimotedev::balanceboard balanceboard("org.wiimotedev.daemon", "/balanceboard", QDBusConnection::systemBus(), this);

	QSettings settings(QString::fromStdString(configurationFilePath), QSettings::IniFormat);
	for (const auto &group : settings.childGroups()) {
		if (settings.value(group + "/module", {}).toString() != "gamepad")
			continue;

		settings.beginGroup(group);
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
}

Profile::~Profile() {
	m_gamepads.clear();
}

void Profile::gamepad_iterator(const IGamepad::Type type, const quint32 id, std::function<void(const std::unique_ptr<IGamepad> &)> &&function) {
	for (const auto &gamepad : m_gamepads) {
		if (gamepad->id() == id && gamepad->type() == type && gamepad->isCreated()) {
			function(gamepad);
		}
	}
}

void Profile::classicControllerButtons(quint32 id, quint64 buttons) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

//void Profile::classicControllerLStick(quint32 id, stickdata stick) {
//	gamepad_iterator(IGamepad::Type::Classic, id,
//		[&stick](const auto &gamepad) { gamepad->input(Stick::LStick, stick.x, stick.y); });
//}

//void Profile::classicControllerRStick(quint32 id, stickdata stick) {
//	gamepad_iterator(IGamepad::Type::Classic, id,
//		[&stick](const auto &gamepad) { gamepad->input(Stick::RStick, stick.x, stick.y); });
//}

void Profile::nunchukButtons(quint32 id, quint64 buttons) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

//void Profile::nunchukStick(quint32 id, stickdata stick) {
//	gamepad_iterator(IGamepad::Type::Nunchuk, id,
//		[&stick](const auto &gamepad) { gamepad->input(Stick::Stick, stick.x, stick.y); });
//}

void Profile::wiimoteButtons(quint32 id, quint64 buttons) {
	gamepad_iterator(IGamepad::Type::Wiimote, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void Profile::wiimoteGeneralButtons(quint32 id, quint64 buttons) {
	static_cast<void>(id);
	static_cast<void>(buttons);
}

bool Profile::setup(const IGamepad::Type type, const std::string &name, quint32 id) {
	auto device = GamepadFactory::create(type, name, id);
	auto isValid = GamepadFactory::configure(device);

	if (isValid)
		m_gamepads.emplace_back(std::move(device));

	return isValid;
}
