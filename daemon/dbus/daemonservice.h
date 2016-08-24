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

#ifndef ADAPTORS_DAEMONSERVICE_H
#define ADAPTORS_DAEMONSERVICE_H

#include <wiimotedev/consts.h>

#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusMetaType>

class DBusServiceAdaptor : public QDBusAbstractAdaptor {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.service")
	Q_CLASSINFO("D-Bus Introspection", ""
									   "<interface name=\"org.wiimotedev.service\">\n"
									   "    <method name=\"dbusReloadSequenceList\">\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"status\"/>\n"
									   "    </method>\n"
									   "  </interface>")

public:
	DBusServiceAdaptor(QObject *parent);
	Q_SLOT bool dbusReloadSequenceList();
};

class DBusServiceAdaptorWrapper : public QObject {
	Q_OBJECT
private:
	bool registred;

public:
	DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection &connection);
	inline bool isRegistred() {
		return registred;
	}
	Q_SLOT bool dbusReloadSequenceList();
};

inline DBusServiceAdaptor::DBusServiceAdaptor(QObject *parent)
		: QDBusAbstractAdaptor(parent) {
	setAutoRelaySignals(true);
}

inline bool DBusServiceAdaptor::dbusReloadSequenceList() {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusReloadSequenceList", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
	return value;
}

inline DBusServiceAdaptorWrapper::DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection &connection)
		: QObject(parent) {
	new DBusServiceAdaptor(this);
	registred = connection.registerObject(WIIMOTEDEV_DBUS_OBJECT_SERVICE, this);
}

inline bool DBusServiceAdaptorWrapper::dbusReloadSequenceList() {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusReloadSequenceList", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
	return value;
}

#endif // ADAPTORS_DAEMONSERVICE_H
