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

#include "deviceevents.h"

DBusDeviceEventsAdaptor::DBusDeviceEventsAdaptor (QObject *parent) : QDBusAbstractAdaptor(parent)
{
  QWIIMOTEDEV_REGISTER_META_TYPES;
  setAutoRelaySignals(true);
}

QList < uint> DBusDeviceEventsAdaptor::dbusGetWiimoteList()
{
  QList < uint> list;
  QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QList < uint>, list));
  return list;
}

bool DBusDeviceEventsAdaptor::dbusIsClassicConnected(quint32 id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsClassicConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(quint32, id));
  return connected;
}

bool DBusDeviceEventsAdaptor::dbusIsNunchukConnected(quint32 id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsNunchukConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(quint32, id));
  return connected;
}

bool DBusDeviceEventsAdaptor::dbusIsWiimoteConnected(quint32 id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsWiimoteConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(quint32, id));
  return connected;
}

QStringList DBusDeviceEventsAdaptor::dbusGetUnregistredWiimoteList()
{
  QStringList list;
  QMetaObject::invokeMethod(parent(), "dbusGetUnregistredWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QStringList, list));
  return list;
}

quint8 DBusDeviceEventsAdaptor::dbusWiimoteGetStatus(quint32 id)
{
  quint8 value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
  return value;
}

quint8 DBusDeviceEventsAdaptor::dbusWiimoteGetLedStatus(quint32 id)
{
  quint8 value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
  return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteGetRumbleStatus(quint32 id)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id));
  return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteSetLedStatus(quint32 id, quint32 status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(quint32, status));
  return value;
}

bool DBusDeviceEventsAdaptor::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection,  Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(bool, status));
  return value;
}

quint32 DBusDeviceEventsAdaptor::dbusWiimoteGetCurrentLatency(quint32 id)
{
  quint32 latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
  return latency;
}

quint32 DBusDeviceEventsAdaptor::dbusWiimoteGetBatteryLife(quint32 id)
{
  quint32 life;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetBatteryLife", Qt::DirectConnection, Q_RETURN_ARG(quint32, life), Q_ARG(quint32, id));
  return life;
}

quint32 DBusDeviceEventsAdaptor::dbusWiimoteGetAverageLatency(quint32 id)
{
  quint32 latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
  return latency;
}

QString DBusDeviceEventsAdaptor::dbusWiimoteGetMacAddress(quint32 id)
{
  QString macAddress;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetMacAddress", Qt::DirectConnection, Q_RETURN_ARG(QString, macAddress), Q_ARG(quint32, id));
  return macAddress;
}

DBusDeviceEventsAdaptorWrapper::DBusDeviceEventsAdaptorWrapper(QObject *parent, QDBusConnection &connection) : QObject(parent)
{
  new DBusDeviceEventsAdaptor(this);
  registred = connection.registerObject(WIIMOTEDEV_DBUS_OBJECT_EVENTS, this);
}


quint32 DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetCurrentLatency(quint32 id){
  quint32 latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
  return latency;
}

quint32 DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetAverageLatency(quint32 id) {
  quint32 latency;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
  return latency;
}

QString DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetMacAddress(quint32 id)
{
  QString macAddress;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetMacAddress", Qt::DirectConnection, Q_RETURN_ARG(QString, macAddress), Q_ARG(quint32, id));
  return macAddress;
}

bool DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetRumbleStatus(quint32 id){
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id));
  return value;
}

quint32 DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetBatteryLife(quint32 id)
{
  quint32 life;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetBatteryLife", Qt::DirectConnection, Q_RETURN_ARG(quint32, life), Q_ARG(quint32, id));
  return life;
}

bool DBusDeviceEventsAdaptorWrapper::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection,  Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(bool, status));
  return value;
}

quint8 DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetLedStatus(quint32 id){
  quint8 value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
  return value;
}

bool DBusDeviceEventsAdaptorWrapper::dbusWiimoteSetLedStatus(quint32 id, quint32 status)
{
  bool value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(quint32, status));
  return value;
}

quint8 DBusDeviceEventsAdaptorWrapper::dbusWiimoteGetStatus(quint32 id){
  quint8 value;
  QMetaObject::invokeMethod(parent(), "dbusWiimoteGetStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
  return value;
}

QList < uint> DBusDeviceEventsAdaptorWrapper::dbusGetWiimoteList()
{
  QList < uint> list;
  QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QList < uint>, list));
  return list;
}

QStringList DBusDeviceEventsAdaptorWrapper::dbusGetUnregistredWiimoteList() {
  QStringList list;
  QMetaObject::invokeMethod(parent(), "dbusGetUnregistredWiimoteList", Qt::DirectConnection, Q_RETURN_ARG(QStringList, list));
  return list;
}

bool DBusDeviceEventsAdaptorWrapper::dbusIsClassicConnected(quint32 id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsClassicConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(quint32, id));
  return connected;
}

bool DBusDeviceEventsAdaptorWrapper::dbusIsNunchukConnected(quint32 id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsNunchukConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(quint32, id));
  return connected;
}

bool DBusDeviceEventsAdaptorWrapper::dbusIsWiimoteConnected(quint32 id)
{
  bool connected;
  QMetaObject::invokeMethod(parent(), "dbusIsWiimoteConnected", Qt::DirectConnection, Q_RETURN_ARG(bool, connected), Q_ARG(quint32, id));
  return connected;
}
