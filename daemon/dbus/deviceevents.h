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

#ifndef ADAPTORS_DEVICEEVENTS_H
#define ADAPTORS_DEVICEEVENTS_H

#include <wiimotedev/consts.h>

#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusMetaType>

#ifndef WIIMOTEDEV_MARSHALL
#include <QDBusArgument>
#endif

#ifndef WIIMOTEDEV_META_TYPES
#define WIIMOTEDEV_META_TYPES

Q_DECLARE_METATYPE(QList<irpoint>)
Q_DECLARE_METATYPE(QList<accdata>)
Q_DECLARE_METATYPE(QList<stickdata>)
Q_DECLARE_METATYPE(QList<uint>)

Q_DECLARE_METATYPE(irpoint)
Q_DECLARE_METATYPE(accdata)
Q_DECLARE_METATYPE(stickdata)

#endif

#ifndef WIIMOTEDEV_MARSHALL
#define WIIMOTEDEV_MARSHALL

QDBusArgument &operator<<(QDBusArgument &argument, const irpoint &point);
const QDBusArgument &operator>>(const QDBusArgument &argument, irpoint &point);
QDBusArgument &operator<<(QDBusArgument &argument, const accdata &acc);
const QDBusArgument &operator>>(const QDBusArgument &argument, accdata &acc);
QDBusArgument &operator<<(QDBusArgument &argument, const stickdata &stick);
const QDBusArgument &operator>>(const QDBusArgument &argument, stickdata &stick);

inline QDBusArgument &operator<<(QDBusArgument &argument, const irpoint &point) {
	argument.beginStructure();
	argument << point.size << point.x << point.y;
	argument.endStructure();
	return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, irpoint &point) {
	argument.beginStructure();
	argument >> point.size >> point.x >> point.y;
	argument.endStructure();
	return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const accdata &acc) {
	argument.beginStructure();
	argument << acc.x << acc.y << acc.z << acc.pitch << acc.roll;
	argument.endStructure();
	return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, accdata &acc) {
	argument.beginStructure();
	argument >> acc.x >> acc.y >> acc.z >> acc.pitch >> acc.roll;
	argument.endStructure();
	return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const stickdata &stick) {
	argument.beginStructure();
	argument << stick.x << stick.y;
	argument.endStructure();
	return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, stickdata &stick) {
	argument.beginStructure();
	argument >> stick.x >> stick.y;
	argument.endStructure();
	return argument;
}

#endif

class WiimotedevDBusEvents : public QDBusAbstractAdaptor {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.deviceEvents")
	Q_CLASSINFO("D-Bus Introspection", ""
									   " <interface name=\"org.wiimotedev.deviceEvents\">\n"
									   "    <method name=\"dbusGetWiimoteList\">\n"
									   "      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
									   "      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusIsClassicConnected\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusIsNunchukConnected\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusIsWiimoteConnected\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusNunchukGetAccelerometrCalibration\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
									   "      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetAccelerometrCalibration\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
									   "      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetAverageLatency\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetBatteryLife\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetCurrentLatency\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetMacAddress\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"s\" name=\"mac\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetLedStatus\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"led\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteGetRumbleStatus\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"b\" name=\"rumble\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteSetLedStatus\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"status\" />\n"
									   "      <arg direction=\"out\" type=\"b\" name=\"led\" />\n"
									   "    </method>\n"
									   "    <method name=\"dbusWiimoteSetRumbleStatus\">\n"
									   "      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"in\" type=\"b\" name=\"status\" />\n"
									   "      <arg direction=\"out\" type=\"b\" name=\"rumble\" />\n"
									   "    </method>\n"
									   "    <signal name=\"dbusClassicControllerButtons\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusClassicControllerLStick\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusClassicControllerPlugged\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusClassicControllerRStick\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusClassicControllerUnplugged\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusNunchukAcc\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"(yyydd)\" name=\"acc\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusNunchukButtons\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusNunchukPlugged\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusNunchukStick\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusNunchukUnplugged\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteAcc\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"(yyydd)\" name=\"acc\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteBatteryLife\">\n"
									   "     <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "     <arg direction=\"out\" type=\"y\" name=\"life\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteButtons\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteConnected\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteDisconnected\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteGeneralButtons\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteInfrared\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"a(nqq)\" name=\"table\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteLedStatusChanged\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteRumbleStatusChanged\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"value\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusWiimoteStatus\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusVirtualCursorPosition\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "      <arg direction=\"out\" type=\"d\" name=\"x\" />\n"
									   "      <arg direction=\"out\" type=\"d\" name=\"y\" />\n"
									   "      <arg direction=\"out\" type=\"d\" name=\"length\" />\n"
									   "      <arg direction=\"out\" type=\"d\" name=\"angle\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusVirtualCursorFound\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "    <signal name=\"dbusVirtualCursorLost\">\n"
									   "      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
									   "    </signal>\n"
									   "  </interface>")

public:
	WiimotedevDBusEvents(QObject *parent);

public Q_SLOTS:
	bool dbusIsClassicConnected(uint32_t id);
	bool dbusIsNunchukConnected(uint32_t id);
	bool dbusIsWiimoteConnected(uint32_t id);

