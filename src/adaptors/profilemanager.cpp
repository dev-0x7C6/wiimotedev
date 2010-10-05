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

#include "adaptors/profilemanager.h"

DBusProfileManagerAdaptor::DBusProfileManagerAdaptor(QObject *parent): QDBusAbstractAdaptor(parent) {
  setAutoRelaySignals(true);
}

QString DBusProfileManagerAdaptor::currentProfile() {
  QString value;
  QMetaObject::invokeMethod(parent(), "currentProfile", Qt::DirectConnection, Q_RETURN_ARG(QString, value));
  return value;
}

bool DBusProfileManagerAdaptor::loadProfile(QString file) {
  bool value;
  QMetaObject::invokeMethod(parent(), "loadProfile", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(QString, file));
  return value;
}

void DBusProfileManagerAdaptor::unloadProfile() {
  QMetaObject::invokeMethod(parent(), "unloadProfile", Qt::DirectConnection);
}

DBusProfileManagerAdaptorWrapper::DBusProfileManagerAdaptorWrapper (QObject *parent, QDBusConnection connection): QObject(parent) {
  new DBusProfileManagerAdaptor(this);
  registred = connection.registerObject("/profileManager", this);
}

QString DBusProfileManagerAdaptorWrapper::currentProfile() {
  QString value;
  QMetaObject::invokeMethod(parent(), "currentProfile", Qt::DirectConnection, Q_RETURN_ARG(QString, value));
  return value;
}

bool DBusProfileManagerAdaptorWrapper::loadProfile(QString file) {
  bool value;
  QMetaObject::invokeMethod(parent(), "loadProfile", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(QString, file));
  return value;
}

void DBusProfileManagerAdaptorWrapper::unloadProfile() {
  QMetaObject::invokeMethod(parent(), "unloadProfile", Qt::DirectConnection);
}
