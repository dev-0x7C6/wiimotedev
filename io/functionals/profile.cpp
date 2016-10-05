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

using namespace common::enums;
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

	connect(&balanceboard, &org::wiimotedev::balanceboard::connected, [this](uint id) { connected(common::enums::Device::BalanceBoard, id); });
	connect(&classic, &org::wiimotedev::classic::connected, [this](uint id) { connected(common::enums::Device::Classic, id); });
	connect(&nunchuk, &org::wiimotedev::nunchuk::connected, [this](uint id) { connected(common::enums::Device::Nunchuk, id); });
	connect(&procontroller, &org::wiimotedev::procontroller::connected, [this](uint id) { connected(common::enums::Device::ProController, id); });
	connect(&wiimote, &org::wiimotedev::wiimote::connected, [this](uint id) { connected(common::enums::Device::Wiimote, id); });

	connect(&balanceboard, &org::wiimotedev::balanceboard::disconnected, [this](uint id) { disconnected(common::enums::Device::BalanceBoard, id); });
	connect(&classic, &org::wiimotedev::classic::disconnected, [this](uint id) { disconnected(common::enums::Device::Classic, id); });
	connect(&nunchuk, &org::wiimotedev::nunchuk::disconnected, [this](uint id) { disconnected(common::enums::Device::Nunchuk, id); });
	connect(&procontroller, &org::wiimotedev::procontroller::disconnected, [this](uint id) { disconnected(common::enums::Device::ProController, id); });
	connect(&wiimote, &org::wiimotedev::wiimote::disconnected, [this](uint id) { disconnected(common::enums::Device::Wiimote, id); });

	connect(&classic, &org::wiimotedev::classic::buttonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(common::enums::Device::Classic, id, mask);
	});

	connect(&nunchuk, &org::wiimotedev::nunchuk::buttonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(common::enums::Device::Nunchuk, id, mask);
	});

	connect(&procontroller, &org::wiimotedev::procontroller::buttonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(common::enums::Device::ProController, id, mask);
	});

	connect(&wiimote, &org::wiimotedev::wiimote::buttonDataChanged, [this](uint id, qulonglong mask) {
		buttonDataChanged(common::enums::Device::Wiimote, id, mask);
	});

	connect(&classic, &org::wiimotedev::classic::stickDataChanged, [this](uint id, int lx, int ly, int rx, int ry) {
		stickDataChanged(common::enums::Device::Classic, id, lx, ly, rx, ry);
	});

	connect(&nunchuk, &org::wiimotedev::nunchuk::stickDataChanged, [this](uint id, int x, int y) {
		stickDataChanged(common::enums::Device::Nunchuk, id, x, y, 0, 0);
	});

	connect(&procontroller, &org::wiimotedev::procontroller::stickDataChanged, [this](uint id, int lx, int ly, int rx, int ry) {
		stickDataChanged(common::enums::Device::ProController, id, lx, ly, rx, ry);
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

void Profile::connected(Device, uint id) {
}

void Profile::disconnected(Device, uint id) {
}

void Profile::buttonDataChanged(Device source, uint id, qulonglong mask) {
	gamepad_iterator(source, id, [&mask](const auto &gamepad) { gamepad->input(mask); });
}

void Profile::stickDataChanged(Device source, uint id, int lx, int ly, int rx, int ry) {
	gamepad_iterator(source, id, [source, lx, ly, rx, ry](const auto &gamepad) {
		if (source == Device::Classic || source == Device::ProController) {
			gamepad->input(Stick::LStick, lx, ly);
			gamepad->input(Stick::RStick, lx, ly);
		}

		if (source == Device::Nunchuk) {
			gamepad->input(Stick::Stick, lx, ly);
		}
	});
}

void Profile::accelerometerDataChanged(Device, uint id, int x, int y, int z) {
}

void Profile::gyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ) {
}

void Profile::infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
}

void Profile::gamepad_iterator(const Device type, const quint32 id, std::function<void(const std::unique_ptr<IGamepad> &)> &&function) {
	for (const auto &gamepad : m_gamepads) {
		if (gamepad->id() == id && gamepad->type() == type && gamepad->isCreated()) {
			function(gamepad);
		}
	}
}

bool Profile::setup(const Device type, const std::string &name, quint32 id) {
	auto device = GamepadFactory::create(type, name, id);
	auto isValid = GamepadFactory::configure(device);

	if (isValid)
		m_gamepads.emplace_back(std::move(device));

	return isValid;
}