	QList<uint> dbusNunchukGetAccelerometrCalibration(uint32_t id);
	QList<uint> dbusWiimoteGetAccelerometrCalibration(uint32_t id);

	uint32_t dbusWiimoteGetAverageLatency(uint32_t id);
	uint32_t dbusWiimoteGetBatteryLife(uint32_t id);
	uint32_t dbusWiimoteGetCurrentLatency(uint32_t id);
	QString dbusWiimoteGetMacAddress(uint32_t id);

	uint8_t dbusWiimoteGetLedStatus(uint32_t id);

	QList<uint> dbusGetWiimoteList();
	bool dbusWiimoteGetRumbleStatus(uint32_t id);
	bool dbusWiimoteSetLedStatus(uint32_t id, uint32_t status);
	bool dbusWiimoteSetRumbleStatus(uint32_t id, bool status);

Q_SIGNALS:
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

	void dbusVirtualCursorPosition(uint32_t, double, double, double, double);
	void dbusVirtualCursorFound(uint32_t);
	void dbusVirtualCursorLost(uint32_t);

	void dbusWiimoteAcc(uint32_t, struct accdata);
	void dbusWiimoteBatteryLife(uint32_t, uint8_t);
	void dbusWiimoteButtons(uint32_t, uint64_t);
	void dbusWiimoteConnected(uint32_t);
	void dbusWiimoteDisconnected(uint32_t);
	void dbusWiimoteInfrared(uint32_t, QList<struct irpoint>);
	void dbusWiimoteLedStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteRumbleStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteStatus(uint32_t, uint8_t);

	void dbusNunchukPlugged(uint32_t);
	void dbusNunchukUnplugged(uint32_t);

	void dbusNunchukAcc(uint32_t, struct accdata);
	void dbusNunchukButtons(uint32_t, uint64_t);
	void dbusNunchukStick(uint32_t, struct stickdata);

	void dbusClassicControllerPlugged(uint32_t);
	void dbusClassicControllerUnplugged(uint32_t);

	void dbusClassicControllerButtons(uint32_t, uint64_t);
	void dbusClassicControllerLStick(uint32_t, struct stickdata);
	void dbusClassicControllerRStick(uint32_t, struct stickdata);
};

class WiimotedevDBusEventsWrapper : public QObject {
	Q_OBJECT
private:
	bool registred;

public:
	WiimotedevDBusEventsWrapper(QObject *parent, QDBusConnection &connection);

	inline bool isRegistred() {
		return registred;
	}

public Q_SLOTS:
	bool dbusIsClassicConnected(uint32_t id);
	bool dbusIsNunchukConnected(uint32_t id);
	bool dbusIsWiimoteConnected(uint32_t id);

