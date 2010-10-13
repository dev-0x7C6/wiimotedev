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

#include "core/manager.h"
#include "network/servermanager.h"
#include "syslog/syslog.h"

NetworkServerThread::NetworkServerThread(QStringList allowed, quint16 port, ConnectionManager *manager)
  :allowed(allowed),
   port(port),
   manager(manager)
{
}

void NetworkServerThread::run() {
  server = new NetworkServer(allowed, manager);

  if (server->listen(QHostAddress::Any, port)) {
    systemlog::notice(QString("listening on %1").arg(QString::number(port, 10)));
    exec();
    server->close();
  } else
    systemlog::critical(QString("can't listen on %1, tcp service halted").arg(QString::number(port, 10)));

  delete server;
}

NetworkSocket::NetworkSocket(QObject *parent) : QTcpSocket(parent) {
  connect(this, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
}

void NetworkSocket::readyReadSlot() {
  emit readyReadConnection(this);
}

NetworkServer::NetworkServer(QStringList allowed, ConnectionManager *manager, QObject *parent):
  QTcpServer(parent),
  allowed(allowed),
  manager(manager)
{
}

NetworkServer::~NetworkServer()
{
  if (connections.isEmpty())
    return;

  foreach (NetworkSocket *socket, connections) {
    if (socket->isValid())
      socket->disconnectFromHost();
    delete socket;
  }
}

void NetworkServer::tcpSendEvent(QByteArray &data)
{
  mutex.lock();
  QList < NetworkSocket*> brokenConnections;

  foreach (NetworkSocket *socket, connections) {
    if (!(socket->isWritable() && socket->isValid())) {
      brokenConnections << socket;
      continue;
    }
    socket->write(data);
    socket->waitForBytesWritten();
  }

  foreach (NetworkSocket *broken, brokenConnections) {
    connections.removeAt(connections.indexOf(broken, 0));
    delete broken;
  }

  mutex.unlock();
}

void NetworkServer::incomingConnection(int socketDescriptor)
{
  NetworkSocket *tcpSocket  = new NetworkSocket();

  if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
    delete tcpSocket;
    return;
  }

  systemlog::information(QString("Incoming connection from %1").arg(tcpSocket->peerAddress().toString()));

  bool accepted = false;

  foreach (const QString& host, allowed) {
    if (QHostAddress(host) == tcpSocket->peerAddress()) {
      accepted = true;
      break;
    }
  }

  if (!accepted) {
    systemlog::information(QString("Connection rejected %1").arg(tcpSocket->peerAddress().toString()));
    tcpSocket->disconnectFromHost();
    delete tcpSocket;
    return;
  }

  systemlog::information(QString("Connection established with %1").arg(tcpSocket->peerAddress().toString()));
  connections << tcpSocket;

  connect(connections.last(), SIGNAL(readyReadConnection(QTcpSocket*)), this, SLOT(readyRead(QTcpSocket*)), Qt::DirectConnection);
}


void NetworkServer::dbusWiimoteGeneralButtons(quint32 id, quint64 value)
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

void NetworkServer::dbusWiimoteConnected(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteConnected);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void NetworkServer::dbusWiimoteDisconnected(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusWiimoteDisconnected);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void NetworkServer::dbusWiimoteBatteryLife(quint32 id, quint8 life)
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

void NetworkServer::dbusWiimoteButtons(quint32 id, quint64 value)
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

void NetworkServer::dbusWiimoteStatus(quint32 id, quint8 status)
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

void NetworkServer::dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points)
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

void NetworkServer::dbusWiimoteAcc(quint32 id, struct accdata acc)
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

void NetworkServer::dbusNunchukPlugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukPlugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void NetworkServer::dbusNunchukUnplugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusNunchukUnplugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void NetworkServer::dbusNunchukButtons(quint32 id, quint64 value)
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

void NetworkServer::dbusNunchukStick(quint32 id, struct stickdata stick)
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

void NetworkServer::dbusNunchukAcc(quint32 id, struct accdata acc)
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

void NetworkServer::dbusClassicControllerPlugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerPlugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void NetworkServer::dbusClassicControllerUnplugged(quint32 id)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast< quint16>( iddbusClassicControllerUnplugged);
  out << static_cast< quint32>( id);
  out.device()->seek(0);

  tcpSendEvent(block);
}

void NetworkServer::dbusClassicControllerButtons(quint32 id, quint64 value)
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

void NetworkServer::dbusClassicControllerLStick(quint32 id, struct stickdata stick)
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

void NetworkServer::dbusClassicControllerRStick(quint32 id, struct stickdata stick)
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

void NetworkServer::readyRead(QTcpSocket *socket) {
  mutex.lock();
  QDataStream stream(socket);
  stream.setVersion(QDataStream::Qt_4_0);

  quint16 signal;
  quint32 id;

  quint32 status;

  stream >> signal >> id;
  switch (signal) {
    case iddbusWiimoteSetLedStatus:
      stream >> status;
      stream << iddbusWiimoteSetLedStatus << id << manager->dbusWiimoteSetLedStatus(id, status);
      break;
  }

  mutex.unlock();
}
