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

QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteGetRumbleStatus(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetRumbleStatus", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteSetLedStatus(uint id, uint status) {
  QList<QVariant> argumentList;
  argumentList << id;
  argumentList << status;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteSetLedStatus", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusWiimoteSetRumbleStatus(uint id, bool status) {
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

QDBusReply < bool> DBusDeviceEventsInterface::dbusIsClassicConnected(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusIsClassicConnected", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusIsNunchukConnected(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusIsNunchukConnected", argumentList);
}

QDBusReply < bool> DBusDeviceEventsInterface::dbusIsWiimoteConnected(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusIsWiimoteConnected", argumentList);
}

QDBusReply < uint> DBusDeviceEventsInterface::dbusWiimoteAverageLatency(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteAverageLatency", argumentList);
}

QDBusReply < uint> DBusDeviceEventsInterface::dbusWiimoteCurrentLatency(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteCurrentLatency", argumentList);
}

QDBusReply < uint8> DBusDeviceEventsInterface::dbusWiimoteGetStatus(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetStatus", argumentList);
}

QDBusReply < QString> DBusDeviceEventsInterface::dbusWiimoteGetMacAddress(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetMacAddress", argumentList);
}

QDBusReply < uint8> DBusDeviceEventsInterface::dbusWiimoteGetLedStatus(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetLedStatus", argumentList);
}

QDBusReply < uint> DBusDeviceEventsInterface::dbusWiimoteGetBatteryLife(uint id) {
  QList<QVariant> argumentList;
  argumentList << id;
  return callWithArgumentList(QDBus::Block, "dbusWiimoteGetBatteryLife", argumentList);
}
