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

#include "network/support.h"

#ifdef USE_SYSLOG
#include "syslog/syslog.h"
#endif

MessageServerThread::MessageServerThread(QObject *manager, WiimotedevSettings* settings, quint16 port,  QObject *parent):
  QThread(parent),
  settings(settings),
  port(port),
  manager(manager)
{
  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void MessageServerThread::run() {
  MessageServer *server = new MessageServer(manager, settings, port);
  if (server->listen(QHostAddress::Any, port)) {
#ifdef USE_SYSLOG
    syslog::information(QString("listening on %1").arg(QString::number(port, 10)));
#endif
    exec();
  }
#ifdef USE_SYSLOG
  else
    syslog::information(QString("can't listen on %1, tcp service halted").arg(QString::number(port, 10)));
#endif
}

MessageServer::MessageServer(QObject *manager, WiimotedevSettings* settings, quint16 port, QObject *parent) : QTcpServer(parent), settings(settings), manager(manager), port(port)
{
  qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
  qRegisterMetaType< QList< accdata> >("QList< accdata>");
  qRegisterMetaType< QList< stickdata> >("QList< stickdata>");

  qRegisterMetaType< irpoint>("irpoint");
  qRegisterMetaType< accdata>("accdata");
  qRegisterMetaType< stickdata>("stickdata");

  connect(manager, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::QueuedConnection);

  connect(manager, SIGNAL(dbusWiimoteConnected(quint32)), this, SLOT(dbusWiimoteConnected(quint32)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusWiimoteDisconnected(quint32)), this, SLOT(dbusWiimoteDisconnected(quint32)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), this, SLOT(dbusWiimoteBatteryLife(quint32,quint8)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SLOT(dbusWiimoteButtons(quint32,quint64)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusWiimoteStatus(quint32,quint8)), this, SLOT(dbusWiimoteStatus(quint32,quint8)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), this, SLOT(dbusWiimoteAcc(quint32, struct accdata)), Qt::QueuedConnection);

  connect(manager, SIGNAL(dbusNunchukPlugged(quint32)), this, SLOT(dbusNunchukPlugged(quint32)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusNunchukUnplugged(quint32)), this, SLOT(dbusNunchukUnplugged(quint32)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), this, SLOT(dbusNunchukStick(quint32,struct stickdata)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SLOT(dbusNunchukButtons(quint32,quint64)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), this, SLOT(dbusNunchukAcc(quint32,struct accdata)), Qt::QueuedConnection);

  connect(manager, SIGNAL(dbusClassicControllerPlugged(quint32)), this, SLOT(dbusClassicControllerPlugged(quint32)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusClassicControllerUnplugged(quint32)), this, SLOT(dbusClassicControllerUnplugged(quint32)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SLOT(dbusClassicControllerButtons(quint32,quint64)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), this, SLOT(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::QueuedConnection);
  connect(manager, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), this, SLOT(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::QueuedConnection);
}

MessageServer::~MessageServer()
{
  for (register int i = 0; i < connections.count(); ++i)
    delete connections.at(i);
}

void MessageServer::tcpSendEvent(QByteArray &data)
{
  mutex.lock();
  QList < void* > ptrs;
  for (register int i = 0; i < connections.count(); ++i) {
    if (connections.at(i)->isWritable())
      connections.at(i)->write(data); else
      ptrs << static_cast< void*>( connections.at(i));
  }

  for (register int i = 0; i < ptrs.count(); ++i)
    connections.removeAt(connections.indexOf(static_cast< QTcpSocket*>( ptrs.at(i))));

  mutex.unlock();
}

void MessageServer::incomingConnection(int socketDescriptor)
{
  QTcpSocket *tcpSocket  = new QTcpSocket();

  if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
    delete tcpSocket;
    return;
  }

  quint32 host = tcpSocket->peerAddress().toIPv4Address();

#ifdef USE_SYSLOG
  syslog::information(QString("Incoming connection from %1").arg(tcpSocket->peerAddress())));
#endif

  bool accepted = false;

  QStringList allowed = settings->tcpGetAllowedHostList();

  for (register int i = 0; i < allowed.count(); ++i) if (QHostAddress(allowed.at(i)).toIPv4Address() == host) {
    accepted = true;
    break;
  }

  if (!accepted) {
#ifdef USE_SYSLOG
    syslog::information(QString("Connection rejected %1, probably host is not allowed").arg(tcpSocket->peerAddress()));
#endif
    tcpSocket->disconnectFromHost();
    delete tcpSocket;
    return;
  }

#ifdef USE_SYSLOG
  syslog::information(QString("Connection established with %1").arg(tcpSocket->peerAddress());
#endif
  connections << tcpSocket;
}

void MessageServer::dbusWiimoteGeneralButtons(quint32 id, quint64 value)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteGeneralButtons);
  out << static_cast< quint32>( id);
  out << static_cast< quint64>( value);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteConnected(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteConnected);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteDisconnected(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteDisconnected);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteBatteryLife(quint32 id, quint8 life)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteBatteryLife);
  out << static_cast< quint32>( id);
  out << static_cast< quint8>( life);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteButtons(quint32 id, quint64 value)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteButtons);
  out << static_cast< quint32>( id);
  out << static_cast< quint64>( value);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteStatus(quint32 id, quint8 status)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteStatus);
  out << static_cast< quint32>( id);
  out << static_cast< quint8>( status);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteInfrared);
  out << static_cast< quint32>( id);
  out << static_cast< quint32>( points.count());

  for (register int i = 0; i < points.count(); ++i)
  {
      out << static_cast< qint16>( points.at(i).size);
      out << static_cast< quint16>( points.at(i).x);
      out << static_cast< quint16>( points.at(i).y);
  }
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusWiimoteAcc(quint32 id, struct accdata acc)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteAcc);
  out << static_cast< quint32>( id);

  out << static_cast< double>( acc.pitch);
  out << static_cast< double>( acc.roll);
  out << static_cast< quint8>( acc.x);
  out << static_cast< quint8>( acc.y);
  out << static_cast< quint8>( acc.z);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusNunchukPlugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukPlugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusNunchukUnplugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukUnplugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusNunchukButtons(quint32 id, quint64 value)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukButtons);
  out << static_cast< quint32>( id);
  out << static_cast< quint64>( value);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusNunchukStick(quint32 id, struct stickdata stick)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukStick);
  out << static_cast< quint32>( id);
  out << static_cast< quint8>( stick.x);
  out << static_cast< quint8>( stick.y);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusNunchukAcc(quint32 id, struct accdata acc)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukAcc);
  out << static_cast< quint32>( id);

  out << static_cast< double>( acc.pitch);
  out << static_cast< double>( acc.roll);
  out << static_cast< quint8>( acc.x);
  out << static_cast< quint8>( acc.y);
  out << static_cast< quint8>( acc.z);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusClassicControllerPlugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerPlugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusClassicControllerUnplugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerUnplugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusClassicControllerButtons(quint32 id, quint64 value)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerButtons);
  out << static_cast< quint32>( id);
  out << static_cast< quint64>( value);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusClassicControllerLStick(quint32 id, struct stickdata stick)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerLStick);
  out << static_cast< quint32>( id);
  out << static_cast< quint8>( stick.x);
  out << static_cast< quint8>( stick.y);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void MessageServer::dbusClassicControllerRStick(quint32 id, struct stickdata stick)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerRStick);
  out << static_cast< quint32>( id);
  out << static_cast< quint8>( stick.x);
  out << static_cast< quint8>( stick.y);
  out.device()->seek(0);

  tcpSendEvent(block);
}
