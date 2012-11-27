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

#include "daemonservice.h"

DBusServiceAdaptor::DBusServiceAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{
  QWIIMOTEDEV_REGISTER_META_TYPES;
  setAutoRelaySignals(true);
}

bool DBusServiceAdaptor::dbusReloadSequenceList()
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusReloadSequenceList", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
  return value;
}

DBusServiceAdaptorWrapper::DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection &connection) : QObject(parent)
{
  new DBusServiceAdaptor(this);
  registred = connection.registerObject(WIIMOTEDEV_DBUS_OBJECT_SERVICE, this);
}

bool DBusServiceAdaptorWrapper::dbusReloadSequenceList()
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusReloadSequenceList", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
  return value;
}
