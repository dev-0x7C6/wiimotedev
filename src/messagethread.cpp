/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "messagethread.h"

MessageThread::MessageThread(int socketDescriptor, QObject *manager, QObject *parent) : socketDescriptor(socketDescriptor),  manager(manager), QThread(parent)
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
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void MessageThread::run()
{
    connect(manager, SIGNAL(dbusNunchukPlugged(quint32)), this, SLOT(slotDBusNunchukPlugged(quint32)));
    connect(manager, SIGNAL(dbusNunchukUnplugged(quint32)), this, SLOT(slotDBusNunchukUnplugged(quint32)));
    connect(manager, SIGNAL(dbusClassicControllerPlugged(quint32)), this, SLOT(slotDBusClassicControllerPlugged(quint32)));
    connect(manager, SIGNAL(dbusClassicControllerUnplugged(quint32)), this, SLOT(slotDBusClassicControllerUnplugged(quint32)));

    connect(manager, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)));

    connect(manager, SIGNAL(dbusWiimoteConnected(quint32)), this, SIGNAL(dbusWiimoteConnected(quint32)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusWiimoteDisconnected(quint32)), this, SIGNAL(dbusWiimoteDisconnected(quint32)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), this, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SIGNAL(dbusWiimoteButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusWiimoteStatus(quint32,quint8)), this, SIGNAL(dbusWiimoteStatus(quint32,quint8)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), this, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), this, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), Qt::QueuedConnection);

    connect(manager, SIGNAL(dbusNunchukPlugged(quint32)), this, SIGNAL(dbusNunchukPlugged(quint32)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusNunchukUnplugged(quint32)), this, SIGNAL(dbusNunchukUnplugged(quint32)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), this, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SIGNAL(dbusNunchukButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), this, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), Qt::QueuedConnection);

    connect(manager, SIGNAL(dbusClassicControllerPlugged(quint32)), this, SIGNAL(dbusClassicControllerPlugged(quint32)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusClassicControllerUnplugged(quint32)), this, SIGNAL(dbusClassicControllerUnplugged(quint32)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), this, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::QueuedConnection);
    connect(manager, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), this, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::QueuedConnection);

    tcpSocket = new QTcpSocket();
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket->error());
        return;
    }

    exec();


    disconnect(manager, 0, 0, 0);

    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();

    delete tcpSocket;
}

void MessageThread::dbusWiimoteGeneralButtons(quint32 id, quint64 value)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteGeneralButtons);
    out << static_cast< quint32>( id);
    out << static_cast< quint64>( value);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteConnected(quint32 id)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteConnected);
    out << static_cast< quint32>( id);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteDisconnected(quint32 id)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteDisconnected);
    out << static_cast< quint32>( id);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteBatteryLife(quint32 id, quint8 life)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteBatteryLife);
    out << static_cast< quint32>( id);
    out << static_cast< quint8>( life);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteButtons(quint32 id, quint64 value)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteButtons);
    out << static_cast< quint32>( id);
    out << static_cast< quint64>( value);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteStatus(quint32 id, quint8 status)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteStatus);
    out << static_cast< quint32>( id);
    out << static_cast< quint8>( status);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
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

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusWiimoteAcc(quint32 id, struct accdata acc)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusWiimoteAcc);
    out << static_cast< quint32>( id);

    out << static_cast< double>( acc.pitch);
    out << static_cast< double>( acc.roll);
    out << static_cast< quint8>( acc.x);
    out << static_cast< quint8>( acc.y);
    out << static_cast< quint8>( acc.z);

    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusNunchukPlugged(quint32 id)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusNunchukPlugged);
    out << static_cast< quint32>( id);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusNunchukUnplugged(quint32 id)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusNunchukUnplugged);
    out << static_cast< quint32>( id);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusNunchukButtons(quint32 id, quint64 value)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusNunchukButtons);
    out << static_cast< quint32>( id);
    out << static_cast< quint64>( value);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusNunchukStick(quint32 id, struct stickdata stick)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusNunchukStick);
    out << static_cast< quint32>( id);
    out << static_cast< quint8>( stick.x);
    out << static_cast< quint8>( stick.y);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusNunchukAcc(quint32 id, struct accdata acc)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusNunchukAcc);
    out << static_cast< quint32>( id);

    out << static_cast< double>( acc.pitch);
    out << static_cast< double>( acc.roll);
    out << static_cast< quint8>( acc.x);
    out << static_cast< quint8>( acc.y);
    out << static_cast< quint8>( acc.z);

    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusClassicControllerPlugged(quint32 id)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusClassicControllerPlugged);
    out << static_cast< quint32>( id);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusClassicControllerUnplugged(quint32 id)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusClassicControllerUnplugged);
    out << static_cast< quint32>( id);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusClassicControllerButtons(quint32 id, quint64 value)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusClassicControllerButtons);
    out << static_cast< quint32>( id);
    out << static_cast< quint64>( value);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusClassicControllerLStick(quint32 id, struct stickdata stick)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusClassicControllerLStick);
    out << static_cast< quint32>( id);
    out << static_cast< quint8>( stick.x);
    out << static_cast< quint8>( stick.y);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}

void MessageThread::dbusClassicControllerRStick(quint32 id, struct stickdata stick)
{
    mutex.lock();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << static_cast< quint16>( iddbusClassicControllerRStick);
    out << static_cast< quint32>( id);
    out << static_cast< quint8>( stick.x);
    out << static_cast< quint8>( stick.y);
    out.device()->seek(0);

    tcpSocket->write(block);
    mutex.unlock();
}