	QList<uint> dbusNunchukGetAccelerometrCalibration(uint32_t id);
	QList<uint> dbusWiimoteGetAccelerometrCalibration(uint32_t id);

	uint32_t dbusWiimoteGetAverageLatency(uint32_t id);
	uint32_t dbusWiimoteGetBatteryLife(uint32_t id);
	uint32_t dbusWiimoteGetCurrentLatency(uint32_t id);
	QString dbusWiimoteGetMacAddress(uint32_t id);

	uint8_t dbusWiimoteGetLedStatus(uint32_t id);

	QList<uint> dbusGetWiimoteList();
	QStringList dbusGetUnregistredWiimoteList();

	bool dbusWiimoteGetRumbleStatus(uint32_t id);
	bool dbusWiimoteSetLedStatus(uint32_t id, uint32_t status);
	bool dbusWiimoteSetRumbleStatus(uint32_t id, bool status);

Q_SIGNALS:
	void dbusReportUnregistredWiimote(QString);
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

	void dbusVirtualCursorPosition(uint32_t, double, double, double, double);
	void dbusVirtualCursorFound(uint32_t);
	void dbusVirtualCursorLost(uint32_t);

	void dbusWiimoteAcc(uint32_t, struct accdata);
	void dbusWiimoteBatteryLife(uint32_t, uint8_t);
	void dbusWiimoteButtons(uint32_t, uint64_t);
	void dbusWiimoteConnected(uint32_t);
	void dbusWiimoteDisconnected(uint32_t);
	void dbusWiimoteInfrared(uint32_t, QList<struct irpoint>);
	void dbusWiimoteLedStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteRumbleStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteStatus(uint32_t, uint8_t);

	void dbusNunchukPlugged(uint32_t);
	void dbusNunchukUnplugged(uint32_t);

	void dbusNunchukAcc(uint32_t, struct accdata);
	void dbusNunchukButtons(uint32_t, uint64_t);
	void dbusNunchukStick(uint32_t, struct stickdata);

	void dbusClassicControllerPlugged(uint32_t);
	void dbusClassicControllerUnplugged(uint32_t);

	void dbusClassicControllerButtons(uint32_t, uint64_t);
	void dbusClassicControllerLStick(uint32_t, struct stickdata);
	void dbusClassicControllerRStick(uint32_t, struct stickdata);
};

inline WiimotedevDBusEvents::WiimotedevDBusEvents(QObject *parent)
		: QDBusAbstractAdaptor(parent) {
	qRegisterMetaType<uint8_t>("uint8_t");
	qRegisterMetaType<uint16_t>("uint16_t");
	qRegisterMetaType<uint32_t>("uint32_t");
	qRegisterMetaType<uint64_t>("uint64_t");
	qRegisterMetaType<int8_t>("int8_t");
	qRegisterMetaType<int16_t>("int16_t");
	qRegisterMetaType<int32_t>("int32_t");
	qRegisterMetaType<int64_t>("int64_t");
	qRegisterMetaType<QList<struct irpoint>>("QList< irpoint>");
	qRegisterMetaType<QList<struct accdata>>("QList< accdata>");
	qRegisterMetaType<QList<struct stickdata>>("QList< stickdata>");
	qRegisterMetaType<struct irpoint>("irpoint");
	qRegisterMetaType<struct accdata>("accdata");
	qRegisterMetaType<struct stickdata>("stickdata");
	qDBusRegisterMetaType<QList<struct irpoint>>();
	qDBusRegisterMetaType<QList<struct accdata>>();
	qDBusRegisterMetaType<QList<struct stickdata>>();
	qDBusRegisterMetaType<struct irpoint>();
	qDBusRegisterMetaType<struct accdata>();
	qDBusRegisterMetaType<struct stickdata>();
	setAutoRelaySignals(true);
}

