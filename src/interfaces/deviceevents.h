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
  QDBusReply < bool> dbusIsClassicConnected(quint32 id);
  QDBusReply < bool> dbusIsNunchukConnected(quint32 id);
  QDBusReply < bool> dbusIsWiimoteConnected(quint32 id);

  QDBusReply < quint8> dbusWiimoteAverageLatency(quint32 id);
  QDBusReply < quint8> dbusWiimoteCurrentLatency(quint32 id);
  QDBusReply < quint8> dbusWiimoteGetStatus(quint32 id);
  QDBusReply < quint32> dbusWiimoteGetBatteryLife(quint32 id);
  QDBusReply < QString> dbusWiimoteGetMacAddress(quint32 id);
  QDBusReply < bool> dbusWiimoteGetRumbleStatus(quint32 id);
  QDBusReply < quint8> dbusWiimoteGetLedStatus(quint32 id);
  QDBusReply < bool> dbusWiimoteSetLedStatus(quint32 id, quint32 status);
  QDBusReply < bool> dbusWiimoteSetRumbleStatus(quint32 id, bool status);

Q_SIGNALS:
  void dbusWiimoteGeneralButtons(quint32, quint64);

  void dbusVirtualCursorPosition(quint32, double, double, double, double);
  void dbusVirtualCursorFound(quint32);
  void dbusVirtualCursorLost(quint32);

  void dbusWiimoteConnected(quint32);
  void dbusWiimoteDisconnected(quint32);
  void dbusWiimoteBatteryLife(quint32, quint8);
  void dbusWiimoteButtons(quint32, quint64);
  void dbusWiimoteLedStatusChanged(quint32, quint8);
  void dbusWiimoteRumbleStatusChanged(quint32, quint8);
  void dbusWiimoteStatus(quint32, quint8);
  void dbusWiimoteInfrared(quint32, const QList< irpoint>&);
  void dbusWiimoteAcc(quint32, const accdata&);

  void dbusNunchukPlugged(quint32);
  void dbusNunchukUnplugged(quint32);
  void dbusNunchukButtons(quint32, quint64);
  void dbusNunchukStick(quint32, const stickdata&);
  void dbusNunchukAcc(quint32, const accdata&);

  void dbusClassicControllerPlugged(quint32);
  void dbusClassicControllerUnplugged(quint32);
  void dbusClassicControllerButtons(quint32, quint64);
  void dbusClassicControllerLStick(quint32, const stickdata&);
  void dbusClassicControllerRStick(quint32, const stickdata&);

  void dbusReportUnregisteredWiimote(quint32);
};

#endif // DEVICEEVENTS_INTERFACE_H
