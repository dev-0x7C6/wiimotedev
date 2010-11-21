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

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QThread>
#include <QMutex>
#include <QReadWriteLock>

#include "adaptors/deviceevents.h"
#include "adaptors/daemonservice.h"
#include "core/connection.h"
#include "core/settings.h"
#include "headers/consts.h"
#include "network/servermanager.h"

template class QList< void*>;

class ConnectionManager : public QThread
{
  Q_OBJECT
private:
// Adaptor section ------------------------------------------ /
  DBusDeviceEventsAdaptorWrapper *dbusDeviceEventsAdaptor;
  DBusServiceAdaptorWrapper *dbusServiceAdaptor;

//
  bool terminateReq;

  bool dbusInterfaceCreated;
  bool tcpInterfaceCreated;

// Settings ------------------------------------------------- /
  WiimotedevSettings *settings;
  NetworkServerThread *networkServerThread;

  QList< WiimoteConnection*> connections;

  QMap< QString, bool> unregisterWiimoteList;

  QMap< QString, quint32> sequence;
  bdaddr_t bdaddr_any;

  WiimoteConnection* findWiiremoteObject(quint32 id);
  QMutex *mutex;
  QReadWriteLock *rwlock;

public:
  ConnectionManager(QObject *parent = 0);
 ~ConnectionManager();

  bool getTerminateRequest();
  void setTerminateRequest(bool);

  quint32 result;

  static const int WaitForBluetooth = 3000;

protected:
  void run();

private Q_SLOTS:
  void freeConnection(WiimoteConnection*);
  void freeAllConnections();
  bool registerConnection(WiimoteConnection*);


public Q_SLOTS:
  bool dbusIsClassicConnected(quint32 id);
  bool dbusIsNunchukConnected(quint32 id);
  bool dbusIsWiimoteConnected(quint32 id);

  QList< uint> dbusNunchukGetAccelerometrCalibration(quint32 id);
  QList< uint> dbusWiimoteGetAccelerometrCalibration(quint32 id);

  quint32 dbusWiimoteGetAverageLatency(quint32 id);
  quint32 dbusWiimoteGetBatteryLife(quint32 id);
  quint32 dbusWiimoteGetCurrentLatency(quint32 id);
  QString dbusWiimoteGetMacAddress(quint32 id);

  bool dbusWiimoteGetRumbleStatus(quint32 id);
  bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
  bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
  quint8 dbusWiimoteGetLedStatus(quint32 id);

  quint8 dbusWiimoteGetStatus(quint32 id);

  QList < uint> dbusGetWiimoteList();
  QStringList dbusGetUnregistredWiimoteList();

  bool dbusReloadSequenceList();

Q_SIGNALS:
  void dbusReportUnregistredWiimote(QString);
};

#endif // CONNECTIONMANAGER_H
