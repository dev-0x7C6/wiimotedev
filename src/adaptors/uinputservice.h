/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
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

#ifndef ADAPTORS_UINPUTSERVICE_H
#define ADAPTORS_UINPUTSERVICE_H

#include <QDBusAbstractAdaptor>
#include <QDBusAbstractInterface>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>

#include <QList>

#include <QDBusMetaType>
#include <QMetaType>

#include "adaptors/adaptors.h"

class DBusServiceAdaptor :public QDBusAbstractAdaptor
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.service")
  Q_CLASSINFO("D-Bus Introspection", ""
   "  <interface name=\"org.wiimotedev.service\">\n"
   "    <method name=\"isWiimotedevServiceAvailable\">\n"
   "      <arg type=\"y\" direction=\"out\"/>\n"
   "    </method>\n"
   "  </interface>\n"
   "")
public:
  DBusServiceAdaptor (QObject *parent = 0);

public slots:
  bool isWiimotedevServiceAvailable();

};

class DBusServiceAdaptorWrapper :public QObject
{
  Q_OBJECT
private:
  bool registred;

public:
  DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection connection);

  inline bool isRegistred() { return registred; }

public slots:
  bool isWiimotedevServiceAvailable();

};

#endif // ADAPTORS_UINPUTSERVICE_H
