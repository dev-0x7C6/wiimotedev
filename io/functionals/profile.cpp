#include "profile.h"

#include <iostream>
#include <QSettings>
#include <QDBusConnection>

#include "factories/gamepad-factory.h"
#include "functionals/balance-board-processor.h"
#include "interfaces/igamepad.h"

using namespace common::enums;
using namespace io::factory;
using namespace io::interface;
using namespace io::functional;

Profile::Profile(const std::string &configurationFilePath)
		: QObject(nullptr)
		, IProfile(configurationFilePath)

{
	QSettings settings(QString::fromStdString(configurationFilePath), QSettings::IniFormat);
	for (const auto &group : settings.childGroups()) {
		if (settings.value(group + "/module", {}).toString() != "gamepad")
			continue;

		settings.beginGroup(group);
		auto assign = settings.value("assign", 1).toUInt();
		auto device = settings.value("device").toString().toLower().toStdString();
		auto name = settings.value("name").toString().toStdString();

		QJsonObject configuration;
		configuration["balanceboard.layout"] = settings.value("balanceboard.layout", "snowboard").toString();

		try {
			setup(IGamepad::fromString(device), name, assign, configuration);
		} catch (std::out_of_range &) {
			std::cout << "unknown gamepad module \"" << device << "\"" << std::endl;
		}

		settings.endGroup();
	}
}

Profile::~Profile() {
	m_gamepads.clear();
}

void Profile::connected(Device device, uint id) {
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
			gamepad->input(Stick::RStick, rx, ry);
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

void Profile::pressureDataChanged(uint id, int tl, int tr, int bl, int br) {
	gamepad_iterator(Device::BalanceBoard, id, [tl, tr, bl, br](const std::unique_ptr<interface::IGamepad> &gamepad) {
		BalanceBoardProcessor processor({tl, tr, bl, br}, wiimotedev::WIIMOTEDEV_STICK_MIN, wiimotedev::WIIMOTEDEV_STICK_MAX);

		if (!processor.isValid()) {
			gamepad->input(Stick::Stick, 0, 0);
			return;
		}

		if (gamepad->configuration().value("balanceboard.layout").toString() == "snowboard") {
			gamepad->input(Stick::Stick, processor.y() * -1, processor.x() * -1);
			return;
		}

		gamepad->input(Stick::Stick, processor.x() * -1, processor.y());
	});
}

void Profile::gamepad_iterator(const Device type, const quint32 id, std::function<void(const std::unique_ptr<IGamepad> &)> &&function) {
	for (const auto &gamepad : m_gamepads) {
		if (gamepad->id() == id && gamepad->type() == type && gamepad->isCreated()) {
			function(gamepad);
		}
	}
}

bool Profile::setup(const Device type, const std::string &name, quint32 id, const QJsonObject &json) {
	auto device = GamepadFactory::create(type, name, id);
	auto isValid = GamepadFactory::configure(device);

	if (isValid) {
		device->setConfiguration(json);
		m_gamepads.emplace_back(std::move(device));
	}

	return isValid;
}
