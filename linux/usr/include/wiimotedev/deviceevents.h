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
	QDBusReply<bool> dbusIsClassicConnected(uint32_t id);
	QDBusReply<bool> dbusIsNunchukConnected(uint32_t id);
	QDBusReply<bool> dbusIsWiimoteConnected(uint32_t id);

	QDBusReply<uint> dbusWiimoteGetAverageLatency(uint32_t id);
	QDBusReply<uint> dbusWiimoteGetCurrentLatency(uint32_t id);
	QDBusReply<uint8_t> dbusWiimoteGetStatus(uint32_t id);
	QDBusReply<uint> dbusWiimoteGetBatteryLife(uint32_t id);
	QDBusReply<QString> dbusWiimoteGetMacAddress(uint32_t id);
	QDBusReply<bool> dbusWiimoteGetRumbleStatus(uint32_t id);
	QDBusReply<uint8_t> dbusWiimoteGetLedStatus(uint32_t id);
	QDBusReply<bool> dbusWiimoteSetLedStatus(uint32_t id, uint32_t status);
	QDBusReply<bool> dbusWiimoteSetRumbleStatus(uint32_t id, bool status);

Q_SIGNALS:
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

	void dbusVirtualCursorPosition(uint32_t, double, double, double, double);
	void dbusVirtualCursorFound(uint32_t);
	void dbusVirtualCursorLost(uint32_t);

	void dbusWiimoteConnected(uint32_t);
	void dbusWiimoteDisconnected(uint32_t);
	void dbusWiimoteBatteryLife(uint32_t, uint8_t);
	void dbusWiimoteButtons(uint32_t, uint64_t);
	void dbusWiimoteLedStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteRumbleStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteStatus(uint32_t, uint8_t);
	void dbusWiimoteInfrared(uint32_t, const QList<irpoint> &);
	void dbusWiimoteAcc(uint32_t, const accdata &);

	void dbusNunchukPlugged(uint32_t);
	void dbusNunchukUnplugged(uint32_t);
	void dbusNunchukButtons(uint32_t, uint64_t);
	void dbusNunchukStick(uint32_t, const stickdata &);
	void dbusNunchukAcc(uint32_t, const accdata &);

	void dbusClassicControllerPlugged(uint32_t);
	void dbusClassicControllerUnplugged(uint32_t);
	void dbusClassicControllerButtons(uint32_t, uint64_t);
	void dbusClassicControllerLStick(uint32_t, const stickdata &);
	void dbusClassicControllerRStick(uint32_t, const stickdata &);
};

inline WiimotedevDeviceEvents::WiimotedevDeviceEvents(const QString &service, const QString &path,
	const QDBusConnection &connection, QObject *parent)
		: QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent) {
	qRegisterMetaType<uint8_t>("uint8");
	qRegisterMetaType<uint16_t>("uint16_t");
	qRegisterMetaType<uint32_t>("uint32_t");
	qRegisterMetaType<uint64_t>("uint64");
	qRegisterMetaType<int8_t>("int8");
	qRegisterMetaType<int16_t>("int16");
	qRegisterMetaType<int32_t>("int32_t");
	qRegisterMetaType<int64_t>("int64");
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

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusWiimoteGetRumbleStatus(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetRumbleStatus", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusWiimoteSetLedStatus(uint32_t id, uint32_t status) {
	QList<QVariant> argumentList;
	argumentList << id;
	argumentList << status;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteSetLedStatus", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusWiimoteSetRumbleStatus(uint32_t id, bool status) {
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

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusIsClassicConnected(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusIsClassicConnected", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusIsNunchukConnected(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusIsNunchukConnected", argumentList);
}

inline QDBusReply<bool> WiimotedevDeviceEvents::dbusIsWiimoteConnected(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusIsWiimoteConnected", argumentList);
}

inline QDBusReply<uint> WiimotedevDeviceEvents::dbusWiimoteGetAverageLatency(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetAverageLatency", argumentList);
}

inline QDBusReply<uint> WiimotedevDeviceEvents::dbusWiimoteGetCurrentLatency(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetCurrentLatency", argumentList);
}

inline QDBusReply<uint8_t> WiimotedevDeviceEvents::dbusWiimoteGetStatus(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetStatus", argumentList);
}

inline QDBusReply<QString> WiimotedevDeviceEvents::dbusWiimoteGetMacAddress(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetMacAddress", argumentList);
}

inline QDBusReply<uint8_t> WiimotedevDeviceEvents::dbusWiimoteGetLedStatus(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetLedStatus", argumentList);
}

inline QDBusReply<uint> WiimotedevDeviceEvents::dbusWiimoteGetBatteryLife(uint32_t id) {
	QList<QVariant> argumentList;
	argumentList << id;
	return callWithArgumentList(QDBus::Block, "dbusWiimoteGetBatteryLife", argumentList);
}

#endif // DEVICEEVENTS_INTERFACE_H
