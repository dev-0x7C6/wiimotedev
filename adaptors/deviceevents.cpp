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

#include "deviceevents.h"

DBusDeviceEventsAdaptor::DBusDeviceEventsAdaptor (QObject *parent) : QDBusAbstractAdaptor(parent)
{
  QWIIMOTEDEV_REGISTER_META_TYPES;
  setAutoRelaySignals(true);
}

QList < uint> DBusDeviceEventsAdaptor::dbusGetWiimoteList()
{
  QList < uint> list;
  QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Qt::AutoConnection, Q_RETURN_ARG(QList < uint>, list));
  return list;
}

bool DBusDeviceEventsAdaptor::dbusIsClassicConnected(uint id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsClassicConnected", Qt::AutoConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint, id));
  return connected;
}

bool DBusDeviceEventsAdaptor::dbusIsNunchukConnected(uint id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsNunchukConnected", Qt::AutoConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint, id));
  return connected;
}

bool DBusDeviceEventsAdaptor::dbusIsWiimoteConnected(uint id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsWiimoteConnected", Qt::AutoConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint, id));
  return connected;
}

uint8 DBusDeviceEventsAdaptor::dbusWiimoteGetLedStatus(uint id)
{
  uint8 value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::AutoConnection, Q_RETURN_ARG(uint8, value), Q_ARG(uint, id));
  return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteGetRumbleStatus(uint id)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::AutoConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint, id));
  return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteSetLedStatus(uint id, uint status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::AutoConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint, id), Q_ARG(uint, status));
  return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteSetRumbleStatus(uint id, bool status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::AutoConnection,  Q_RETURN_ARG(bool, value), Q_ARG(uint, id), Q_ARG(bool, status));
  return value;
}

uint DBusDeviceEventsAdaptor::dbusWiimoteGetCurrentLatency(uint id)
{
  uint latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::AutoConnection, Q_RETURN_ARG(uint, latency), Q_ARG(uint, id));
  return latency;
}

uint DBusDeviceEventsAdaptor::dbusWiimoteGetBatteryLife(uint id)
{
  uint life;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetBatteryLife", Qt::AutoConnection, Q_RETURN_ARG(uint, life), Q_ARG(uint, id));
  return life;
}

QList < uint> DBusDeviceEventsAdaptor::dbusNunchukGetAccelerometrCalibration(uint id)
{
  QList < uint> value;
  QMetaObject::invokeMethod(parent(), "dbusNunchukGetAccelerometrCalibration", Qt::AutoConnection, Q_RETURN_ARG(QList < uint>, value), Q_ARG(uint, id));
  return value;
}

QList < uint> DBusDeviceEventsAdaptor::dbusWiimoteGetAccelerometrCalibration(uint id) {
  QList < uint> value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAccelerometrCalibration", Qt::AutoConnection, Q_RETURN_ARG(QList < uint>, value), Q_ARG(uint, id));
  return value;
}

uint DBusDeviceEventsAdaptor::dbusWiimoteGetAverageLatency(uint id)
{
  uint latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::AutoConnection, Q_RETURN_ARG(uint, latency), Q_ARG(uint, id));
  return latency;
}

QString DBusDeviceEventsAdaptor::dbusWiimoteGetMacAddress(uint id)
{
  QString macAddress;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetMacAddress", Qt::AutoConnection, Q_RETURN_ARG(QString, macAddress), Q_ARG(uint, id));
  return macAddress;
}

DBusDeviceEventsAdaptorWrapper::DBusDeviceEventsAdaptorWrapper(QObject *parent, QDBusConnection &connection) : QObject(parent)
{
  new DBusDeviceEventsAdaptor(this);
  registred = connection.registerObject(WIIMOTEDEV_DBUS_OBJECT_EVENTS, this);
}


uint DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetCurrentLatency(uint id){
  uint latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::AutoConnection, Q_RETURN_ARG(uint, latency), Q_ARG(uint, id));
  return latency;
}

uint DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetAverageLatency(uint id) {
  uint latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::AutoConnection, Q_RETURN_ARG(uint, latency), Q_ARG(uint, id));
  return latency;
}

QList < uint> DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetAccelerometrCalibration(uint id)
{
  QList < uint> value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAccelerometrCalibration", Qt::AutoConnection, Q_RETURN_ARG(QList < uint>, value), Q_ARG(uint, id));
  return value;
}

QList < uint> DBusDeviceEventsAdaptorWrapper::dbusNunchukGetAccelerometrCalibration(uint id)
{
  QList < uint> value;
  QMetaObject::invokeMethod(parent(), "dbusNunchukGetAccelerometrCalibration", Qt::AutoConnection, Q_RETURN_ARG(QList < uint>, value), Q_ARG(uint, id));
  return value;
}

QString DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetMacAddress(uint id)
{
  QString macAddress;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetMacAddress", Qt::AutoConnection, Q_RETURN_ARG(QString, macAddress), Q_ARG(uint, id));
  return macAddress;
}

bool DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetRumbleStatus(uint id){
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::AutoConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint, id));
  return value;
}

uint DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetBatteryLife(uint id)
{
  uint life;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetBatteryLife", Qt::AutoConnection, Q_RETURN_ARG(uint, life), Q_ARG(uint, id));
  return life;
}

bool DBusDeviceEventsAdaptorWrapper::dbusWiimoteSetRumbleStatus(uint id, bool status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::AutoConnection,  Q_RETURN_ARG(bool, value), Q_ARG(uint, id), Q_ARG(bool, status));

  if (value)
    emit dbusWiimoteRumbleStatusChanged(id, status);

  return value;
}

uint8 DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetLedStatus(uint id){
  uint8 value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::AutoConnection, Q_RETURN_ARG(uint8, value), Q_ARG(uint, id));
  return value;
}

bool DBusDeviceEventsAdaptorWrapper::dbusWiimoteSetLedStatus(uint id, uint status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::AutoConnection, Q_RETURN_ARG(bool, value), Q_ARG(uint, id), Q_ARG(uint, status));

  if (value)
    emit dbusWiimoteLedStatusChanged(id, status);

  return value;
}

QList < uint> DBusDeviceEventsAdaptorWrapper::dbusGetWiimoteList()
{
  QList < uint> list;
  QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Qt::AutoConnection, Q_RETURN_ARG(QList < uint>, list));
  return list;
}

QStringList DBusDeviceEventsAdaptorWrapper::dbusGetUnregistredWiimoteList() {
  QStringList list;
  QMetaObject::invokeMethod(parent(), "dbusGetUnregistredWiimoteList", Qt::AutoConnection, Q_RETURN_ARG(QStringList, list));
  return list;
}

bool DBusDeviceEventsAdaptorWrapper::dbusIsClassicConnected(uint id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsClassicConnected", Qt::AutoConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint, id));
  return connected;
}

bool DBusDeviceEventsAdaptorWrapper::dbusIsNunchukConnected(uint id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsNunchukConnected", Qt::AutoConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint, id));
  return connected;
}

bool DBusDeviceEventsAdaptorWrapper::dbusIsWiimoteConnected(uint id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsWiimoteConnected", Qt::AutoConnection, Q_RETURN_ARG(bool, connected), Q_ARG(uint, id));
  return connected;
}
