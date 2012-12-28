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

#ifndef DEVICEEVENTS_INTERFACE_H
#define DEVICEEVENTS_INTERFACE_H

#include "headers/consts.h"
#include "interfaces.h"

class DBusDeviceEventsInterface :public QDBusAbstractInterface
{
  Q_OBJECT
public:
  static inline const char *staticInterfaceName() { return WIIMOTEDEV_DBUS_IFACE_EVENTS; }

public:
  DBusDeviceEventsInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0)
    :QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent) { QWIIMOTEDEV_REGISTER_META_TYPES }

public Q_SLOTS:
  QDBusReply < QList <uint> > dbusGetWiimoteList();
  QDBusReply < QStringList> dbusGetUnregistredWiimoteList();
  QDBusReply < bool> dbusIsClassicConnected(uint id);
  QDBusReply < bool> dbusIsNunchukConnected(uint id);
  QDBusReply < bool> dbusIsWiimoteConnected(uint id);

  QDBusReply < uint8> dbusWiimoteAverageLatency(uint id);
  QDBusReply < uint8> dbusWiimoteCurrentLatency(uint id);
  QDBusReply < uint8> dbusWiimoteGetStatus(uint id);
  QDBusReply < uint> dbusWiimoteGetBatteryLife(uint id);
  QDBusReply < QString> dbusWiimoteGetMacAddress(uint id);
  QDBusReply < bool> dbusWiimoteGetRumbleStatus(uint id);
  QDBusReply < uint8> dbusWiimoteGetLedStatus(uint id);
  QDBusReply < bool> dbusWiimoteSetLedStatus(uint id, uint status);
  QDBusReply < bool> dbusWiimoteSetRumbleStatus(uint id, bool status);

Q_SIGNALS:
  void dbusWiimoteGeneralButtons(uint, uint64);

  void dbusVirtualCursorPosition(uint, double, double, double, double);
  void dbusVirtualCursorFound(uint);
  void dbusVirtualCursorLost(uint);

  void dbusWiimoteConnected(uint);
  void dbusWiimoteDisconnected(uint);
  void dbusWiimoteBatteryLife(uint, uint8);
  void dbusWiimoteButtons(uint, uint64);
  void dbusWiimoteLedStatusChanged(uint, uint8);
  void dbusWiimoteRumbleStatusChanged(uint, uint8);
  void dbusWiimoteStatus(uint, uint8);
  void dbusWiimoteInfrared(uint, const QList< irpoint>&);
  void dbusWiimoteAcc(uint, const accdata&);

  void dbusNunchukPlugged(uint);
  void dbusNunchukUnplugged(uint);
  void dbusNunchukButtons(uint, uint64);
  void dbusNunchukStick(uint, const stickdata&);
  void dbusNunchukAcc(uint, const accdata&);

  void dbusClassicControllerPlugged(uint);
  void dbusClassicControllerUnplugged(uint);
  void dbusClassicControllerButtons(uint, uint64);
  void dbusClassicControllerLStick(uint, const stickdata&);
  void dbusClassicControllerRStick(uint, const stickdata&);

  void dbusReportUnregisteredWiimote(uint);
};

#endif // DEVICEEVENTS_INTERFACE_H
