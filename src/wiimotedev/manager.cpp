/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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
#include <QList>
#include <QTime>

#include "syslog/syslog.h"
#include "wiimotedev/manager.h"

extern bool additional_debug;

ConnectionManager::ConnectionManager(QObject *parent):
  QThread(parent),
  dbusDeviceEventsAdaptor(0),
  dbusServiceAdaptor(0),
  terminateReq(false)
{
// Setup ------------------------------------------------------------ /
  memset(&bdaddr_any, 0x00, sizeof(uint8_t) * 6);
  setTerminationEnabled(true);

  if (additional_debug)
    systemlog::notice(QString("loading rules from %1").arg(WIIMOTEDEV_CONFIG_FILE));

  wiimotedevSettings = new WiimotedevSettings(WIIMOTEDEV_CONFIG_FILE, this);
  sequence = wiimotedevSettings->getWiiremoteSequence();

  if (additional_debug) {
    systemlog::debug(QString("dbus interface - %1").arg(wiimotedevSettings->dbusInterfaceSupport() ? "enabled" : "disabled"));
    systemlog::debug(QString("tcp/ip interface - %1").arg(wiimotedevSettings->tcpInterfaceSupport() ? "enabled" : "disabled"));
  }

// DBus interface --------------------------------------------------- /

  if (wiimotedevSettings->dbusInterfaceSupport()) {
    QDBusConnection connection = QDBusConnection::systemBus();

    dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
    dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
    bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

    connect(this, SIGNAL(dbusReportUnregistredWiimote(QString)), dbusDeviceEventsAdaptor, SIGNAL(dbusReportUnregistredWiimote(QString)));

    if (additional_debug) {
      systemlog::debug(QString("register %1 object %2").arg(WIIMOTEDEV_DBUS_OBJECT_EVENTS, dbusDeviceEventsAdaptor->isRegistred() ? "done" : "failed"));
      systemlog::debug(QString("register %1 object %2").arg(WIIMOTEDEV_DBUS_OBJECT_SERVICE, dbusServiceAdaptor->isRegistred() ? "done" : "failed"));
      systemlog::debug(QString("register %1 service %2").arg(WIIMOTEDEV_DBUS_SERVICE_NAME, registred ? "done" : "failed"));
    }

    if (!(dbusDeviceEventsAdaptor->isRegistred() &&
          dbusServiceAdaptor->isRegistred() && registred))
      systemlog::error("can not register dbus service");
  }

// TCP interface ---------------------------------------------------- /

  if (wiimotedevSettings->tcpInterfaceSupport()) {
    tcpServerThread = new MessageServerThread(wiimotedevSettings, wiimotedevSettings->tcpGetPort(), this);
    tcpServerThread->start();
    if (additional_debug)
      systemlog::debug(QString("starting tcp/ip thread at 0x%1").arg(QString::number(tcpServerThread->thread()->currentThreadId(), 0x10)));
  }

  if (!(wiimotedevSettings->tcpInterfaceSupport() || wiimotedevSettings->dbusInterfaceSupport())) {
    systemlog::error("dbus/tcp disabled, stoping wiimotedev");
    terminateReq = true;
  }
}

bool ConnectionManager::dbusReloadSequenceList() {
  if (additional_debug)
    systemlog::notice(QString("loading sequences from %1").arg(WIIMOTEDEV_CONFIG_FILE));

  wiimotedevSettings->reload();
  sequence = wiimotedevSettings->getWiiremoteSequence();

  return !sequence.isEmpty();
}

void ConnectionManager::terminateRequest()
{
  terminateReq = true;
  connections.last()->wait();
}

void ConnectionManager::run() {
  QTime clock;

  connections << (new WiimoteConnection(dbusDeviceEventsAdaptor));

  while (!terminateReq) {
    clock.start();
    if (connections.last()->wiimote->connectToDevice(1)) {
      if (!registerConnection(connections.last())) {
        qDebug() << ":)";
        delete connections.last();
        connections.removeLast();
      }
      connections << (new WiimoteConnection(dbusDeviceEventsAdaptor));
      continue;
    }

    msleep((clock.elapsed() < 100 && !terminateReq) ? 1000 : 0);
  }

  if (additional_debug) {
    systemlog::notice("pepare to shutdown");
    systemlog::debug(QString("active connections count = %1").arg(QString::number(objectList.count())));
  }

  foreach (WiimoteConnection* connection, connections) {
    connection->quitThread();
    connection->wait();
    systemlog::information(QString("wiiremote %1 disconnected").arg(connection->wiimote->getWiimoteSAddr()));
    delete connection;
  }

  connections.clear();

  if (additional_debug)
    systemlog::debug(QString("leaving main thread %1").arg(QString::number(currentThreadId())));
}

