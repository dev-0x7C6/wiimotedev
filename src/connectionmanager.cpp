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

QDBusArgument& operator<<(QDBusArgument& argument, const irpoint& point)
{
    argument.beginStructure();
    argument << point.size << point.x << point.y;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, irpoint& point)
{
    argument.beginStructure();
    argument >> point.size >> point.x >> point.y;
    argument.endStructure();
    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const accdata& acc)
{
    argument.beginStructure();
    argument << acc.x << acc.y << acc.z << acc.pitch << acc.roll;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, accdata& acc)
{
    argument.beginStructure();
    argument >> acc.x >> acc.y >> acc.z >> acc.pitch >> acc.roll;
    argument.endStructure();
    return argument;
}

QDBusArgument& operator<<(QDBusArgument& argument, const stickdata& stick)
{
    argument.beginStructure();
    argument << stick.x << stick.y;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, stickdata& stick)
{
    argument.beginStructure();
    argument >> stick.x >> stick.y;
    argument.endStructure();
    return argument;
}

DeviceEventsClass::DeviceEventsClass(QObject *parent) : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

QStringList DeviceEventsClass::dbusGetWiimoteList()
{
    QStringList value;
    QMetaObject::invokeMethod(parent(), "dbusGetWiimoteList", Q_RETURN_ARG(QStringList, value));
    return value;
}


ConnectionManager::ConnectionManager()
{
    qDBusRegisterMetaType< QList < struct irpoint> >();
    qDBusRegisterMetaType< QList < struct accdata> >();
    qDBusRegisterMetaType< QList < struct stickdata> >();

    qDBusRegisterMetaType< struct irpoint>();
    qDBusRegisterMetaType< struct accdata>();
    qDBusRegisterMetaType< struct stickdata>();

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< accdata>("accdata");
    qRegisterMetaType< stickdata>("stickdata");

    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
    qRegisterMetaType< QList< accdata> >("QList< accdata>");
    qRegisterMetaType< QList< stickdata> >("QList< stickdata>");

    QSettings settings(filePathWiimotedev, QSettings::IniFormat);
    settings.beginGroup(sequenceGroup);

    for (register int i = 0; i < settings.allKeys().count(); ++i)
        wiiremoteSequence.insert(settings.allKeys().at(i), settings.value(settings.allKeys().at(i), 0).toInt());

    settings.endGroup();

    new DeviceEventsClass(this);

    QDBusConnection connection = QDBusConnection::systemBus();
    connection.registerObject(WIIMOTEDEV_DBUS_OBJECT_NAME, this);
    connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

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

#ifdef __syslog
    syslog(LOG_NOTICE, "Connection established with %s", connection->getWiimoteSAddr().toAscii().data());
#endif

    connection->setWiimoteSequence(wiiremoteSequence.value(connection->getWiimoteSAddr(), 0));
    if (connection->getWiimoteSequence())
        connection->setLedStatus(connection->getWiimoteSequence());

    connect(connection, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::DirectConnection);

    connect(connection, SIGNAL(dbusWiimoteConnected(quint32)), this, SIGNAL(dbusWiimoteConnected(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteDisconnected(quint32)), this, SIGNAL(dbusWiimoteDisconnected(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), this, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SIGNAL(dbusWiimoteButtons(quint32,quint64)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteStatus(quint32,quint8)), this, SIGNAL(dbusWiimoteStatus(quint32,quint8)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), this, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), this, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), Qt::DirectConnection);

    connect(connection, SIGNAL(dbusNunchukPlugged(quint32)), this, SIGNAL(dbusNunchukPlugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukUnplugged(quint32)), this, SIGNAL(dbusNunchukUnplugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), this, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SIGNAL(dbusNunchukButtons(quint32,quint64)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), this, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), Qt::DirectConnection);

    connect(connection, SIGNAL(dbusClassicControllerPlugged(quint32)), this, SIGNAL(dbusClassicControllerPlugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerUnplugged(quint32)), this, SIGNAL(dbusClassicControllerUnplugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), this, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), this, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::DirectConnection);

  // profile interface
    connect(connection, SIGNAL(unregisterConnection(void*)), this, SLOT(unregisterConnection(void*)), Qt::QueuedConnection);
    connection->start();
}

void ConnectionManager::unregisterConnection(void *object)
{
    WiimoteConnection *connection = static_cast< WiimoteConnection*>( object);
    disconnect(connection, 0, 0, 0);

#ifdef __syslog
    syslog(LOG_NOTICE, "Connection closed %s", connection->getWiimoteSAddr().toAscii().data());
#endif

    connection->wait();
    delete connection;

    for (register int i = 0; i < objectList.count(); ++i) if(objectList.at(i) == object)
    {
        objectList.removeAt(i);
        break;
    }
}
