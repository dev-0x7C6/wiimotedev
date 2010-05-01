/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "dbus_support.h"

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

QDBusArgument& operator<<(QDBusArgument& argument, const deviceinfo& info)
{
    argument.beginStructure();
    argument << info.id << QString().fromStdString(info.addr) << info.registred << info.nunchuk << info.classic;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, deviceinfo& info)
{
    QString data;
    argument.beginStructure();
    argument >> info.id >> data;
    info.addr = data.toStdString();
    argument >> info.registred >> info.nunchuk >> info.classic;
    argument.endStructure();
    return argument;
}

bool DBusServiceAdaptor::dbusReloadSequenceList()
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusReloadSequenceList", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
    return value;
}

QList < int> DBusDeviceEventsAdaptor::dbusGetDeviceList()
{
    QList < int> list;
    QMetaObject::invokeMethod(parent(), "dbusGetDeviceList", Qt::DirectConnection, Q_RETURN_ARG(QList < int>, list));
    return list;
}

QStringList DBusDeviceEventsAdaptor::dbusUnregistredWiiremoteList()
{
    QStringList list;
    QMetaObject::invokeMethod(parent(), "dbusUnregistredWiiremoteList", Qt::DirectConnection, Q_RETURN_ARG(QStringList, list));
    return list;
}

quint8 DBusDeviceEventsAdaptor::dbusWiimoteGetLedStatus(quint32 id)
{
    quint8 value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
    return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteGetRumbleStatus(quint32 id)
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id));
    return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteSetLedStatus(quint32 id, quint8 status)
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(quint8, status));
    return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection,  Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(bool, status));
    return value;
}

quint32 DBusDeviceEventsAdaptor::dbusWiimoteGetCurrentLatency(quint32 id)
{
    quint32 latency;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
    return latency;
}

quint32 DBusDeviceEventsAdaptor::dbusWiimoteGetAverageLatency(quint32 id)
{
    quint32 latency;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
    return latency;
}
