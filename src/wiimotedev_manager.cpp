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

#include "wiimotedev_manager.h"
#include <QTime>

ConnectionManager::ConnectionManager()
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

    terminateReq = false;

/* Setup ------------------------------------------------------------ */
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

/* Load rules ------------------------------------------------------- */
    syslog_message(QString::fromUtf8("loading rules from %1").arg(filePathWiimotedev).toAscii().constData());

    QSettings settings(filePathWiimotedev, QSettings::IniFormat);

    settings.beginGroup(sequenceSection);

    for (register int i = 0; i < settings.allKeys().count(); ++i)
        wiiremoteSequence.insert(settings.allKeys().at(i), settings.value(settings.allKeys().at(i), 0).toInt());

    settings.endGroup();

    DBusInterface = settings.value(wiimotedevSection + wiimotedevDBusIface, defDBusInterfaceEnabled).toBool();
    TCPInterface = settings.value(wiimotedevSection + wiimotedevTCPIface, defTCPInterfaceEnabled).toBool();

    tcpPort = settings.value(tcpSection + tcpPort, defTCPPort).toInt();

    memset(&bdaddr_any, 0x00, sizeof(uint8_t) * 6);

/* Syslog interface ------------------------------------------------- */
    syslog_message(QString::fromUtf8("dbus interface - %1").arg(DBusInterface ? "enabled" : "disabled").toAscii().constData());
    syslog_message(QString::fromUtf8("tcp/ip interface - %1").arg(TCPInterface ? "enabled" : "disabled").toAscii().constData());

/* DBus interface --------------------------------------------------- */

    if (DBusInterface) {
        QDBusConnection connection = QDBusConnection::systemBus();

        dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
        dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
        bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

        connect(this, SIGNAL(dbusReportUnregistredWiiremote(QString)), dbusDeviceEventsAdaptor, SIGNAL(dbusReportUnregistredWiiremote(QString)));

        syslog_message(QString::fromUtf8("register %1 object %2").arg(QString(WIIMOTEDEV_DBUS_EVENTS_OBJECT), dbusDeviceEventsAdaptor->isRegistred() ? "done" : "failed").toAscii().constData());
        syslog_message(QString::fromUtf8("register %1 object %2").arg(QString(WIIMOTEDEV_DBUS_SERVICE_OBJECT), dbusServiceAdaptor->isRegistred() ? "done" : "failed").toAscii().constData());
        syslog_message(QString::fromUtf8("register %1 service %2").arg(QString(WIIMOTEDEV_DBUS_SERVICE_NAME), registred ? "done" : "failed").toAscii().constData());
    }

/* TCP interface ---------------------------------------------------- */

    if (TCPInterface)
    {
        tcpServerThread = new MessageServerThread(this, tcpPort, this);
        tcpServerThread->start();
        syslog_message(QString::fromUtf8("starting tcp/ip thread at 0x%1").arg(QString::number(tcpServerThread->thread()->currentThreadId(), 0x10)).toAscii().constData());
    }

    if (!(TCPInterface || DBusInterface))
    {
        syslog_message(QString::fromUtf8("dbus/tcp disabled, stoping wiimotedev").toAscii().constData());
        terminateReq = true;
    }
}

bool ConnectionManager::dbusReloadSequenceList()
{
    wiiremoteSequence.clear();
    syslog_message(QString::fromUtf8("loading sequences from %1").arg(filePathWiimotedev).toAscii().constData());

    QSettings settings(filePathWiimotedev, QSettings::IniFormat);
    settings.beginGroup(sequenceSection);

    for (register int i = 0; i < settings.allKeys().count(); ++i)
        wiiremoteSequence.insert(settings.allKeys().at(i), settings.value(settings.allKeys().at(i), 0).toInt());

    settings.endGroup();

    return true;
}

