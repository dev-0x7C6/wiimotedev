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

#ifndef DEVICEEVENTS_INTERFACE_H
#define DEVICEEVENTS_INTERFACE_H

#include <wiimotedev/consts.h>

#include <QDBusAbstractInterface>
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

class WiimotedevDeviceEvents : public QDBusAbstractInterface {
	Q_OBJECT
public:
	static inline const char *staticInterfaceName() {
		return WIIMOTEDEV_DBUS_IFACE_EVENTS;
	}

public:
	WiimotedevDeviceEvents(
		const QString &service = WIIMOTEDEV_DBUS_SERVICE_NAME,
		const QString &path = WIIMOTEDEV_DBUS_OBJECT_EVENTS,
		const QDBusConnection &connection = QDBusConnection::systemBus(),
		QObject *parent = 0);

public Q_SLOTS:
	QDBusReply<QList<uint>> dbusGetWiimoteList();
	QDBusReply<QStringList> dbusGetUnregistredWiimoteList();
	QDBusReply<bool> dbusIsClassicConnected(quint32 id);
	QDBusReply<bool> dbusIsNunchukConnected(quint32 id);
	QDBusReply<bool> dbusIsWiimoteConnected(quint32 id);

	QDBusReply<uint> dbusWiimoteGetAverageLatency(quint32 id);
	QDBusReply<uint> dbusWiimoteGetCurrentLatency(quint32 id);
	QDBusReply<quint8> dbusWiimoteGetStatus(quint32 id);
	QDBusReply<uint> dbusWiimoteGetBatteryLife(quint32 id);
	QDBusReply<QString> dbusWiimoteGetMacAddress(quint32 id);
	QDBusReply<bool> dbusWiimoteGetRumbleStatus(quint32 id);
	QDBusReply<quint8> dbusWiimoteGetLedStatus(quint32 id);
	QDBusReply<bool> dbusWiimoteSetLedStatus(quint32 id, quint32 status);
	QDBusReply<bool> dbusWiimoteSetRumbleStatus(quint32 id, bool status);

Q_SIGNALS:
	void dbusWiimoteGeneralButtons(quint32, quint64);

	void dbusVirtualCursorPosition(quint32, double, double, double, double);
	void dbusVirtualCursorFound(quint32);
	void dbusVirtualCursorLost(quint32);

	void dbusWiimoteConnected(quint32);
	void dbusWiimoteDisconnected(quint32);
	void dbusWiimoteBatteryLife(quint32, quint8);
	void dbusWiimoteButtons(quint32, quint64);
	void dbusWiimoteLedStatusChanged(quint32, quint8);
	void dbusWiimoteRumbleStatusChanged(quint32, quint8);
	void dbusWiimoteStatus(quint32, quint8);
	void dbusWiimoteInfrared(quint32, const QList<irpoint> &);
	void dbusWiimoteAcc(quint32, const accdata &);

	void dbusNunchukPlugged(quint32);
	void dbusNunchukUnplugged(quint32);
	void dbusNunchukButtons(quint32, quint64);
	void dbusNunchukStick(quint32, const stickdata &);
	void dbusNunchukAcc(quint32, const accdata &);

	void dbusClassicControllerPlugged(quint32);
	void dbusClassicControllerUnplugged(quint32);
	void dbusClassicControllerButtons(quint32, quint64);
	void dbusClassicControllerLStick(quint32, const stickdata &);
	void dbusClassicControllerRStick(quint32, const stickdata &);
};

inline WiimotedevDeviceEvents::WiimotedevDeviceEvents(const QString &service, const QString &path,
	const QDBusConnection &connection, QObject *parent)
		: QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent) {
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
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusWiimoteGetRumbleStatus(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetRumbleStatus", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusWiimoteSetLedStatus(quint32 id, quint32 status) {
	QList<QVariant> argumentList;
	argumentList << id;
	argumentList << status;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteSetLedStatus", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusWiimoteSetRumbleStatus(quint32 id, bool status) {
	QList<QVariant> argumentList;
	argumentList << id << status;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteSetRumbleStatus", argumentList);
}

inline QDBusReply<QList<uint>> WiimotedevDeviceEvents::dbusGetWiimoteList() {
	return call(QDBus::Block, "dbusGetWiimoteList");
}

inline QDBusReply<QStringList> WiimotedevDeviceEvents::dbusGetUnregistredWiimoteList() {
	return call(QDBus::Block, "dbusGetUnregistredWiimoteList");
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusIsClassicConnected(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusIsClassicConnected", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusIsNunchukConnected(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusIsNunchukConnected", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusIsWiimoteConnected(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusIsWiimoteConnected", argumentList);
}

inline QDBusReply<uint> WiimotedevDeviceEvents::dbusWiimoteGetAverageLatency(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetAverageLatency", argumentList);
}

inline QDBusReply<uint> WiimotedevDeviceEvents::dbusWiimoteGetCurrentLatency(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetCurrentLatency", argumentList);
}

inline QDBusReply<quint8> WiimotedevDeviceEvents::dbusWiimoteGetStatus(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetStatus", argumentList);
}

inline QDBusReply<QString> WiimotedevDeviceEvents::dbusWiimoteGetMacAddress(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetMacAddress", argumentList);
}

inline QDBusReply<quint8> WiimotedevDeviceEvents::dbusWiimoteGetLedStatus(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetLedStatus", argumentList);
}

inline QDBusReply<uint> WiimotedevDeviceEvents::dbusWiimoteGetBatteryLife(quint32 id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetBatteryLife", argumentList);
}

#endif // DEVICEEVENTS_INTERFACE_H
