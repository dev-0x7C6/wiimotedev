/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#ifndef ADAPTORS_UINPUTSERVICE_H
#define ADAPTORS_UINPUTSERVICE_H

#include "adaptors.h"

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
  Q_SLOT bool isWiimotedevServiceAvailable();

};

class DBusServiceAdaptorWrapper :public QObject
{
  Q_OBJECT
private:
  bool registred;

public:
  DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection connection);

  inline bool isRegistred() { return registred; }
  Q_SLOT bool isWiimotedevServiceAvailable();

};

inline DBusServiceAdaptor::DBusServiceAdaptor(QObject *parent): QDBusAbstractAdaptor(parent)
{
  setAutoRelaySignals(true);
}

inline bool DBusServiceAdaptor::isWiimotedevServiceAvailable() {
  bool value;
  QMetaObject::invokeMethod(parent(), "isWiimotedevServiceAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
  return value;
}

inline DBusServiceAdaptorWrapper::DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection connection) : QObject(parent)
{
  new DBusServiceAdaptor(this);
  registred = connection.registerObject("/service", this);
}

inline bool DBusServiceAdaptorWrapper::isWiimotedevServiceAvailable() {
  bool value;
  QMetaObject::invokeMethod(parent(), "isWiimotedevServiceAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
  return value;
}


#endif // ADAPTORS_UINPUTSERVICE_H
