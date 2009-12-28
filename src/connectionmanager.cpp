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

#include <QSettings>

#include "connectionmanager.h"
#include "datalogger.h"

extern QString filePathWiimotedev;
extern DataLogger dataLogger;

const QString sequenceGroup("sequence");

ConnectionManager::ConnectionManager(ProfileManager *manager)
{
    QSettings settings(filePathWiimotedev, QSettings::IniFormat);
    settings.beginGroup(sequenceGroup);

    for (register int i = 0; i < settings.allKeys().count(); ++i)
        wiiremoteSequence.insert(settings.allKeys().at(i), settings.value(settings.allKeys().at(i), 0).toInt());

    settings.endGroup();

    managerObject = manager;
    terminateReq = false;
    bdaddr_any = *BDADDR_ANY;

}

ConnectionManager::~ConnectionManager()
{
    terminateReq = true;

    connectionObject->_disconnect();
    disconnect(connectionObject, 0, 0, 0);

    while (objectList.count()) msleep(1);
}

void ConnectionManager::run()
{
    QTime time;
    while (!terminateReq)
    {
        connectionObject = new WiimoteConnection();
        connect(connectionObject, SIGNAL(registerConnection(void*)), this, SLOT(registerConnection(void*)), Qt::DirectConnection);
        time.start();
        if (connectionObject->connectAny())
            objectList << static_cast< void*>(connectionObject); else delete connectionObject;
        msleep((time.elapsed() < 100) ? 3000 : 0);
    }
}



void ConnectionManager::registerConnection(void *object)
{
    WiimoteConnection *connection = static_cast< WiimoteConnection*>( object);

    if (verboseLevel >= 4)
        qDebug(QString("Wiimotedev: connection established: %1").arg(connection->getWiimoteSAddr()).toAscii());

    connection->setWiimoteSequence(wiiremoteSequence.value(connection->getWiimoteSAddr(), 0));
    if (connection->getWiimoteSequence())
        connection->setLedStatus(connection->getWiimoteSequence());

    connect(connection, SIGNAL(unregisterConnection(void*)), this, SLOT(unregisterConnection(void*)), Qt::DirectConnection);
    connect(connection, SIGNAL(wiimoteStatusChanged(void*,quint8)), managerObject, SLOT(wiimoteStatusChanged(void*,quint8)), Qt::DirectConnection);
    connect(connection, SIGNAL(buttonStatusChanged(void*,quint64)), managerObject, SLOT(buttonStatusChanged(void*, quint64)), Qt::DirectConnection);
    connection->start();
}

void ConnectionManager::unregisterConnection(void *object)
{
    WiimoteConnection *connection = static_cast< WiimoteConnection*>( object);
    disconnect(connection, 0, 0, 0);

    if (verboseLevel >= 4)
        qDebug(QString("Wiimotedev: connection closed: %1").arg(connection->getWiimoteSAddr()).toAscii());

    while(connection->isRunning());
    delete connection;

    for (register int i = 0; i < objectList.count(); ++i) if(objectList.at(i) == object)
    {
        objectList.removeAt(i);
        break;
    }
}
