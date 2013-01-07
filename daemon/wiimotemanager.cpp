/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
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
#include <QElapsedTimer>

#include "wiimotemanager.h"
#include "syslog/syslog.h"
#include "wiimotedevice.h"
#include "wiimotemessagethread.h"

WiimoteManager::WiimoteManager(QObject *parent):
  QThread(parent),
  dbusDeviceEventsAdaptor(0),
  dbusServiceAdaptor(0),
  m_mutex(new QMutex()),
  m_threadQuitStatus(false),
  result(EXIT_SUCCESS)
{
  if (!QFile::exists(WIIMOTEDEV_CONFIG_FILE)) {
    systemlog::critical(QString("missing configuration file %1").arg(WIIMOTEDEV_CONFIG_FILE));
    result = EXIT_FAILURE;
    return;
  }

  systemlog::notice(QString("loading rules from %1").arg(WIIMOTEDEV_CONFIG_FILE));
  settings = new WiimotedevSettings(this);
  sequence = settings->connectionTable();

  QDBusConnection connection = QDBusConnection::systemBus();

  dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
  dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
  bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);


  if (!(dbusDeviceEventsAdaptor->isRegistred() &&
        dbusServiceAdaptor->isRegistred() && registred)) {
    systemlog::critical("dbus registration failed");
    result = EXIT_FAILURE;
    return;
  }
}

WiimoteManager::~WiimoteManager() {
  delete m_mutex;
}

void WiimoteManager::run() {
  if (result == EXIT_FAILURE) {
    QCoreApplication::quit();
    return;
  }

  WiimoteDevice *dev = new WiimoteDevice();
  QElapsedTimer m_bluetoothInertia;
  QMutex m_bluetoothBlocking;
  uint id  = 0;

  m_bluetoothBlocking.lock();
  while (!threadQuitStatus()) {
    m_bluetoothInertia.start();
    if (dev->connectToDevice(1)) {
      id = sequence.value(dev->getWiimoteSAddr(), 0);

      if (id)
      if (id = settings->registerWiiremote(dev->getWiimoteSAddr())) {
        systemlog::information(QString("note: wiiremote %1 registred, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));
        sequence = settings->connectionTable();
      }

      systemlog::information(QString("wiiremote %1 connected, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));

      WiimoteMessageThread *thread = new WiimoteMessageThread(dev, id);
      thread->setPowerSafeTimeout(settings->powerSaveTiemout() * 60000);
      connect(thread, SIGNAL(dbusVirtualCursorPosition(uint, double, double, double, double)), dbusDeviceEventsAdaptor, SIGNAL(dbusVirtualCursorPosition(uint,double, double, double, double)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusVirtualCursorFound(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusVirtualCursorFound(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusVirtualCursorLost(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusVirtualCursorLost(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteGeneralButtons(uint,uint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteGeneralButtons(uint,uint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteConnected(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteConnected(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteDisconnected(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteDisconnected(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteBatteryLife(uint,uint8)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteBatteryLife(uint,uint8)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteButtons(uint,uint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteButtons(uint,uint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteInfrared(uint,QList<struct irpoint>)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteInfrared(uint,QList< struct irpoint>)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteAcc(uint,struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteAcc(uint, struct accdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukPlugged(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukPlugged(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukUnplugged(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukUnplugged(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukStick(uint,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukStick(uint,struct stickdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukButtons(uint,uint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukButtons(uint,uint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusNunchukAcc(uint,struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukAcc(uint,struct accdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerPlugged(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerPlugged(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerUnplugged(uint)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerUnplugged(uint)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerButtons(uint,uint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerButtons(uint,uint64)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerLStick(uint,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerLStick(uint,struct stickdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusClassicControllerRStick(uint,struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerRStick(uint,struct stickdata)), Qt::QueuedConnection);
      connect(thread, SIGNAL(dbusWiimoteDisconnected(uint)), this, SLOT(dbusWiimoteDisconnected(uint)), Qt::QueuedConnection);
      threads.insert(id, thread);
      thread->start();
      dev = new WiimoteDevice();
    }

    if (m_bluetoothInertia.elapsed() < WiimoteManager::BluetoothFlood && !threadQuitStatus()) {
      for (register int i = 0; i < 100; ++i) {
        m_bluetoothBlocking.tryLock(WiimoteManager::WaitForBluetooth / 100);
        if (threadQuitStatus())
          break;
      }
    }
  }

  m_bluetoothBlocking.unlock();
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

void WiimoteManager::dbusWiimoteDisconnected(uint id) {
  threads[id]->setThreadQuitState(true);
  threads[id]->wait();
  delete threads[id];
  threads.remove(id);
  systemlog::information(QString("wiiremote %1 disconnected").arg(QString::number(id)));
}

void WiimoteManager::setThreadQuitStatus(bool quit) {
  QMutexLocker locker(m_mutex);
  m_threadQuitStatus = quit;
}

bool WiimoteManager::threadQuitStatus() {
  QMutexLocker locker(m_mutex);
  return m_threadQuitStatus;
}

bool WiimoteManager::dbusReloadSequenceList() {
  systemlog::notice(QString("loading sequences from %1").arg(WIIMOTEDEV_CONFIG_FILE));

  settings->reload();
  sequence = settings->connectionTable();

  return true;
}

QList < uint> WiimoteManager::dbusGetWiimoteList()
{
  QList < uint> list;
  foreach (WiimoteMessageThread *thread, threads.values())
    list << thread->id();

  return list;
}

QList< uint> WiimoteManager::dbusNunchukGetAccelerometrCalibration(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusNunchukGetAccelerometrCalibration();
  return QList < uint>();
}

QList< uint> WiimoteManager::dbusWiimoteGetAccelerometrCalibration(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetAccelerometrCalibration();
  return QList < uint>();
}

uint8 WiimoteManager::dbusWiimoteGetStatus(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetStatus();
  return 0;
}


uint8 WiimoteManager::dbusWiimoteGetLedStatus(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetLedStatus();
  return 0;
}

bool WiimoteManager::dbusWiimoteGetRumbleStatus(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetRumbleStatus();
  return false;
}

bool WiimoteManager::dbusWiimoteSetLedStatus(uint id, uint status)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteSetLedStatus(status);
  return false;
}

bool WiimoteManager::dbusWiimoteSetRumbleStatus(uint id, bool status)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteSetRumbleStatus(status);
  return false;
}

uint WiimoteManager::dbusWiimoteGetCurrentLatency(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetCurrentLatency();
  return 0;
}

uint WiimoteManager::dbusWiimoteGetAverageLatency(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetCurrentLatency();
  return 0;
}

QString WiimoteManager::dbusWiimoteGetMacAddress(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetMacAddress();
  return QString();
}

bool WiimoteManager::dbusIsClassicConnected(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusIsClassicConnected();
  return false;
}

bool WiimoteManager::dbusIsNunchukConnected(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusIsNunchukConnected();
  return false;
}

bool WiimoteManager::dbusIsWiimoteConnected(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusIsWiimoteConnected();
  return false;
}

uint WiimoteManager::dbusWiimoteGetBatteryLife(uint id)
{
  WiimoteMessageThread *thread = threads.value(id);
  if (thread)
    return threads.value(id)->dbusWiimoteGetBatteryLife();
  return false;
}
