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

#ifndef DBUS_SUPPORT_H
#define DBUS_SUPPORT_H

// Defaults
#include <QDBusAbstractAdaptor>
#include <QDBusArgument>
#include <QDBusConnection>

#include <QList>
#include "../include/wiimotedev.h"

// Meta-types
#include <QDBusMetaType>
#include <QMetaType>

#define QWIIMOTEDEV_REGISTER_META_TYPES                                      \
    qRegisterMetaType< QList< struct irpoint> >("QList< irpoint>");          \
    qRegisterMetaType< QList< struct accdata> >("QList< accdata>");          \
    qRegisterMetaType< QList< struct stickdata> >("QList< stickdata>");      \
    qRegisterMetaType< QList< struct deviceinfo> >("QList< deviceinfo>");    \
    qRegisterMetaType< struct irpoint>("irpoint");                           \
    qRegisterMetaType< struct accdata>("accdata");                           \
    qRegisterMetaType< struct stickdata>("stickdata");                       \
    qRegisterMetaType< struct deviceinfo>("deviceinfo");                     \
    qDBusRegisterMetaType< QList < struct irpoint> >();                      \
    qDBusRegisterMetaType< QList < struct accdata> >();                      \
    qDBusRegisterMetaType< QList < struct stickdata> >();                    \
    qDBusRegisterMetaType< QList < struct deviceinfo> >();                   \
    qDBusRegisterMetaType< struct irpoint>();                                \
    qDBusRegisterMetaType< struct accdata>();                                \
    qDBusRegisterMetaType< struct stickdata>();                              \
    qDBusRegisterMetaType< struct deviceinfo>();

#ifndef QWIIMOTEDEV_META_TYPES
#define QWIIMOTEDEV_META_TYPES

    Q_DECLARE_METATYPE(QList < deviceinfo>)
    Q_DECLARE_METATYPE(QList < irpoint>)
    Q_DECLARE_METATYPE(QList < accdata>)
    Q_DECLARE_METATYPE(QList < stickdata>)
    Q_DECLARE_METATYPE(QList < uint>)

    Q_DECLARE_METATYPE(deviceinfo)
    Q_DECLARE_METATYPE(irpoint)
    Q_DECLARE_METATYPE(accdata)
    Q_DECLARE_METATYPE(stickdata)

#endif

#ifndef QWIIMOTEDEV_MARSHALL
#define QWIIMOTEDEV_MARSHALL

    QDBusArgument& operator<<(QDBusArgument& argument, const irpoint& point);
    const QDBusArgument& operator>>(const QDBusArgument& argument, irpoint& point);
    QDBusArgument& operator<<(QDBusArgument& argument, const accdata& acc);
    const QDBusArgument& operator>>(const QDBusArgument& argument, accdata& acc);
    QDBusArgument& operator<<(QDBusArgument& argument, const stickdata& stick);
    const QDBusArgument& operator>>(const QDBusArgument& argument, stickdata& stick);
    QDBusArgument& operator<<(QDBusArgument& argument, const deviceinfo& info);
    const QDBusArgument& operator>>(const QDBusArgument& argument, deviceinfo& info);
    QDBusArgument& operator<<(QDBusArgument& argument, const QList < quint32>& list);
    const QDBusArgument& operator>>(const QDBusArgument& argument, QList < quint32>& list);

#endif

#endif // DBUS_SUPPORT_H
