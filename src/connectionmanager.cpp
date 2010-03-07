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

#include <QMapIterator>
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

QDBusArgument& operator<<(QDBusArgument& argument, const deviceinfo& info)
{
    argument.beginStructure();
    argument << info.id << QString().fromStdString(info.addr) << info.registred << info.nunchuk << info.classic;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument& argument, deviceinfo& info)
{
    QString data;
    argument.beginStructure();
    argument >> info.id >> data;
    info.addr = data.toStdString();
    argument >> info.registred >> info.nunchuk >> info.classic;
    argument.endStructure();
    return argument;
}

DeviceEventsClass::DeviceEventsClass(QObject *parent) : QDBusAbstractAdaptor(parent)
{
    qDBusRegisterMetaType< QList < struct irpoint> >();
    qDBusRegisterMetaType< QList < struct accdata> >();
    qDBusRegisterMetaType< QList < struct stickdata> >();
    qDBusRegisterMetaType< QList < struct deviceinfo> >();

    qDBusRegisterMetaType< struct irpoint>();
    qDBusRegisterMetaType< struct accdata>();
    qDBusRegisterMetaType< struct stickdata>();
    qDBusRegisterMetaType< struct deviceinfo>();

    setAutoRelaySignals(true);
}

QList < struct deviceinfo> DeviceEventsClass::dbusGetDeviceList()
{
    QList < struct deviceinfo> value;
    QMetaObject::invokeMethod(parent(), "dbusGetDeviceList", Qt::DirectConnection, Q_RETURN_ARG(QList < struct deviceinfo>, value));
    return value;
}

quint8 DeviceEventsClass::dbusWiimoteGetLedStatus(quint32 id)
{
    quint8 value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
    return value;
}

bool DeviceEventsClass::dbusWiimoteGetRumbleStatus(quint32 id)
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id));
    return value;
}

bool DeviceEventsClass::dbusWiimoteSetLedStatus(quint32 id, quint8 status)
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(quint8, status));
    return value;
}

bool DeviceEventsClass::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
    bool value;
    QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection,  Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(bool, status));
    return value;
}


ConnectionManager::ConnectionManager()
{
    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
    qRegisterMetaType< QList< accdata> >("QList< accdata>");
    qRegisterMetaType< QList< stickdata> >("QList< stickdata>");
    qRegisterMetaType< QList< deviceinfo> >("QList< deviceinfo>");

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< accdata>("accdata");
    qRegisterMetaType< stickdata>("stickdata");
    qRegisterMetaType< deviceinfo>("deviceinfo");

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

    memset(&bdaddr_any, 0x00, sizeof(uint8_t) * 6);
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

    struct deviceinfo dev;
    dev.id = connection->getWiimoteSequence();
    dev.registred = connection->getWiimoteSequence();
    dev.addr = connection->getWiimoteSAddr().toStdString();
    dev.nunchuk = false;
    dev.classic = false;

    deviceList[object] = dev;

    connect(connection, SIGNAL(dbusNunchukPlugged(quint32)), this, SLOT(slotDBusNunchukPlugged(quint32)));
    connect(connection, SIGNAL(dbusNunchukUnplugged(quint32)), this, SLOT(slotDBusNunchukUnplugged(quint32)));
    connect(connection, SIGNAL(dbusClassicControllerPlugged(quint32)), this, SLOT(slotDBusClassicControllerPlugged(quint32)));
    connect(connection, SIGNAL(dbusClassicControllerUnplugged(quint32)), this, SLOT(slotDBusClassicControllerUnplugged(quint32)));

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

    deviceList.remove(object);

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

void ConnectionManager::slotDBusNunchukPlugged(quint32)
{
    struct deviceinfo dev = deviceList[static_cast< void *>(sender())];
    dev.nunchuk = true;
    deviceList[static_cast< void *>(sender())] = dev;
}

void ConnectionManager::slotDBusNunchukUnplugged(quint32)
{
    struct deviceinfo dev = deviceList[static_cast< void *>(sender())];
    dev.nunchuk = false;
    deviceList[static_cast< void *>(sender())] = dev;
}

void ConnectionManager::slotDBusClassicControllerPlugged(quint32)
{
    struct deviceinfo dev = deviceList[static_cast< void *>(sender())];
    dev.classic = true;
    deviceList[static_cast< void *>(sender())] = dev;
}

void ConnectionManager::slotDBusClassicControllerUnplugged(quint32)
{
    struct deviceinfo dev = deviceList[static_cast< void *>(sender())];
    dev.classic = false;
    deviceList[static_cast< void *>(sender())] = dev;
}

QList < struct deviceinfo> ConnectionManager::dbusGetDeviceList()
{
    QList < struct deviceinfo> list;
    QMapIterator<void *, struct deviceinfo> i(deviceList);
    while (i.hasNext())
        list << i.next().value();
    return list;
}

quint8 ConnectionManager::dbusWiimoteGetLedStatus(quint32 id)
{
    quint8 status = 0x00;
    for (register int i = 0; i < objectList.count(); ++i)
        if (static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence() == id)
        {
            status = static_cast< WiimoteConnection*>( objectList.at(i))->getLedStatus();
            break;
        }
    return status;
}


bool ConnectionManager::dbusWiimoteGetRumbleStatus(quint32 id)
{
    bool status = false;
    for (register int i = 0; i < objectList.count(); ++i)
        if (static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence() == id)
        {
            status = static_cast< WiimoteConnection*>( objectList.at(i))->getRumbleStatus();
            break;
        }
    return status;
}

bool ConnectionManager::dbusWiimoteSetLedStatus(quint32 id, quint8 status)
{
    bool done = false;
    for (register int i = 0; i < objectList.count(); ++i)
        if (static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence() == id)
        {
            static_cast< WiimoteConnection*>( objectList.at(i))->setLedStatus(status);
            done = true;
            break;
        }
    return done;
}

bool ConnectionManager::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
    bool done = false;
    for (register int i = 0; i < objectList.count(); ++i)
        if (static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence() == id)
        {
            static_cast< WiimoteConnection*>( objectList.at(i))->setRumbleStatus(status);
            done = true;
            break;
        }
    return done;
}
