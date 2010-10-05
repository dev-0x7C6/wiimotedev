/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
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

#ifndef INTERFACES_INTERFACES_H
#define INTERFACES_INTERFACES_H

#include <QDBusAbstractInterface>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>

#include <QList>

#include <QDBusMetaType>
#include <QMetaType>

#include "headers/consts.h"

#define QWIIMOTEDEV_REGISTER_META_TYPES                                    \
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

#ifndef QWIIMOTEDEV_META_TYPES
#define QWIIMOTEDEV_META_TYPES

  Q_DECLARE_METATYPE(QList < irpoint>)
  Q_DECLARE_METATYPE(QList < accdata>)
  Q_DECLARE_METATYPE(QList < stickdata>)
  Q_DECLARE_METATYPE(QList < uint>)

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


  inline QDBusArgument& operator<<(QDBusArgument& argument, const irpoint& point)
  {
    argument.beginStructure();
    argument << point.size << point.x << point.y;
    argument.endStructure();
    return argument;
  }

  inline const QDBusArgument& operator>>(const QDBusArgument& argument, irpoint& point)
  {
    argument.beginStructure();
    argument >> point.size >> point.x >> point.y;
    argument.endStructure();
    return argument;
  }

  inline QDBusArgument& operator<<(QDBusArgument& argument, const accdata& acc)
  {
    argument.beginStructure();
    argument << acc.x << acc.y << acc.z << acc.pitch << acc.roll;
    argument.endStructure();
    return argument;
  }

  inline const QDBusArgument& operator>>(const QDBusArgument& argument, accdata& acc)
  {
    argument.beginStructure();
    argument >> acc.x >> acc.y >> acc.z >> acc.pitch >> acc.roll;
    argument.endStructure();
    return argument;
  }

  inline QDBusArgument& operator<<(QDBusArgument& argument, const stickdata& stick)
  {
    argument.beginStructure();
    argument << stick.x << stick.y;
    argument.endStructure();
    return argument;
  }

  inline const QDBusArgument& operator>>(const QDBusArgument& argument, stickdata& stick)
  {
    argument.beginStructure();
    argument >> stick.x >> stick.y;
    argument.endStructure();
    return argument;
  }

#endif

#endif // INTERFACES_INTERFACES_H
