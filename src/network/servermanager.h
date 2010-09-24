/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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

#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include "proto.h"
#include "consts.h"
#include "settings.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>

class ConnectionManager;

class NetworkSocket : public QTcpSocket
{
  Q_OBJECT
public:
  NetworkSocket(QObject *parent = 0);

private Q_SLOTS:
  void readyReadSlot();

Q_SIGNALS:
  void readyReadConnection(QTcpSocket*);
};


class NetworkServer : public QTcpServer
{
  Q_OBJECT
private:
  QList < NetworkSocket*> connections;
  QStringList allowed;
  QMutex mutex;
  ConnectionManager *manager;

public:
  NetworkServer(QStringList allowed, ConnectionManager *manager, QObject *parent = 0);
 ~NetworkServer();

protected:
  void incomingConnection(int socketDescriptor);
  void tcpSendEvent(QByteArray &data);

private Q_SLOTS:
  void readyRead(QTcpSocket*);

public Q_SLOTS:
  void dbusWiimoteGeneralButtons(quint32 id, quint64 value);
  void dbusWiimoteConnected(quint32 id);
  void dbusWiimoteDisconnected(quint32 id);
  void dbusWiimoteBatteryLife(quint32 id, quint8 life);
  void dbusWiimoteButtons(quint32 id, quint64 value);
  void dbusWiimoteStatus(quint32 id, quint8 status);
  void dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points);
  void dbusWiimoteAcc(quint32 id, struct accdata acc);
  void dbusNunchukPlugged(quint32 id);
  void dbusNunchukUnplugged(quint32 id);
  void dbusNunchukButtons(quint32 id, quint64 value);
  void dbusNunchukStick(quint32 id, struct stickdata stick);
  void dbusNunchukAcc(quint32 id, struct accdata acc);
  void dbusClassicControllerPlugged(quint32 id);
  void dbusClassicControllerUnplugged(quint32 id);
  void dbusClassicControllerButtons(quint32 id, quint64 value);
  void dbusClassicControllerLStick(quint32 id, struct stickdata stick);
  void dbusClassicControllerRStick(quint32 id, struct stickdata stick);
};


class NetworkServerThread :public QThread
{
private:
  QStringList allowed;
  quint16 port;
  ConnectionManager *manager;

public:
  NetworkServerThread(QStringList allowed, quint16 port, ConnectionManager *manager);
  NetworkServer *server;

protected:
  void run();
};



#endif // SERVER_MANAGER_H
