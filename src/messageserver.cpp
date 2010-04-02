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


#include "messageserver.h"
#include "messagethread.h"

const QString tcpSection("tcp/");

const QString tcpAllowedValue("allowed");
const QString tcpPortValue("port");

extern QString filePathWiimotedev;

MessageServerThread::MessageServerThread(QObject *manager, quint16 port,  QObject *parent) : manager(manager), port(port), QThread(parent)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void MessageServerThread::run()
{
    MessageServer *server = new MessageServer(manager, port);
    server->listen();
}

MessageServer::MessageServer(QObject *manager, quint16 port, QObject *parent) : manager(manager), port(port), QTcpServer(parent)
{
}

void MessageServer::incomingConnection(int socketDescriptor)
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor))
        return;

    bool accepted = false;
    quint32 host = tcpSocket.peerAddress().toIPv4Address();

    QSettings settings(filePathWiimotedev, QSettings::IniFormat);
    QStringList allowed = settings.value(tcpSection + tcpAllowedValue, QStringList()).toStringList();

    for (register int i = 0; i < allowed.count(); ++i)
        if (QHostAddress(allowed.at(i)).toIPv4Address() == host) {
            accepted  = true;
            break;
        }

    if (accepted)
    {
        MessageThread *thread = new MessageThread(socketDescriptor, manager);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}
