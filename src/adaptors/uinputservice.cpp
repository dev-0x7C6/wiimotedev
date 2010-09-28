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

#include "adaptors/uinputservice.h"

DBusServiceAdaptor::DBusServiceAdaptor(QObject *parent): QDBusAbstractAdaptor(parent)
{
  setAutoRelaySignals(true);
}

bool DBusServiceAdaptor::isWiimotedevServiceAvailable() {
  bool value;
  QMetaObject::invokeMethod(parent(), "isWiimotedevServiceAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
  return value;
}

DBusServiceAdaptorWrapper::DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection connection) : QObject(parent)
{
  new DBusServiceAdaptor(this);
  registred = connection.registerObject("/service", this);
}

bool DBusServiceAdaptorWrapper::isWiimotedevServiceAvailable() {
  bool value;
  QMetaObject::invokeMethod(parent(), "isWiimotedevServiceAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
  return value;
}