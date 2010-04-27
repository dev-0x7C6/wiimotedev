/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "tcp_support.h"

MessageServerThread::MessageServerThread(QObject *manager, quint16 port,  QObject *parent) : QThread(parent), port(port), manager(manager)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void MessageServerThread::run()
{
    MessageServer *server = new MessageServer(manager, port);
    if (server->listen(QHostAddress::Any, port))
    {
        syslog_message(QString::fromUtf8("listening on %1").arg(QString::number(port, 10)).toAscii().constData());
        exec();
    } else
        syslog_message(QString::fromUtf8("can't listen on %1, tcp service halted").arg(QString::number(port, 10)).toAscii().constData());
}

extern QString filePathWiimotedev;

const QString tcpSection("tcp/");

const QString tcpAllowedValue("allowed");
const QString tcpPortValue("port");

MessageServer::MessageServer(QObject *manager, quint16 port, QObject *parent) : QTcpServer(parent), manager(manager), port(port)
{
/* Register Meta Types ---------------------------------------------- */
    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
    qRegisterMetaType< QList< accdata> >("QList< accdata>");
    qRegisterMetaType< QList< stickdata> >("QList< stickdata>");
    qRegisterMetaType< QList< deviceinfo> >("QList< deviceinfo>");

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< accdata>("accdata");
    qRegisterMetaType< stickdata>("stickdata");
    qRegisterMetaType< deviceinfo>("deviceinfo");

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
    for (register int i = 0; i < connections.count(); ++i) {
        connections.at(i)->write(data);
    }
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

    syslog_message(QString::fromUtf8("Incoming connection from %1").arg(tcpSocket->peerAddress().toString()).toAscii().constData());

    bool accepted = false;

    QSettings *settings = new QSettings(filePathWiimotedev, QSettings::IniFormat);
    QStringList allowed = settings->value(tcpSection + tcpAllowedValue, QStringList()).toStringList();

    for (register int i = 0; i < allowed.count(); ++i) if (QHostAddress(allowed.at(i)).toIPv4Address() == host) {
        accepted = true;
        break;
    }

    delete settings;

    syslog_message(QString::fromUtf8("Incoming connection from %1").arg(tcpSocket->peerAddress().toString()).toAscii().constData());

    if (!accepted) {
        syslog_message(QString::fromUtf8("Connection rejected %1, probably host is not allowed").arg(tcpSocket->peerAddress().toString()).toAscii().constData());
        tcpSocket->disconnectFromHost();
        delete tcpSocket;
        return;
    }

    syslog_message(QString::fromUtf8("Connection established with %1").arg(tcpSocket->peerAddress().toString()).toAscii().constData());

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