bool ConnectionManager::registerConnection(WiimoteConnection *connection)
{
  QString addr = connection->wiimote->getWiimoteSAddr();
  quint32 id = sequence.value(addr, 0);

  if (!id) {
    connection->wiimote->setLedStatus(0x0f); // special blink, but better some animation will be better ?
    unregisterWiiremoteList[addr] = true;
    connection->wiimote->disconnectFromDevice();
    connection->wait();
    systemlog::information(QString("wiiremote %1 is unregistred, disconnected").arg(addr));
    return false;
  }

  unregisterWiiremoteList[addr] = false; // if rules are changed, this remove unregistred wiiremote from this list
  connection->wiimote->setLedStatus(id);
  connection->setWiimoteSequence(id);

  systemlog::information(QString("wiiremote %1 connected, id %2").arg(addr, QString::number(id)));

  if (wiimotedevSettings->tcpInterfaceSupport()) {
    connect(connection, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteConnected(quint32)), this, SIGNAL(dbusWiimoteConnected(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteDisconnected(quint32)), this, SIGNAL(dbusWiimoteDisconnected(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), this, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SIGNAL(dbusWiimoteButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteStatus(quint32,quint8)), this, SIGNAL(dbusWiimoteStatus(quint32,quint8)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), this, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), this, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukPlugged(quint32)), this, SIGNAL(dbusNunchukPlugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukUnplugged(quint32)), this, SIGNAL(dbusNunchukUnplugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), this, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SIGNAL(dbusNunchukButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), this, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerPlugged(quint32)), this, SIGNAL(dbusClassicControllerPlugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerUnplugged(quint32)), this, SIGNAL(dbusClassicControllerUnplugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), this, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), this, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::QueuedConnection);
  }

  connect(connection, SIGNAL(unregisterConnection(WiimoteConnection*)), this, SLOT(unregisterConnection(WiimoteConnection*)), Qt::QueuedConnection);
  connection->start();

  return true;
}

void ConnectionManager::unregisterConnection(WiimoteConnection *connection)
{
  systemlog::information(QString("wiiremote %1 disconnected, id %2").arg(connection->wiimote->getWiimoteSAddr(), QString::number(connection->getWiimoteSequence(), 10)));

  connection->wait();
  connections.removeAt(connections.indexOf(connection, 0));
  delete connection;

  qDebug() << connections.count();
}

QList < uint> ConnectionManager::dbusGetWiimoteList()
{
  QList < uint> list;
  for (register int i = 0; i < objectList.count(); ++i)
    list << static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence();
  return list;
}

QStringList ConnectionManager::dbusGetUnregistredWiimoteList()
{
  QStringList list;

  QMapIterator <QString, bool> map(unregisterWiiremoteList);

  while (map.hasNext()) {
    map.next();
    if (map.value()) list << map.key();
  }

  return list;
}

quint8 ConnectionManager::dbusWiimoteGetStatus(quint32 id)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection)
    return connection->getWiiremoteStatus();
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
    return connection->wiimote->getLedStatus();
  return 0;
}


bool ConnectionManager::dbusWiimoteGetRumbleStatus(quint32 id)
{
  WiimoteConnection *connection = findWiiremoteObject(id);

  if (connection)
    return connection->wiimote->getRumbleStatus();
  return false;
}

bool ConnectionManager::dbusWiimoteSetLedStatus(quint32 id, quint32 status)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection) {
    connection->wiimote->setLedStatus(status);
    return true;
  }
  return false;
}

bool ConnectionManager::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection) {
    connection->wiimote->setRumbleStatus(status);
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

bool ConnectionManager::dbusIsClassicConnected(quint32 id)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection)
    return connection->dbusIsClassicConnected();
  return false;
}

bool ConnectionManager::dbusIsNunchukConnected(quint32 id)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection)
    return connection->dbusIsNunchukConnected();
  return false;
}

bool ConnectionManager::dbusIsWiimoteConnected(quint32 id)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection)
    return connection->dbusIsWiimoteConnected();
  return false;
}

quint32 ConnectionManager::dbusWiimoteGetBatteryLife(quint32 id)
{
  WiimoteConnection *connection = findWiiremoteObject(id);
  if (connection)
    return connection->dbusWiimoteGetBatteryLife();
  return 0;
}
