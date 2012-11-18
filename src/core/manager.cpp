/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
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
#include <QFile>
#include <QTime>

#include "core/manager.h"
#include "syslog/syslog.h"
#include "core/wiiremote.h"

ConnectionManager::ConnectionManager(QObject *parent):
  QThread(parent),
  dbusDeviceEventsAdaptor(0),
  dbusServiceAdaptor(0),
  terminateReq(false),
  mutex(new QMutex()),
  result(EXIT_SUCCESS)
{
  if (!QFile::exists(WIIMOTEDEV_CONFIG_FILE)) {
    systemlog::critical(QString("missing configuration file %1").arg(WIIMOTEDEV_CONFIG_FILE));
    result = EXIT_FAILURE;
    return;
  }

  systemlog::notice(QString("loading rules from %1").arg(WIIMOTEDEV_CONFIG_FILE));
  settings = new WiimotedevSettings(WIIMOTEDEV_CONFIG_FILE, this);

  sequence = settings->getWiiremoteSequence();

  QDBusConnection connection = QDBusConnection::systemBus();

  dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
  dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
  bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

  connect(this, SIGNAL(dbusReportUnregistredWiimote(QString)), dbusDeviceEventsAdaptor, SIGNAL(dbusReportUnregistredWiimote(QString)));

  if (!(dbusDeviceEventsAdaptor->isRegistred() &&
        dbusServiceAdaptor->isRegistred() && registred)) {
    systemlog::critical("dbus registration fail");
    result = EXIT_FAILURE;
    return;
  }
}

ConnectionManager::~ConnectionManager() {
  delete mutex;
}

bool ConnectionManager::getTerminateRequest() {
  bool value = terminateReq;
  return value;
}

void ConnectionManager::setTerminateRequest(bool value) {
  terminateReq = value;
}

#include "service/wiimotemessagethread.h"
#include <QDebug>

void ConnectionManager::run() {
  if (result == EXIT_FAILURE)
    return;

  WiimoteDevice *dev = new WiimoteDevice();
  QTime clock;

  mutex->lock();
  while (!getTerminateRequest()) {
    clock.start();
    if (dev->connectToDevice(1)) {
      quint32 id = sequence.value(dev->getWiimoteSAddr(), 0);

      if (!id) {
        id = settings->registerWiiremote(dev->getWiimoteSAddr());
        systemlog::information(QString("note: wiiremote %1 registred, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));
      }

      systemlog::information(QString("wiiremote %1 connected, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));

      WiimoteMessageThread *thread = new WiimoteMessageThread(dev, id);
      thread->setPowerSafeTimeout(settings->getPowerSaveValue()*60000);
      connect(thread, SIGNAL(dbusVirtualCursorPosition(quint32, double, double, double, double)), dbusDeviceEventsAdaptor, SIGNAL(dbusVirtualCursorPosition(quint32,double, double, double, double)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusVirtualCursorFound(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusVirtualCursorFound(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusVirtualCursorLost(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusVirtualCursorLost(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteConnected(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteConnected(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteDisconnected(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteDisconnected(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteButtons(quint32,quint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukPlugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukPlugged(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukUnplugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukUnplugged(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukButtons(quint32,quint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerPlugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerPlugged(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerUnplugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerUnplugged(quint32)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(finished()), this , SLOT(wiimoteMessageThreadFinished()), Qt::QueuedConnection);
      threads.insert(1, thread);
      thread->start();
      dev = new WiimoteDevice();
    }

    if (clock.elapsed() < 100 && !getTerminateRequest())
      mutex->tryLock(ConnectionManager::WaitForBluetooth);
  }

  dev->disconnectFromDevice();
  delete dev;

  foreach (WiimoteMessageThread *thread, threads.values()) {
    thread->setThreadQuitState(true);
    thread->wait();
    delete thread;
  }

  threads.clear();

  QCoreApplication::quit();
}

void ConnectionManager::wiimoteMessageThreadFinished() {
  WiimoteMessageThread *thread = reinterpret_cast< WiimoteMessageThread*>(sender());
  threads.remove(thread->id());
  systemlog::information(QString("wiiremote %1 disconnected").arg(QString::number(thread->id())));
  delete thread;
}

bool ConnectionManager::dbusReloadSequenceList() {
  systemlog::notice(QString("loading sequences from %1").arg(WIIMOTEDEV_CONFIG_FILE));

  settings->reload();
  sequence = settings->getWiiremoteSequence();

  foreach(const QString &key, sequence.keys())
    unregisterWiimoteList[key] = false;

  return true;
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
  foreach (WiimoteMessageThread *thread, threads.values())
    list << thread->id();

  return list;
}

QList< uint> ConnectionManager::dbusNunchukGetAccelerometrCalibration(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusNunchukGetAccelerometrCalibration();
  return QList < uint>();
}

QList< uint> ConnectionManager::dbusWiimoteGetAccelerometrCalibration(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetAccelerometrCalibration();
  return QList < uint>();
}

quint8 ConnectionManager::dbusWiimoteGetStatus(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetStatus();
  return 0;
}


quint8 ConnectionManager::dbusWiimoteGetLedStatus(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetLedStatus();
  return 0;
}

bool ConnectionManager::dbusWiimoteGetRumbleStatus(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetRumbleStatus();
  return false;
}

bool ConnectionManager::dbusWiimoteSetLedStatus(quint32 id, quint32 status)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteSetLedStatus(status);
  return false;
}

bool ConnectionManager::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteSetRumbleStatus(status);
  return false;
}

quint32 ConnectionManager::dbusWiimoteGetCurrentLatency(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetCurrentLatency();
  return 0;
}

quint32 ConnectionManager::dbusWiimoteGetAverageLatency(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetCurrentLatency();
  return 0;
}

QString ConnectionManager::dbusWiimoteGetMacAddress(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetMacAddress();
  return QString();
}

bool ConnectionManager::dbusIsClassicConnected(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusIsClassicConnected();
  return false;
}

bool ConnectionManager::dbusIsNunchukConnected(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusIsNunchukConnected();
  return false;
}

bool ConnectionManager::dbusIsWiimoteConnected(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusIsWiimoteConnected();
  return false;
}

quint32 ConnectionManager::dbusWiimoteGetBatteryLife(quint32 id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetBatteryLife();
  return false;
}
