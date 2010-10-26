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

QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteGetRumbleStatus(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetRumbleStatus", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteSetLedStatus(quint32 id, quint32 status) {
  QList<QVariant> argumentList;
  argumentList << id;
  argumentList << status;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteSetLedStatus", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteSetRumbleStatus(quint32 id, bool status) {
  QList<QVariant> argumentList;
  argumentList << id << status;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteSetRumbleStatus", argumentList);
}

QDBusReply < QList <uint> > DBusDeviceEventsInterface::dbusGetWiimoteList() {
  return call(QDBus::Block, "dbusGetWiimoteList");
}

QDBusReply < QStringList> DBusDeviceEventsInterface::dbusGetUnregistredWiimoteList() {
  return call(QDBus::Block, "dbusGetUnregistredWiimoteList");
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusIsClassicConnected(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusIsClassicConnected", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusIsNunchukConnected(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusIsNunchukConnected", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusIsWiimoteConnected(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusIsWiimoteConnected", argumentList);
}

QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteAverageLatency(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteAverageLatency", argumentList);
}

QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteCurrentLatency(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteCurrentLatency", argumentList);
}

QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteGetStatus(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetStatus", argumentList);
}

QDBusReply < QString> DBusDeviceEventsInterface::dbusWiimoteGetMacAddress(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetMacAddress", argumentList);
}

QDBusReply < quint8> DBusDeviceEventsInterface::dbusWiimoteGetLedStatus(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetLedStatus", argumentList);
}

QDBusReply < quint32> DBusDeviceEventsInterface::dbusWiimoteGetBatteryLife(quint32 id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetBatteryLife", argumentList);
}
