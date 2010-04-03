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


#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSettings>
#include <QStringList>
#include <QThread>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QMetaType>
#include <QDBusMetaType>
#include <QSettings>

#include "wiimotedevproto.h"
#include "wiimotedev.h"

Q_DECLARE_METATYPE(irpoint);
Q_DECLARE_METATYPE(accdata);
Q_DECLARE_METATYPE(stickdata);
Q_DECLARE_METATYPE(deviceinfo);

Q_DECLARE_METATYPE(QList < irpoint>);
Q_DECLARE_METATYPE(QList < accdata>);
Q_DECLARE_METATYPE(QList < stickdata>);
Q_DECLARE_METATYPE(QList < deviceinfo>);


class MessageServer : public QTcpServer
{
Q_OBJECT
public:
    explicit MessageServer(QObject *manager, quint16 port, QObject *parent = 0);
    virtual ~MessageServer();

protected:
    void incomingConnection(int socketDescriptor);    

private:
    QList < QTcpSocket*> connections;

    void tcpSendEvent(QByteArray &data);

    quint16 port;
    QObject *manager;
    QMutex mutex;



public slots:
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

class MessageServerThread : public QThread
{
    Q_OBJECT
public:
    explicit MessageServerThread(QObject *manager, quint16 port,  QObject *parent = 0);
    void run();

private:
    quint16 port;
    QObject *manager;

};


#endif // MESSAGESERVER_H