ConnectionManager::~ConnectionManager()
{
    terminateReq = true;

    disconnect(connectionObject, 0, 0, 0);
    connectionObject->_disconnect();

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

    connection->setWiimoteSequence(wiiremoteSequence.value(connection->getWiimoteSAddr(), 0));
    if (connection->getWiimoteSequence())
        connection->setLedStatus(connection->getWiimoteSequence());

    syslog_message(QString::fromUtf8("Established connection with wiiremote, ID %1 MAC %2").arg(QString::number(connection->getWiimoteSequence(), 10), connection->getWiimoteSAddr()).toAscii().constData());
    if (!connection->getWiimoteSequence())
    {
        syslog_message(QString::fromUtf8("Wiiremote MAC %1 is unregistred, run wiimotedev-register and reconnect device").arg(connection->getWiimoteSAddr()).toAscii().constData());
        QString mac = connection->getWiimoteSAddr();

        bool exist = false;

        for (register int i = 0; i < unregisterWiiremoteList.count(); ++i)
            if (unregisterWiiremoteList.at(i) == mac) {
                exist = true;
                break;
            }

        if (!exist) unregisterWiiremoteList << mac;

        connection->_disconnect();
        unregisterConnection(static_cast< void*>( connection));
        emit dbusReportUnregistredWiiremote(mac);
        return;
    }

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

    connect(connection, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::DirectConnection);

    connect(connection, SIGNAL(dbusWiimoteConnected(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteConnected(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteDisconnected(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteDisconnected(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteButtons(quint32,quint64)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteStatus(quint32,quint8)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteStatus(quint32,quint8)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), Qt::DirectConnection);

    connect(connection, SIGNAL(dbusNunchukPlugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukPlugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukUnplugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukUnplugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukButtons(quint32,quint64)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), Qt::DirectConnection);

    connect(connection, SIGNAL(dbusClassicControllerPlugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerPlugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerUnplugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerUnplugged(quint32)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::DirectConnection);
    connect(connection, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::DirectConnection);

  // profile interface
    connect(connection, SIGNAL(unregisterConnection(void*)), this, SLOT(unregisterConnection(void*)), Qt::QueuedConnection);
    connection->start();
}

void ConnectionManager::unregisterConnection(void *object)
{
    WiimoteConnection *connection = static_cast< WiimoteConnection*>( object);
    disconnect(connection, 0, 0, 0);

    deviceList.remove(object);

    syslog_message(QString::fromUtf8("Wiiremote disconnected, ID %1; MAC %2").arg(QString::number(connection->getWiimoteSequence(), 10), connection->getWiimoteSAddr()).toAscii().constData());

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

QList < quint32> ConnectionManager::dbusGetDeviceList()
{
    QList < quint32> list;
    for (register int i = 0; i < objectList.count(); ++i)
        list << static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence();
    return list;
}

QStringList ConnectionManager::dbusUnregistredWiiremoteList()
{
    return unregisterWiiremoteList;
}

quint8 ConnectionManager::dbusWiimoteGetStatus(quint32 id)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection)
        return connection->getWiimoteStatus();
    return 0;
}

WiimoteConnection* ConnectionManager::findWiiremoteObject(quint32 id)
{
    for (register int i = 0; i < objectList.count(); ++i)
        if (static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence() == id)
            return static_cast< WiimoteConnection*>( objectList.at(i));
    return static_cast< WiimoteConnection*>( 0);
}

quint8 ConnectionManager::dbusWiimoteGetLedStatus(quint32 id)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection)
        return connection->getLedStatus();
    return 0;
}


bool ConnectionManager::dbusWiimoteGetRumbleStatus(quint32 id)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection)
        return connection->getRumbleStatus();
    return false;
}

bool ConnectionManager::dbusWiimoteSetLedStatus(quint32 id, quint8 status)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection) {
        connection->setLedStatus(status);
        return true;
    }
    return false;
}

bool ConnectionManager::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection) {
        connection->setRumbleStatus(status);
        return true;
    }
    return false;
}

quint32 ConnectionManager::dbusWiimoteGetCurrentLatency(quint32 id)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection)
        return connection->dbusWiimoteGetCurrentLatency();
    return 0;
}

quint32 ConnectionManager::dbusWiimoteGetAverageLatency(quint32 id)
{
    WiimoteConnection *connection = findWiiremoteObject(id);
    if (connection)
        return connection->dbusWiimoteGetAverageLatency();
    return 0;
}
