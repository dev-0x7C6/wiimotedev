/**********************************************************************************
 * Wiimotedev interface classes                                                   *
 * Copyright (C) 2010  Bartłomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
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

#ifndef WIIMOTEDEV_INTERFACE_H
#define WIIMOTEDEV_INTERFACE_H

#include <wiimotedev/consts.h>

#include <QDBusAbstractAdaptor>
#include <QDBusAbstractInterface>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>

#include <QList>

#include <QDBusMetaType>
#include <QMetaType>

#ifndef QWIIMOTEDEV_REGISTER_META_TYPES
#define QWIIMOTEDEV_REGISTER_META_TYPES                                      \
    qRegisterMetaType< QList< struct irpoint> >("QList< irpoint>");          \
    qRegisterMetaType< QList< struct accdata> >("QList< accdata>");          \
    qRegisterMetaType< QList< struct stickdata> >("QList< stickdata>");      \
    qRegisterMetaType< struct irpoint>("irpoint");                           \
    qRegisterMetaType< struct accdata>("accdata");                           \
    qRegisterMetaType< struct stickdata>("stickdata");                       \
    qDBusRegisterMetaType< QList < struct irpoint> >();                      \
    qDBusRegisterMetaType< QList < struct accdata> >();                      \
    qDBusRegisterMetaType< QList < struct stickdata> >();                    \
    qDBusRegisterMetaType< struct irpoint>();                                \
    qDBusRegisterMetaType< struct accdata>();                                \
    qDBusRegisterMetaType< struct stickdata>();                              
#endif

#ifndef QWIIMOTEDEV_META_TYPES
#define QWIIMOTEDEV_META_TYPES

    Q_DECLARE_METATYPE(QList < irpoint>);
    Q_DECLARE_METATYPE(QList < accdata>);
    Q_DECLARE_METATYPE(QList < stickdata>);
    Q_DECLARE_METATYPE(QList < uint>);

    Q_DECLARE_METATYPE(irpoint);
    Q_DECLARE_METATYPE(accdata);
    Q_DECLARE_METATYPE(stickdata);

#endif

#ifndef QWIIMOTEDEV_MARSHALL
#define QWIIMOTEDEV_MARSHALL

QDBusArgument& operator<<(QDBusArgument& argument, const irpoint& point)
{
    argument.beginStructure();
    argument << point.size << point.x << point.y;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, irpoint& point)
{
    argument.beginStructure();
    argument >> point.size >> point.x >> point.y;
    argument.endStructure();
    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const accdata& acc)
{
    argument.beginStructure();
    argument << acc.x << acc.y << acc.z << acc.pitch << acc.roll;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, accdata& acc)
{
    argument.beginStructure();
    argument >> acc.x >> acc.y >> acc.z >> acc.pitch >> acc.roll;
    argument.endStructure();
    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const stickdata& stick)
{
    argument.beginStructure();
    argument << stick.x << stick.y;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, stickdata& stick)
{
    argument.beginStructure();
    argument >> stick.x >> stick.y;
    argument.endStructure();
    return argument;
}

#endif

class DBusDeviceEventsInterface :public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName() { return WIIMOTEDEV_DBUS_EVENTS_IFACE; }

public:
    DBusDeviceEventsInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0)
        :QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent) { QWIIMOTEDEV_REGISTER_META_TYPES }

public Q_SLOTS:
    QDBusReply < QList <uint> > dbusGetWiimoteList();
    QDBusReply < QStringList> dbusGetUnregistredWiimoteList();
    QDBusReply < bool> dbusIsClassicConnected(quint32 id);
    QDBusReply < bool> dbusIsNunchukConnected(quint32 id);
    QDBusReply < bool> dbusIsWiimoteConnected(quint32 id);

    QDBusReply < quint8> dbusWiimoteAverageLatency(quint32 id);
    QDBusReply < quint8> dbusWiimoteCurrentLatency(quint32 id);
    QDBusReply < quint8> dbusWiimoteGetStatus(quint32 id);
    QDBusReply < quint32> dbusWiimoteGetBatteryLife(quint32 id);
    QDBusReply < bool> dbusWiimoteGetRumbleStatus(quint32 id);
    QDBusReply < quint8> dbusWiimoteGetLedStatus(quint32 id);
    QDBusReply < bool> dbusWiimoteSetLedStatus(quint32 id, quint32 status);
    QDBusReply < bool> dbusWiimoteSetRumbleStatus(quint32 id, bool status);


Q_SIGNALS:
    void dbusWiimoteGeneralButtons(quint32, quint64);

    void dbusWiimoteConnected(quint32);
    void dbusWiimoteDisconnected(quint32);
    void dbusWiimoteBatteryLife(quint32, quint8);
    void dbusWiimoteButtons(quint32, quint64);
    void dbusWiimoteStatus(quint32, quint8);
    void dbusWiimoteInfrared(quint32, QList< struct irpoint>);
    void dbusWiimoteAcc(quint32, struct accdata);

    void dbusNunchukPlugged(quint32);
    void dbusNunchukUnplugged(quint32);
    void dbusNunchukButtons(quint32, quint64);
    void dbusNunchukStick(quint32, struct stickdata);
    void dbusNunchukAcc(quint32, struct accdata);

    void dbusClassicControllerPlugged(quint32);
    void dbusClassicControllerUnplugged(quint32);
    void dbusClassicControllerButtons(quint32, quint64);
    void dbusClassicControllerLStick(quint32, struct stickdata);
    void dbusClassicControllerRStick(quint32, struct stickdata);

    void dbusReportUnregisteredWiimote(quint32);
};

inline QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteGetRumbleStatus(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteGetRumbleStatus", argumentList);
}

inline QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteSetLedStatus(quint32 id, quint32 status) {
    QList<QVariant> argumentList;
    argumentList << id;
    argumentList << status;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteSetLedStatus", argumentList);
}

inline QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteSetRumbleStatus(quint32 id, bool status) {
    QList<QVariant> argumentList;
    argumentList << id << status;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteSetRumbleStatus", argumentList);
}

inline QDBusReply < QList <uint> > DBusDeviceEventsInterface::dbusGetWiimoteList() {
    return call(QDBus::Block, "dbusGetWiimoteList");
}

inline QDBusReply < QStringList> DBusDeviceEventsInterface::dbusGetUnregistredWiimoteList() {
    return call(QDBus::Block, "dbusGetUnregistredWiimoteList");
}

inline QDBusReply < bool> DBusDeviceEventsInterface::dbusIsClassicConnected(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusIsClassicConnected", argumentList);
}

inline QDBusReply < bool> DBusDeviceEventsInterface::dbusIsNunchukConnected(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusIsNunchukConnected", argumentList);
}

inline QDBusReply < bool> DBusDeviceEventsInterface::dbusIsWiimoteConnected(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusIsWiimoteConnected", argumentList);
}

inline QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteAverageLatency(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteAverageLatency", argumentList);
}

inline QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteCurrentLatency(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteCurrentLatency", argumentList);
}

inline QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteGetStatus(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteGetStatus", argumentList);
}


inline QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteGetLedStatus(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteGetLedStatus", argumentList);
}

inline QDBusReply < quint32> DBusDeviceEventsInterface::dbusWiimoteGetBatteryLife(quint32 id) {
    QList<QVariant> argumentList;
    argumentList << id;
    return callWithArgumentList(QDBus::Block, "dbusWiimoteGetBatteryLife", argumentList);
}

#endif WIIMOTEDEV_INTERFACE_H
