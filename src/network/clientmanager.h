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

#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "adaptors/deviceevents.h"
#include "adaptors/daemonservice.h"
#include "headers/consts.h"
#include "headers/proto.h"
#include "syslog/syslog.h"

#include <QThread>
#include <QTcpSocket>
#include <QEventLoop>

const QString clientSection("client/");
const QString deftcpTimeout("timeout");
const QString deftcpHostname("hostname");
const QString deftcpPort("port");

const QString confFile("/etc/wiimotedev/client.conf");

class ConnectionManager : public QThread
{
  Q_OBJECT
private:     
  DBusDeviceEventsAdaptorWrapper *dbusDeviceEventsAdaptor;
  DBusServiceAdaptorWrapper *dbusServiceAdaptor;

  quint32 tcpTimeout;
  QString tcpHostname;
  quint16 tcpPort;

  QTcpSocket *socket;

  bool terminateReq;

  QHash < qint32, QEventLoop*> waitMethod;
  QHash < qint32, QVariant> outMethod;

public:
  ConnectionManager();
 ~ConnectionManager();

 void terminateRequest();

protected:
  void run();

private Q_SLOTS:
  void readyRead();

public Q_SLOTS:
  bool dbusIsClassicConnected(quint32 id);
  bool dbusIsNunchukConnected(quint32 id);
  bool dbusIsWiimoteConnected(quint32 id);

  quint32 dbusWiimoteGetAverageLatency(quint32 id);
  quint32 dbusWiimoteGetBatteryLife(quint32 id);
  quint32 dbusWiimoteGetCurrentLatency(quint32 id);

  bool dbusWiimoteGetRumbleStatus(quint32 id);
  bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
  bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
  quint8 dbusWiimoteGetLedStatus(quint32 id);

  quint8 dbusWiimoteGetStatus(quint32 id);

  QList < uint> dbusGetWiimoteList();
  QStringList dbusGetUnregistredWiimoteList();

  bool dbusReloadSequenceList();

Q_SIGNALS:
  void dbusWiimoteGeneralButtons(quint32, quint64);
  void dbusWiimoteConnected(quint32);
  void dbusWiimoteDisconnected(quint32);
  void dbusWiimoteBatteryLife(quint32, quint8);
  void dbusWiimoteButtons(quint32, quint64);
  void dbusWiimoteStatus(quint32, quint8);
  void dbusWiimoteInfrared(quint32, QList< struct irpoint>);
  void dbusWiimoteAcc(quint32, struct accdata);
  void dbusNunchukPlugged(quint32);
  void dbusNunchukUnplugged(quint32);
  void dbusNunchukButtons(quint32, quint64);
  void dbusNunchukStick(quint32, struct stickdata);
  void dbusNunchukAcc(quint32, struct accdata);
  void dbusClassicControllerPlugged(quint32);
  void dbusClassicControllerUnplugged(quint32);
  void dbusClassicControllerButtons(quint32, quint64);
  void dbusClassicControllerLStick(quint32, struct stickdata);
  void dbusClassicControllerRStick(quint32, struct stickdata);
};

#endif // CLIENT_MANAGER_H
