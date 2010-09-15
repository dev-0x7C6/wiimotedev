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

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

// Defaults
#include <QDBusAbstractAdaptor>
#include <QDBusAbstractInterface>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>

#include <QList>
#include <wiimotedev.h>

// Meta-types
#include <QDBusMetaType>
#include <QMetaType>

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
  DBusProfileManagerAdaptor (QObject *parent): QDBusAbstractAdaptor(parent) {
    QWIIMOTEDEV_REGISTER_META_TYPES;
    setAutoRelaySignals(true);
  }

public slots:
  inline bool loadProfile(QString file){
    bool value;
    QMetaObject::invokeMethod(parent(), "loadProfile", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(QString, file));
    return value;
  }

  inline void unloadProfile() {
    QMetaObject::invokeMethod(parent(), "unloadProfile", Qt::DirectConnection);
  }

  inline QString currentProfile(){
    QString value;
    QMetaObject::invokeMethod(parent(), "currentProfile", Qt::DirectConnection, Q_RETURN_ARG(QString, value));
    return value;
  }
};


class DBusProfileManagerAdaptorWrapper :public QObject
{
  Q_OBJECT
private:
  bool registred;

public:
  DBusProfileManagerAdaptorWrapper (QObject *parent, QDBusConnection &connection): QObject(parent) {
    new DBusProfileManagerAdaptor(this);
    registred = connection.registerObject("/profileManager", this);
  }

  inline bool isRegistred() { return registred; }

public slots:
  inline bool loadProfile(QString file){
    bool value;
    QMetaObject::invokeMethod(parent(), "loadProfile", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(QString, file));
    return value;
  }

  inline void unloadProfile() {
    QMetaObject::invokeMethod(parent(), "unloadProfile", Qt::DirectConnection);
  }

  inline QString currentProfile(){
    QString value;
    QMetaObject::invokeMethod(parent(), "currentProfile", Qt::DirectConnection, Q_RETURN_ARG(QString, value));
    return value;
  }

};

#endif // PROFILEMANAGER_H
