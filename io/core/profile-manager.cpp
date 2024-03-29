#include "profile-manager.h"
#include "profilemanageradaptor.h"

#include <QDBusConnection>
#include <QFileInfo>

#include "functionals/profile.h"

#include "dbus/interfaces/balanceboard.h"
#include "dbus/interfaces/classic.h"
#include "dbus/interfaces/nunchuk.h"
#include "dbus/interfaces/procontroller.h"
#include "dbus/interfaces/virtualcursor.h"
#include "dbus/interfaces/wiimote.h"

using namespace io::core;
using namespace io::functional;
using namespace common::enums;

ProfileManager::ProfileManager(QObject *parent)
		: QObject(parent) {
	new ProfileManagerAdaptor(this);
	QDBusConnection connection = QDBusConnection::systemBus();
	connection.registerObject("/profileManager", this);
	connection.registerService("org.wiimotedev.io");

	auto balanceboard = new org::wiimotedev::balanceboard("org.wiimotedev.daemon", "/balanceboard", QDBusConnection::systemBus(), this);
	auto classic = new org::wiimotedev::classic("org.wiimotedev.daemon", "/classic", QDBusConnection::systemBus(), this);
	auto nunchuk = new org::wiimotedev::nunchuk("org.wiimotedev.daemon", "/nunchuk", QDBusConnection::systemBus(), this);
	auto procontroller = new org::wiimotedev::procontroller("org.wiimotedev.daemon", "/procontroller", QDBusConnection::systemBus(), this);
	auto wiimote = new org::wiimotedev::wiimote("org.wiimotedev.daemon", "/wiimote", QDBusConnection::systemBus(), this);
	//auto virtualcursor = new org::wiimotedev::virtualcursor("org.wiimotedev.daemon", "/virtualcursor", QDBusConnection::systemBus(), this);

	connect(balanceboard, &org::wiimotedev::balanceboard::connectionChanged, [this](uint id, bool status) {
		for (auto &&profile : m_profiles)
			if (status)
				profile->connected(device::balance_board, id);
			else
				profile->disconnected(device::balance_board, id);
	});

	connect(classic, &org::wiimotedev::classic::connectionChanged, [this](uint id, bool status) {
		for (auto &&profile : m_profiles)
			if (status)
				profile->connected(device::classic_controller, id);
			else
				profile->disconnected(device::classic_controller, id);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::connectionChanged, [this](uint id, bool status) {
		for (auto &&profile : m_profiles)
			if (status)
				profile->connected(device::nunchuk, id);
			else
				profile->disconnected(device::nunchuk, id);
	});

	connect(procontroller, &org::wiimotedev::procontroller::connectionChanged, [this](uint id, bool status) {
		for (const auto &profile : m_profiles)
			if (status)
				profile->connected(device::pro_controller, id);
			else
				profile->disconnected(device::pro_controller, id);
	});

	connect(wiimote, &org::wiimotedev::wiimote::connectionChanged, [this](uint id, bool status) {
		for (const auto &profile : m_profiles)
			if (status)
				profile->connected(device::wiimote, id);
			else
				profile->disconnected(device::wiimote, id);
	});

	connect(classic, &org::wiimotedev::classic::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(device::classic_controller, id, mask);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(device::nunchuk, id, mask);
	});

	connect(procontroller, &org::wiimotedev::procontroller::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(device::pro_controller, id, mask);
	});

	connect(wiimote, &org::wiimotedev::wiimote::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(device::wiimote, id, mask);
	});

	connect(classic, &org::wiimotedev::classic::stickDataChanged, [this](uint id, int lx, int ly, int rx, int ry) {
		for (const auto &profile : m_profiles)
			profile->stickDataChanged(device::classic_controller, id, lx, ly, rx, ry);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::stickDataChanged, [this](uint id, int x, int y) {
		for (const auto &profile : m_profiles)
			profile->stickDataChanged(device::nunchuk, id, x, y, 0, 0);
	});

	connect(procontroller, &org::wiimotedev::procontroller::stickDataChanged, [this](uint id, int lx, int ly, int rx, int ry) {
		for (const auto &profile : m_profiles)
			profile->stickDataChanged(device::pro_controller, id, lx, ly, rx, ry);
	});

	connect(wiimote, &org::wiimotedev::wiimote::infraredDataChanged, [this](uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
		for (const auto &profile : m_profiles)
			profile->infraredDataChanged(id, x1, y1, x2, y2, x3, y3, x4, y4);
	});

	connect(wiimote, &org::wiimotedev::wiimote::accelerometerDataChanged, [this](uint id, int x, int y, int z, int pitch, int roll) {
		for (const auto &profile : m_profiles)
			profile->accelerometerDataChanged(device::wiimote, id, x, y, z);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::accelerometerDataChanged, [this](uint id, int x, int y, int z, int pitch, int roll) {
		for (const auto &profile : m_profiles)
			profile->accelerometerDataChanged(device::nunchuk, id, x, y, z);
	});

	connect(wiimote, &org::wiimotedev::wiimote::gyroscopeDataChanged, [this](uint id, int x, int y, int z) {
		for (const auto &profile : m_profiles)
			profile->gyroscopeDataChanged(id, x, y, z);
	});

	connect(balanceboard, &org::wiimotedev::balanceboard::dataChanged, [this](uint id, int tl, int tr, int bl, int br) {
		for (const auto &profile : m_profiles)
			profile->pressureDataChanged(id, tl, tr, bl, br);
	});

	load("/test.ini");
}

QStringList ProfileManager::list() const {
	QStringList list;

	for (const auto &profile : m_profiles)
		list.append(QString::fromStdString(profile->configurationFilePath()));
	return list;
}

bool ProfileManager::load(const QString &filePath) {
	if (!QFile::exists(filePath))
		return false;

	m_profiles.emplace_back(std::make_unique<Profile>(filePath.toStdString()));
	emit loaded(filePath);
	return true;
}

bool ProfileManager::unload(const QString &filePath) {
	bool isValid = false;

	m_profiles.remove_if([&isValid, &filePath](const auto &profile) {
		if (profile->configurationFilePath() == filePath.toStdString()) {
			isValid = true;
			return true;
		}

		return false;
	});

	if (isValid)
		emit unloaded(filePath);

	return isValid;
}
