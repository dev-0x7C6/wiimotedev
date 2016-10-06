#include "profile-manager.h"
#include "profile-manageradaptor.h"

#include <QDBusConnection>
#include <QFileInfo>

#include "functionals/profile.h"

#include "dbus/interfaces/balanceboard.h"
#include "dbus/interfaces/classic.h"
#include "dbus/interfaces/nunchuk.h"
#include "dbus/interfaces/procontroller.h"
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

	connect(balanceboard, &org::wiimotedev::balanceboard::connected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->connected(Device::BalanceBoard, id);
	});

	connect(classic, &org::wiimotedev::classic::connected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->connected(Device::Classic, id);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::connected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->connected(Device::Nunchuk, id);
	});

	connect(procontroller, &org::wiimotedev::procontroller::connected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->connected(Device::ProController, id);
	});

	connect(wiimote, &org::wiimotedev::wiimote::connected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->connected(Device::Wiimote, id);
	});

	connect(balanceboard, &org::wiimotedev::balanceboard::disconnected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->disconnected(Device::BalanceBoard, id);
	});

	connect(classic, &org::wiimotedev::classic::disconnected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->disconnected(Device::Classic, id);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::disconnected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->disconnected(Device::Nunchuk, id);
	});

	connect(procontroller, &org::wiimotedev::procontroller::disconnected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->disconnected(Device::ProController, id);
	});

	connect(wiimote, &org::wiimotedev::wiimote::disconnected, [this](uint id) {
		for (const auto &profile : m_profiles)
			profile->disconnected(Device::Wiimote, id);
	});

	connect(classic, &org::wiimotedev::classic::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(Device::Classic, id, mask);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(Device::Nunchuk, id, mask);
	});

	connect(procontroller, &org::wiimotedev::procontroller::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(Device::ProController, id, mask);
	});

	connect(wiimote, &org::wiimotedev::wiimote::buttonDataChanged, [this](uint id, qulonglong mask) {
		for (const auto &profile : m_profiles)
			profile->buttonDataChanged(Device::Wiimote, id, mask);
	});

	connect(classic, &org::wiimotedev::classic::stickDataChanged, [this](uint id, int lx, int ly, int rx, int ry) {
		for (const auto &profile : m_profiles)
			profile->stickDataChanged(Device::Classic, id, lx, ly, rx, ry);
	});

	connect(nunchuk, &org::wiimotedev::nunchuk::stickDataChanged, [this](uint id, int x, int y) {
		for (const auto &profile : m_profiles)
			profile->stickDataChanged(Device::Nunchuk, id, x, y, 0, 0);
	});

	connect(procontroller, &org::wiimotedev::procontroller::stickDataChanged, [this](uint id, int lx, int ly, int rx, int ry) {
		for (const auto &profile : m_profiles)
			profile->stickDataChanged(Device::ProController, id, lx, ly, rx, ry);
	});
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