inline QList<uint> WiimotedevDBusEvents::dbusGetWiimoteList() {
	QList<uint> list;
	QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QList<uint>, list));
	return list;
}

inline bool WiimotedevDBusEvents::dbusIsClassicConnected(uint32_t id) {
	bool connected;
	QMetaObject::invokeMethod(parent(), "dbusIsClassicConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint32_t, id));
	return connected;
}

inline bool WiimotedevDBusEvents::dbusIsNunchukConnected(uint32_t id) {
	bool connected;
	QMetaObject::invokeMethod(parent(), "dbusIsNunchukConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint32_t, id));
	return connected;
}

inline bool WiimotedevDBusEvents::dbusIsWiimoteConnected(uint32_t id) {
	bool connected;
	QMetaObject::invokeMethod(parent(), "dbusIsWiimoteConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint32_t, id));
	return connected;
}

inline uint8_t WiimotedevDBusEvents::dbusWiimoteGetLedStatus(uint32_t id) {
	uint8_t value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(uint8_t, value), Q_ARG(uint32_t, id));
	return value;
}

inline bool WiimotedevDBusEvents::dbusWiimoteGetRumbleStatus(uint32_t id) {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint32_t, id));
	return value;
}

inline bool WiimotedevDBusEvents::dbusWiimoteSetLedStatus(uint32_t id, uint32_t status) {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint32_t, id), Q_ARG(uint32_t, status));
	return value;
}

inline bool WiimotedevDBusEvents::dbusWiimoteSetRumbleStatus(uint32_t id, bool status) {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint32_t, id), Q_ARG(bool, status));
	return value;
}

inline uint32_t WiimotedevDBusEvents::dbusWiimoteGetCurrentLatency(uint32_t id) {
	uint32_t latency;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::DirectConnection, Q_RETURN_ARG(uint32_t, latency), Q_ARG(uint32_t, id));
	return latency;
}

inline uint32_t WiimotedevDBusEvents::dbusWiimoteGetBatteryLife(uint32_t id) {
	uint32_t life;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetBatteryLife", Qt::DirectConnection, Q_RETURN_ARG(uint32_t, life), Q_ARG(uint32_t, id));
	return life;
}

inline QList<uint> WiimotedevDBusEvents::dbusNunchukGetAccelerometrCalibration(uint32_t id) {
	QList<uint> value;
	QMetaObject::invokeMethod(parent(), "dbusNunchukGetAccelerometrCalibration", Qt::DirectConnection, Q_RETURN_ARG(QList<uint>, value), Q_ARG(uint32_t, id));
	return value;
}

inline QList<uint> WiimotedevDBusEvents::dbusWiimoteGetAccelerometrCalibration(uint32_t id) {
	QList<uint> value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAccelerometrCalibration", Qt::DirectConnection, Q_RETURN_ARG(QList<uint>, value), Q_ARG(uint32_t, id));
	return value;
}

inline uint32_t WiimotedevDBusEvents::dbusWiimoteGetAverageLatency(uint32_t id) {
	uint32_t latency;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::DirectConnection, Q_RETURN_ARG(uint32_t, latency), Q_ARG(uint32_t, id));
	return latency;
}

inline QString WiimotedevDBusEvents::dbusWiimoteGetMacAddress(uint32_t id) {
	QString macAddress;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetMacAddress", Qt::DirectConnection, Q_RETURN_ARG(QString, macAddress), Q_ARG(uint32_t, id));
	return macAddress;
}

inline WiimotedevDBusEventsWrapper::WiimotedevDBusEventsWrapper(QObject *parent, QDBusConnection &connection)
		: QObject(parent) {
	new WiimotedevDBusEvents(this);
	registred = connection.registerObject(WIIMOTEDEV_DBUS_OBJECT_EVENTS, this);
}

inline uint32_t WiimotedevDBusEventsWrapper::dbusWiimoteGetCurrentLatency(uint32_t id) {
	uint32_t latency;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::DirectConnection, Q_RETURN_ARG(uint32_t, latency), Q_ARG(uint32_t, id));
	return latency;
}

