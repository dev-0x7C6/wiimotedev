/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2014  Bartłomiej Burdukiewicz                               *
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

#ifndef ADAPTORS_PROFILEMANAGER_H
#define ADAPTORS_PROFILEMANAGER_H

#include <wiimotedev/consts.h>

#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusMetaType>

#ifndef WIIMOTEDEV_MARSHALL
#include <QDBusArgument>
#endif

#ifndef WIIMOTEDEV_META_TYPES
#define WIIMOTEDEV_META_TYPES

Q_DECLARE_METATYPE(QList <irpoint>)
Q_DECLARE_METATYPE(QList <accdata>)
Q_DECLARE_METATYPE(QList <stickdata>)
Q_DECLARE_METATYPE(QList <uint>)

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

class DBusProfileManagerAdaptor : public QDBusAbstractAdaptor {
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
  DBusProfileManagerAdaptor(QObject *parent);

public Q_SLOTS:
  QString currentProfile();
  bool loadProfile(QString);
  void unloadProfile();
};


class DBusProfileManagerAdaptorWrapper : public QObject {
  Q_OBJECT
private:
  bool registred;

public:
  DBusProfileManagerAdaptorWrapper(QObject *parent, QDBusConnection connection);
  inline bool isRegistred() {
    return registred;
  }

public Q_SLOTS:
  QString currentProfile();
  bool loadProfile(QString);
  void unloadProfile();
};

inline DBusProfileManagerAdaptor::DBusProfileManagerAdaptor(QObject *parent): QDBusAbstractAdaptor(parent) {
  setAutoRelaySignals(true);
}

inline QString DBusProfileManagerAdaptor::currentProfile() {
  QString value;
  QMetaObject::invokeMethod(parent(), "currentProfile", Qt::DirectConnection, Q_RETURN_ARG(QString, value));
  return value;
}

inline bool DBusProfileManagerAdaptor::loadProfile(QString file) {
  bool value;
  QMetaObject::invokeMethod(parent(), "loadProfile", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(QString, file));
  return value;
}

inline void DBusProfileManagerAdaptor::unloadProfile() {
  QMetaObject::invokeMethod(parent(), "unloadProfile", Qt::DirectConnection);
}

inline DBusProfileManagerAdaptorWrapper::DBusProfileManagerAdaptorWrapper(QObject *parent, QDBusConnection connection): QObject(parent) {
  new DBusProfileManagerAdaptor(this);
  registred = connection.registerObject("/profileManager", this);
}

inline QString DBusProfileManagerAdaptorWrapper::currentProfile() {
  QString value;
  QMetaObject::invokeMethod(parent(), "currentProfile", Qt::DirectConnection, Q_RETURN_ARG(QString, value));
  return value;
}

inline bool DBusProfileManagerAdaptorWrapper::loadProfile(QString file) {
  bool value;
  QMetaObject::invokeMethod(parent(), "loadProfile", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(QString, file));
  return value;
}

inline void DBusProfileManagerAdaptorWrapper::unloadProfile() {
  QMetaObject::invokeMethod(parent(), "unloadProfile", Qt::DirectConnection);
}


#endif // ADAPTORS_PROFILEMANAGER_H
