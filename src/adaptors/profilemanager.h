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

#ifndef ADAPTORS_PROFILEMANAGER_H
#define ADAPTORS_PROFILEMANAGER_H

#include <QDBusAbstractAdaptor>
#include <QDBusAbstractInterface>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>

#include <QList>

#include "adaptors/adaptors.h"

class DBusProfileManagerAdaptor :public QDBusAbstractAdaptor
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.profileManager")
  Q_CLASSINFO("D-Bus Introspection", ""
    "  <interface name=\"org.wiimotedev.profileManager\">\n"
    "    <method name=\"loadProfile\">\n"
    "      <arg name=\"file\" type=\"s\" direction=\"in\"/>\n"
    "      <arg type=\"y\" direction=\"out\"/>\n"
    "    </method>\n"
    "    <method name=\"unloadProfile\" />\n"
    "    <method name=\"currentProfile\">\n"
    "      <arg type=\"s\" direction=\"out\"/>\n"
    "    </method>\n"
    "  </interface>\n"
    "")
public:
  DBusProfileManagerAdaptor (QObject *parent);

public Q_SLOTS:
  QString currentProfile();
  bool loadProfile(QString);
  void unloadProfile();
};


class DBusProfileManagerAdaptorWrapper :public QObject
{
  Q_OBJECT
private:
  bool registred;

public:
  DBusProfileManagerAdaptorWrapper (QObject *parent, QDBusConnection &connection);
  inline bool isRegistred() { return registred; }

public Q_SLOTS:
  QString currentProfile();
  bool loadProfile(QString);
  void unloadProfile();

};

#endif // ADAPTORS_PROFILEMANAGER_H