inline uint32_t WiimotedevDBusEventsWrapper::dbusWiimoteGetAverageLatency(uint32_t id) {
	uint32_t latency;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::DirectConnection, Q_RETURN_ARG(uint32_t, latency), Q_ARG(uint32_t, id));
	return latency;
}

inline QList<uint> WiimotedevDBusEventsWrapper::dbusWiimoteGetAccelerometrCalibration(uint32_t id) {
	QList<uint> value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAccelerometrCalibration", Qt::DirectConnection, Q_RETURN_ARG(QList<uint>, value), Q_ARG(uint32_t, id));
	return value;
}

inline QList<uint> WiimotedevDBusEventsWrapper::dbusNunchukGetAccelerometrCalibration(uint32_t id) {
	QList<uint> value;
	QMetaObject::invokeMethod(parent(), "dbusNunchukGetAccelerometrCalibration", Qt::DirectConnection, Q_RETURN_ARG(QList<uint>, value), Q_ARG(uint32_t, id));
	return value;
}

inline QString WiimotedevDBusEventsWrapper::dbusWiimoteGetMacAddress(uint32_t id) {
	QString macAddress;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetMacAddress", Qt::DirectConnection, Q_RETURN_ARG(QString, macAddress), Q_ARG(uint32_t, id));
	return macAddress;
}

inline bool WiimotedevDBusEventsWrapper::dbusWiimoteGetRumbleStatus(uint32_t id) {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint32_t, id));
	return value;
}

inline uint32_t WiimotedevDBusEventsWrapper::dbusWiimoteGetBatteryLife(uint32_t id) {
	uint32_t life;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetBatteryLife", Qt::DirectConnection, Q_RETURN_ARG(uint32_t, life), Q_ARG(uint32_t, id));
	return life;
}

inline bool WiimotedevDBusEventsWrapper::dbusWiimoteSetRumbleStatus(uint32_t id, bool status) {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint32_t, id), Q_ARG(bool, status));

	if (value)
		emit dbusWiimoteRumbleStatusChanged(id, status);

	return value;
}

inline uint8_t WiimotedevDBusEventsWrapper::dbusWiimoteGetLedStatus(uint32_t id) {
	uint8_t value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(uint8_t, value), Q_ARG(uint32_t, id));
	return value;
}

inline bool WiimotedevDBusEventsWrapper::dbusWiimoteSetLedStatus(uint32_t id, uint32_t status) {
	bool value;
	QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint32_t, id), Q_ARG(uint32_t, status));

	if (value)
		emit dbusWiimoteLedStatusChanged(id, status);

	return value;
}

inline QList<uint> WiimotedevDBusEventsWrapper::dbusGetWiimoteList() {
	QList<uint> list;
	QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QList<uint>, list));
	return list;
}

inline QStringList WiimotedevDBusEventsWrapper::dbusGetUnregistredWiimoteList() {
	QStringList list;
	QMetaObject::invokeMethod(parent(), "dbusGetUnregistredWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QStringList, list));
	return list;
}

inline bool WiimotedevDBusEventsWrapper::dbusIsClassicConnected(uint32_t id) {
	bool connected;
	QMetaObject::invokeMethod(parent(), "dbusIsClassicConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint32_t, id));
	return connected;
}

inline bool WiimotedevDBusEventsWrapper::dbusIsNunchukConnected(uint32_t id) {
	bool connected;
	QMetaObject::invokeMethod(parent(), "dbusIsNunchukConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint32_t, id));
	return connected;
}

inline bool WiimotedevDBusEventsWrapper::dbusIsWiimoteConnected(uint32_t id) {
	bool connected;
	QMetaObject::invokeMethod(parent(), "dbusIsWiimoteConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint32_t, id));
	return connected;
}

#endif // ADAPTORS_DEVICEEVENTS_H
