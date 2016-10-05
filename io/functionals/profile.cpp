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
	org::wiimotedev::classic classic("org.wiimotedev.daemon", "/classic", QDBusConnection::systemBus(), this);
	org::wiimotedev::nunchuk nunchuk("org.wiimotedev.daemon", "/nunchuk", QDBusConnection::systemBus(), this);
	org::wiimotedev::procontroller procontroller("org.wiimotedev.daemon", "/procontroller", QDBusConnection::systemBus(), this);
	org::wiimotedev::wiimote wiimote("org.wiimotedev.daemon", "/wiimote", QDBusConnection::systemBus(), this);

	connect(&balanceboard, &org::wiimotedev::balanceboard::balanceBoardConnected, [this](uint id) { connected(enums::Source::BalanceBoard, id); });
	connect(&classic, &org::wiimotedev::classic::classicConnected, [this](uint id) { connected(enums::Source::Classic, id); });
	connect(&nunchuk, &org::wiimotedev::nunchuk::nunchukConnected, [this](uint id) { connected(enums::Source::Nunchuk, id); });
	connect(&procontroller, &org::wiimotedev::procontroller::procontrollerConnected, [this](uint id) { connected(enums::Source::ProController, id); });
	connect(&wiimote, &org::wiimotedev::wiimote::wiimoteConnected, [this](uint id) { connected(enums::Source::Wiimote, id); });

	connect(&balanceboard, &org::wiimotedev::balanceboard::balanceBoardDisconnected, [this](uint id) { disconnected(enums::Source::BalanceBoard, id); });
	connect(&classic, &org::wiimotedev::classic::classicDisconnected, [this](uint id) { disconnected(enums::Source::Classic, id); });
	connect(&nunchuk, &org::wiimotedev::nunchuk::nunchukDisconnected, [this](uint id) { disconnected(enums::Source::Nunchuk, id); });
	connect(&procontroller, &org::wiimotedev::procontroller::procontrollerDisconnected, [this](uint id) { disconnected(enums::Source::ProController, id); });
	connect(&wiimote, &org::wiimotedev::wiimote::wiimoteDisconnected, [this](uint id) { disconnected(enums::Source::Wiimote, id); });

	connect(&classic, &org::wiimotedev::classic::classicButtonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(enums::Source::Wiimote, id, mask);
	});

	connect(&nunchuk, &org::wiimotedev::nunchuk::nunchukButtonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(enums::Source::Nunchuk, id, mask);
	});

	connect(&procontroller, &org::wiimotedev::procontroller::procontrollerButtonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(enums::Source::ProController, id, mask);
	});

	connect(&wiimote, &org::wiimotedev::wiimote::wiimoteButtonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(enums::Source::Wiimote, id, mask);
	});

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

void Profile::connected(io::enums::Source, uint id) {
}

void Profile::disconnected(io::enums::Source, uint id) {
}

void Profile::buttonDataChanged(io::enums::Source source, uint id, qulonglong mask) {
	if (source == enums::Source::Classic) {
		gamepad_iterator(IGamepad::Type::Classic, id, [&mask](const auto &gamepad) { gamepad->input(mask); });
	}

	if (source == enums::Source::Nunchuk) {
		gamepad_iterator(IGamepad::Type::Nunchuk, id, [&mask](const auto &gamepad) { gamepad->input(mask); });
	}

	if (source == enums::Source::Wiimote) {
		gamepad_iterator(IGamepad::Type::Wiimote, id, [&mask](const auto &gamepad) { gamepad->input(mask); });
	}
}

void Profile::stickDataChanged(io::enums::Source, uint id, int lx, int ly, int rx, int ry) {
}

void Profile::accelerometerDataChanged(io::enums::Source, uint id, int x, int y, int z) {
}

void Profile::gyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ) {
}

void Profile::infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
}

void Profile::gamepad_iterator(const IGamepad::Type type, const quint32 id, std::function<void(const std::unique_ptr<IGamepad> &)> &&function) {
	for (const auto &gamepad : m_gamepads) {
		if (gamepad->id() == id && gamepad->type() == type && gamepad->isCreated()) {
			function(gamepad);
		}
	}
}

bool Profile::setup(const IGamepad::Type type, const std::string &name, quint32 id) {
	auto device = GamepadFactory::create(type, name, id);
	auto isValid = GamepadFactory::configure(device);

	if (isValid)
		m_gamepads.emplace_back(std::move(device));

	return isValid;
}
