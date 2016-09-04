/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2015  Bartłomiej Burdukiewicz                               *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#include <QFile>
#include <QSettings>

#include <io/functionals/hash-compare.h>
#include "eiomanager/manager.h"
#include "eioinfrared/eioinfraredmouse.h"

extern QMap<QString, uint64_t> devicebuttons;
extern QMap<QString, uint> scancodes;

const QRegExp deviceEventRegExp(".*(\\[.*(\\d+)\\])");

UInputProfileManager::UInputProfileManager(QObject *parent)
		: QObject(parent)
		, dbusDeviceEventsIface(std::make_unique<WiimotedevDeviceEvents>(
			  WIIMOTEDEV_DBUS_SERVICE_NAME,
			  WIIMOTEDEV_DBUS_OBJECT_EVENTS,
			  QDBusConnection::systemBus(), this))
		, m_dbusProfileManager(std::make_unique<DBusProfileManagerAdaptorWrapper>(this, QDBusConnection::systemBus()))
		, m_dbusService(std::make_unique<DBusServiceAdaptorWrapper>(this, QDBusConnection::systemBus()))
		, m_dbusCustomJobs(std::make_unique<DBusCustomJobsAdaptorWrapper>(this, QDBusConnection::systemBus()))
		, m_eventDevice("Virtual mouse and keyboard", 0) {
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusWiimoteGeneralButtons, this, &UInputProfileManager::dbusWiimoteGeneralButtons, Qt::DirectConnection);
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusWiimoteButtons, this, &UInputProfileManager::dbusWiimoteButtons, Qt::DirectConnection);
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusNunchukButtons, this, &UInputProfileManager::dbusNunchukButtons, Qt::DirectConnection);
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusNunchukStick, this, &UInputProfileManager::dbusNunchukStick, Qt::DirectConnection);
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusClassicControllerButtons, this, &UInputProfileManager::dbusClassicControllerButtons, Qt::DirectConnection);
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusClassicControllerLStick, this, &UInputProfileManager::dbusClassicControllerLStick, Qt::DirectConnection);
	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusClassicControllerRStick, this, &UInputProfileManager::dbusClassicControllerRStick, Qt::DirectConnection);
	initializeCommandEvents();
	QDBusConnection::systemBus().registerService("org.wiimotedev.io");
	dbusWiimoteGeneralButtons(1, 0);
	m_eventDevice.open();
	m_eventDevice.configure();
	m_eventDevice.create();
}

QHash<uint32_t, uint64_t> UInputProfileManager::extractDeviceEvent(QString input) {
	QStringList list = input.remove(QRegExp("[ ]")).toLower().split('+');
	QHash<uint32_t, uint64_t> values;
	bool result = false;
	uint32_t index;
	uint64_t value;
	foreach (const QString &item, list) {
		deviceEventRegExp.exactMatch(item);
		index = deviceEventRegExp.cap(2).toUInt();
		value = devicebuttons.value(deviceEventRegExp.cap(0).remove(deviceEventRegExp.cap(1)), 0);
		values.insert(index, values.value(index, 0) | value);
		result = result || (value);
	}

	if (!result)
		return (QHash<uint32_t, uint64_t>());
	else
		return values;
}

QList<uint> UInputProfileManager::extractScancodes(QStringList list) {
	QList<uint> values;

	for (int i = 0; i < list.count(); ++i)
		if (scancodes.value(list.at(i), QString(list.at(i)).toUInt()))
			values << scancodes.value(list.at(i), QString(list.at(i)).toUInt());

	return values;
}

void UInputProfileManager::dbusWiimoteGeneralButtons(uint32_t id, uint64_t buttons) {
	if (lastWiiremoteButtons.value(id, -1) == buttons)
		return;

	lastWiiremoteButtons[id] = buttons;
	processCommandEvents();
}

bool UInputProfileManager::loadProfile(QString file) {
	if (!QFile::exists(file))
		return false;

	QSettings settings(file, QSettings::IniFormat);
	foreach (const QString &key, settings.childGroups()) {
		const QString &module = settings.value(key + "/module", QString()).toString();

		if (module.isEmpty())
			continue;

		module.toLower();

		if (module == QString::fromUtf8("joystick"))
			assignJoystickEvents(key, settings);
		else if (module == QString::fromUtf8("keyboard"))
			assignKeyboardEvents(key, settings);
		else if (module == QString::fromUtf8("infrared"))
			assignInfraredEvents(key, settings);
	}
	loadCommandEvents(settings);
	loadInfraredEvents(settings);
	return true;
}

bool UInputProfileManager::unloadProfile() {
	unloadCommandEvents();
	m_gamepads.clear();
	m_mouses.clear();
	m_keyboards.clear();
	return true;
}

UInputProfileManager::~UInputProfileManager() {
	unloadProfile();
}
