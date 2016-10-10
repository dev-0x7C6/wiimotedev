#include "profile.h"

#include <iostream>
#include <QSettings>
#include <QDBusConnection>

#include "factories/gamepad-factory.h"
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

#include <iostream>

void Profile::pressureDataChanged(uint id, int tl, int tr, int bl, int br) {
	gamepad_iterator(Device::BalanceBoard, id, [tl, tr, bl, br](const auto &gamepad) {
		double sum = tl + tr + bl + br + 1;

		if (sum < 200)
			return;

		double tsum = tl + tr + 1;
		double bsum = bl + br + 1;
		double lsum = tl + bl + 1;
		double rsum = tr + br + 1;

		auto p1 = std::min(lsum / rsum, 3.0);
		auto p2 = std::min(rsum / lsum, 3.0);
		auto p3 = std::min(lsum / rsum, 3.0);
		auto p4 = std::min(rsum / lsum, 3.0);
		auto x = 0;
		auto y = 0;

		x = (((p1 > p2) ? p1 : -p2) * (0xfff / 3.0)) + 0xfff;
		y = (((p3 > p4) ? p3 : -p4) * (0xfff / 3.0)) + 0xfff;
		gamepad->input(Stick::Stick, x, y);
	});
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
