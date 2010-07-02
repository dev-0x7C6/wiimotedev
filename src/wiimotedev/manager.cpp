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

#include "manager.h"
#include <QTime>

ConnectionManager::ConnectionManager()
{
// Register Meta Types ---------------------------------------------- /
    setTerminationEnabled(true);

    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
    qRegisterMetaType< QList< accdata> >("QList< accdata>");
    qRegisterMetaType< QList< stickdata> >("QList< stickdata>");
    qRegisterMetaType< QList< deviceinfo> >("QList< deviceinfo>");

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< accdata>("accdata");
    qRegisterMetaType< stickdata>("stickdata");
    qRegisterMetaType< deviceinfo>("deviceinfo");

    terminateReq = false;

// Setup ------------------------------------------------------------ /
    memset(&bdaddr_any, 0x00, sizeof(uint8_t) * 6);

    syslog(QString("loading rules from %1").arg(WIIMOTEDEV_CONFIG_FILE));

    wiimotedevSettings = new WiimotedevSettings(this, WIIMOTEDEV_CONFIG_FILE);   
    wiiremoteSequence = wiimotedevSettings->getWiiremoteSequence();

    syslog(QString("dbus interface - %1").arg(wiimotedevSettings->dbusInterfaceSupport() ? "enabled" : "disabled"));
    syslog(QString("tcp/ip interface - %1").arg(wiimotedevSettings->tcpInterfaceSupport() ? "enabled" : "disabled"));

// DBus interface --------------------------------------------------- /

    if (wiimotedevSettings->dbusInterfaceSupport()) {
        QDBusConnection connection = QDBusConnection::systemBus();

        dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
        dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
        bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

        connect(this, SIGNAL(dbusReportUnregistredWiiremote(QString)), dbusDeviceEventsAdaptor, SIGNAL(dbusReportUnregistredWiiremote(QString)));

        syslog(QString("register %1 object %2").arg(WIIMOTEDEV_DBUS_OBJECT_EVENTS, dbusDeviceEventsAdaptor->isRegistred() ? "done" : "failed"));
        syslog(QString("register %1 object %2").arg(WIIMOTEDEV_DBUS_OBJECT_SERVICE, dbusServiceAdaptor->isRegistred() ? "done" : "failed"));
        syslog(QString("register %1 service %2").arg(WIIMOTEDEV_DBUS_SERVICE_NAME, registred ? "done" : "failed"));
    }

// TCP interface ---------------------------------------------------- /

    if (wiimotedevSettings->tcpInterfaceSupport()) {
        tcpServerThread = new MessageServerThread(this, wiimotedevSettings, wiimotedevSettings->tcpGetPort(), this);
        tcpServerThread->start();
        syslog(QString("starting tcp/ip thread at 0x%1").arg(QString::number(tcpServerThread->thread()->currentThreadId(), 0x10)));
    }

    if (!(wiimotedevSettings->tcpInterfaceSupport() || wiimotedevSettings->dbusInterfaceSupport())) {
        syslog(QString("dbus/tcp disabled, stoping wiimotedev"));
        terminateReq = true;
    }
}

ConnectionManager::~ConnectionManager()
{
    delete wiimotedevSettings;
}

bool ConnectionManager::dbusReloadSequenceList()
{
    syslog(QString("loading sequences from %1").arg(WIIMOTEDEV_CONFIG_FILE));

    wiimotedevSettings->reload();
    wiiremoteSequence = wiimotedevSettings->getWiiremoteSequence();

    return !wiiremoteSequence.isEmpty();
}

void ConnectionManager::run()
{
    QTime time;
    while (!terminateReq)
    {
        WiimoteConnection *connection = new WiimoteConnection();

        time.start();
        if (connection->connectAny() && !terminateReq) {
            registerConnection(static_cast< void*>( connection));
            continue;
        }

        delete connection;

        if (terminateReq)
            msleep((time.elapsed() < 100) ? 3000 : 0);
    }

    syslog(QString("pepare to shutdown"));
    syslog(QString("active connections count = %1").arg(QString::number(objectList.count())));

    for (register int i = 0; i < objectList.count(); ++i)
    {
        register WiimoteConnection* connection = static_cast< WiimoteConnection*>( objectList.at(i));
        disconnect(connection, 0, 0, 0);
        connection->disconnectFromDevice();
        connection->wait();
        syslog(QString("wiiremote %1 disconnected").arg(connection->getWiimoteSAddr()));
        delete connection;
    }

    objectList.clear();

    syslog(QString("leaving main thread"));
}

void ConnectionManager::registerConnection(void *object)
{
    WiimoteConnection *connection = static_cast< WiimoteConnection*>( object);

    QString macaddr = connection->getWiimoteSAddr();
    
    connection->setWiimoteSequence(wiiremoteSequence.value(macaddr, 0));
    if (connection->getWiimoteSequence())
        connection->setLedStatus(connection->getWiimoteSequence());

    syslog(QString("Established connection with wiiremote, ID %1 MAC %2").arg(QString::number(connection->getWiimoteSequence(), 10), macaddr));
    if (!connection->getWiimoteSequence())
    {
        syslog(QString("Wiiremote MAC %1 is unregistred, run wiimotedev-register and reconnect device").arg(macaddr));

        connection->setLedStatus(0x0F);

        bool exist = false;

        for (register int i = 0; i < unregisterWiiremoteList.count(); ++i)
            if (unregisterWiiremoteList.at(i) == macaddr) {
                exist = true;
                break;
            }

        if (!exist) unregisterWiiremoteList << macaddr;

        connection->disconnectFromDevice();
        connection->wait();
        delete connection;

        emit dbusReportUnregistredWiiremote(macaddr);
        return;
    }

    objectList << object;

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

    syslog(QString("Wiiremote disconnected, ID %1; MAC %2").arg(QString::number(connection->getWiimoteSequence(), 10), connection->getWiimoteSAddr()));

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
