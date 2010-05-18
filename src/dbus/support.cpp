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

#include "support.h"

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

QDBusArgument& operator<<(QDBusArgument& argument, const QList < quint32>& list)
{
    argument.beginArray();
    for (register int i = 0; i < list.count(); ++i)
        argument << list.at(i);
    argument.endArray();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, QList < quint32>& list)
{
    argument.beginArray();
    list.clear();

    while (!argument.atEnd()){
        quint32 i;
        argument >> i;
        list << i;
    }

    argument.endArray();
    return argument;
}
