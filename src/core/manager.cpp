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
#include <QCoreApplication>
#include <QList>
#include <QTime>

#include "syslog/syslog.h"
#include "manager.h"

extern bool additional_debug;

ConnectionManager::ConnectionManager(QObject *parent):
  QThread(parent),
  dbusDeviceEventsAdaptor(0),
  dbusServiceAdaptor(0),
  terminateReq(false)
{
  memset(&bdaddr_any, 0x00, sizeof(uint8_t) * 6);
  setTerminationEnabled(true);

  systemlog::notice(QString("loading rules from %1").arg(WIIMOTEDEV_CONFIG_FILE));

  wiimotedevSettings = new WiimotedevSettings(WIIMOTEDEV_CONFIG_FILE, this);
  sequence = wiimotedevSettings->getWiiremoteSequence();

  if (wiimotedevSettings->dbusInterfaceSupport()) {
    QDBusConnection connection = QDBusConnection::systemBus();

    dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
    dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
    bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

    connect(this, SIGNAL(dbusReportUnregistredWiimote(QString)), dbusDeviceEventsAdaptor, SIGNAL(dbusReportUnregistredWiimote(QString)));

    if (!(dbusDeviceEventsAdaptor->isRegistred() &&
          dbusServiceAdaptor->isRegistred() && registred))
      systemlog::error("can not register dbus service");
  }

  if (wiimotedevSettings->tcpInterfaceSupport()) {
    networkServerThread = new NetworkServerThread(wiimotedevSettings->tcpGetAllowedHostList(), wiimotedevSettings->tcpGetPort(), this);
    networkServerThread->start();
  }

  if (!(wiimotedevSettings->tcpInterfaceSupport() || wiimotedevSettings->dbusInterfaceSupport())) {
    systemlog::error("dbus/tcp disabled, stoping wiimotedev");
    terminateReq = true;
  }
}

void ConnectionManager::terminateRequest() {
  terminateReq = true;
}

void ConnectionManager::run() {
  connections << (new WiimoteConnection(1));
  QTime clock;
  while (!terminateReq) {
    clock.start();
    if (connections.last()->wiimote->connectToDevice(wiimotedevSettings->getPowerSaveValue())) {
      registerConnection(connections.last());
      connections << (new WiimoteConnection(wiimotedevSettings->getPowerSaveValue()));
      continue;
    }
    msleep((clock.elapsed() < 100 && !terminateReq) ? 1000 : 0);
  }

  connections.last()->wiimote->disconnectFromDevice();
  delete connections.last();
  connections.removeLast();

  freeAllConnections();

  if (wiimotedevSettings->tcpInterfaceSupport() && networkServerThread->isRunning()) {
    networkServerThread->quit();
    networkServerThread->wait();
  }

  if (wiimotedevSettings->tcpInterfaceSupport())
    delete networkServerThread;

  QCoreApplication::quit();
}

void ConnectionManager::freeConnection(WiimoteConnection *connection) {
  register int index = connections.indexOf(connection, 0);
  connection->quitThread();
  connection->wait();
  systemlog::information(QString("wiiremote %1 disconnected").arg(connection->wiimote->getWiimoteSAddr()));
  delete connection;

  connections.removeAt(index);
}

void ConnectionManager::freeAllConnections() {
  if (connections.isEmpty())
    return;

  foreach (WiimoteConnection* connection, connections) {
    connection->quitThread();
    connection->wait();
    systemlog::information(QString("wiiremote %1 disconnected").arg(connection->wiimote->getWiimoteSAddr()));
    delete connection;
  }

  connections.clear();
}

bool ConnectionManager::registerConnection(WiimoteConnection *connection)
{
  QString addr = connection->wiimote->getWiimoteSAddr();
  quint32 id = sequence.value(addr, 0);

  if (!id) {
    connection->wiimote->setLedStatus(0x0f);
    unregisterWiimoteList[addr] = true;
    emit dbusReportUnregistredWiimote(addr);
    freeConnection(connection);
    systemlog::information(QString("note: wiiremote %1 is unregistred, use wiimotedev-register to register new wiiremote").arg(addr));
    return false;
  }

  unregisterWiimoteList[addr] = false;
  connection->wiimote->setLedStatus(id);
  connection->setWiimoteSequence(id);

  systemlog::information(QString("wiiremote %1 connected, id %2").arg(addr, QString::number(id)));

  if (wiimotedevSettings->dbusInterfaceSupport()) {
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
  }

  if (wiimotedevSettings->tcpInterfaceSupport() && networkServerThread->isRunning()) {
    connect(connection, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), networkServerThread->server, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteConnected(quint32)), networkServerThread->server, SLOT(dbusWiimoteConnected(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteDisconnected(quint32)), networkServerThread->server, SLOT(dbusWiimoteDisconnected(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), networkServerThread->server, SLOT(dbusWiimoteBatteryLife(quint32,quint8)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteButtons(quint32,quint64)), networkServerThread->server, SLOT(dbusWiimoteButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteStatus(quint32,quint8)), networkServerThread->server, SLOT(dbusWiimoteStatus(quint32,quint8)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), networkServerThread->server, SLOT(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), networkServerThread->server, SLOT(dbusWiimoteAcc(quint32, struct accdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukPlugged(quint32)), networkServerThread->server, SLOT(dbusNunchukPlugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukUnplugged(quint32)), networkServerThread->server, SLOT(dbusNunchukUnplugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), networkServerThread->server, SLOT(dbusNunchukStick(quint32,struct stickdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukButtons(quint32,quint64)), networkServerThread->server, SLOT(dbusNunchukButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), networkServerThread->server, SLOT(dbusNunchukAcc(quint32,struct accdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerPlugged(quint32)), networkServerThread->server, SLOT(dbusClassicControllerPlugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerUnplugged(quint32)), networkServerThread->server, SLOT(dbusClassicControllerUnplugged(quint32)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), networkServerThread->server, SLOT(dbusClassicControllerButtons(quint32,quint64)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), networkServerThread->server, SLOT(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::QueuedConnection);
    connect(connection, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), networkServerThread->server, SLOT(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::QueuedConnection);
  }

  connect(connection, SIGNAL(unregisterConnection(WiimoteConnection*)), this, SLOT(freeConnection(WiimoteConnection*)), Qt::QueuedConnection);
  connection->start();

  return true;
}

bool ConnectionManager::dbusReloadSequenceList() {
  systemlog::notice(QString("loading sequences from %1").arg(WIIMOTEDEV_CONFIG_FILE));

  wiimotedevSettings->reload();
  sequence = wiimotedevSettings->getWiiremoteSequence();

  foreach(const QString &key, sequence.keys())
    unregisterWiimoteList[key] = false;

  return !sequence.isEmpty();
}

QStringList ConnectionManager::dbusGetUnregistredWiimoteList()
{
  QStringList list;

  QMapIterator <QString, bool> map(unregisterWiimoteList);

  while (map.hasNext()) {
    map.next();
    if (map.value()) list << map.key();
  }

  return list;
}


QList < uint> ConnectionManager::dbusGetWiimoteList()
{
  QList < uint> list;
  for (register int i = 0; i < objectList.count(); ++i)
    list << static_cast< WiimoteConnection*>( objectList.at(i))->getWiimoteSequence();
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
  foreach(WiimoteConnection *connection, connections)
    if (connection->getWiimoteSequence() == id) return connection;
  return 0;
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