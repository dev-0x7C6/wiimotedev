/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2015  Bartłomiej Burdukiewicz                               *
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
#include <QElapsedTimer>
#include <QFile>

#include "syslog/syslog.h"
#include "wiimotedevconnection.h"
#include "wiimotedevcore.h"
#include "wiimotedevdevice.h"

#include <sched.h>

WiimotedevCore::WiimotedevCore(QObject *parent)
		: QThread(parent)
		, WiimotedevDBusEvents(0)
		, dbusServiceAdaptor(0)
		, m_interrupted(false)
		, result(EXIT_SUCCESS) {
	if (!QFile::exists(WIIMOTEDEV_SETTINGS_FILE)) {
		systemlog::critical(QString("missing configuration file %1").arg(WIIMOTEDEV_SETTINGS_FILE));
		result = EXIT_FAILURE;
		return;
	}

	systemlog::notice(QString("config: %1").arg(WIIMOTEDEV_SETTINGS_FILE));
	settings = new WiimotedevSettings(this);
	sequence = settings->connectionTable();
	QDBusConnection connection = QDBusConnection::systemBus();
	WiimotedevDBusEvents = new WiimotedevDBusEventsWrapper(this, connection);
	dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
	bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

	if (!(WiimotedevDBusEvents->isRegistred() &&
			dbusServiceAdaptor->isRegistred() && registred)) {
		systemlog::critical("cannot register dbus service, quit...");
		result = EXIT_FAILURE;
		return;
	}
}

WiimotedevCore::~WiimotedevCore() {
}

void WiimotedevCore::interrupt() {
	m_interrupted = true;
}

void WiimotedevCore::run() {
	if (result == EXIT_FAILURE) {
		QCoreApplication::quit();
		return;
	}

	WiimotedevDevice *dev = new WiimotedevDevice();
	QElapsedTimer m_bluetoothInertia;
	QMutex m_bluetoothBlocking;
	uint id = 0;
	m_bluetoothBlocking.lock();

	while (!m_interrupted) {
		m_bluetoothInertia.start();

		if (dev->connectToDevice(1)) {
			id = sequence.value(dev->getWiimoteSAddr(), 0);

			if (!id)
				if ((id = settings->registerWiiremote(dev->getWiimoteSAddr()))) {
					systemlog::information(QString("note: wiiremote %1 registred, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));
					sequence = settings->connectionTable();
				}

			systemlog::information(QString("wiiremote %1 connected, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));
			WiimotedevConnection *thread = new WiimotedevConnection(dev, id);
			thread->setPowerSafeTimeout(settings->powerSaveTiemout() * 60000);
			connect(thread, &WiimotedevConnection::dbusVirtualCursorPosition, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusVirtualCursorPosition, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusVirtualCursorFound, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusVirtualCursorFound, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusVirtualCursorLost, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusVirtualCursorLost, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteGeneralButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteGeneralButtons, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteConnected, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteConnected, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteDisconnected, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteDisconnected, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteBatteryLife, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteBatteryLife, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteButtons, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteInfrared, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteInfrared, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteAcc, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteAcc, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusNunchukPlugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukPlugged, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusNunchukUnplugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukUnplugged, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusNunchukStick, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukStick, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusNunchukButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukButtons, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusNunchukAcc, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukAcc, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusClassicControllerPlugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerPlugged, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusClassicControllerUnplugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerUnplugged, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusClassicControllerButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerButtons, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusClassicControllerLStick, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerLStick, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusClassicControllerRStick, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerRStick, Qt::QueuedConnection);
			connect(thread, &WiimotedevConnection::dbusWiimoteDisconnected, this, &WiimotedevCore::dbusWiimoteDisconnected, Qt::QueuedConnection);
			threads.insert(id, thread);
			thread->start();
			dev = new WiimotedevDevice();
		}

		if (m_bluetoothInertia.elapsed() < WiimotedevCore::BluetoothFlood && !m_interrupted) {
			for (int i = 0; i < 100; ++i) {
				m_bluetoothBlocking.tryLock(WiimotedevCore::WaitForBluetooth / 100);

				if (m_interrupted)
					break;
			}
		}

		sched_yield();
	}

	m_bluetoothBlocking.unlock();
	dev->disconnectFromDevice();
	delete dev;
	foreach (WiimotedevConnection *thread, threads.values()) {
		systemlog::information(QString("wiiremote %1 disconnecting..., id %2").arg(thread->dbusWiimoteGetMacAddress(), QString::number(id)));
		thread->interrupt();
		thread->wait();
		delete thread;
	}
	threads.clear();
	QCoreApplication::quit();
}

void WiimotedevCore::dbusWiimoteDisconnected(uint id) {
	WiimotedevConnection *thread = threads.value(id, 0);

	if (!thread)
		return;

	QString addr = thread->dbusWiimoteGetMacAddress();
	thread->interrupt();
	thread->wait();
	delete thread;
	threads.remove(id);
	systemlog::information(QString("wiiremote %1 disconnected., id %2").arg(addr, QString::number(id)));
}

bool WiimotedevCore::dbusReloadSequenceList() {
	systemlog::notice(QString("loading sequences from %1").arg(WIIMOTEDEV_SETTINGS_FILE));
	settings->reload();
	sequence = settings->connectionTable();
	return true;
}

QList<uint> WiimotedevCore::dbusGetWiimoteList() {
	QList<uint> list;

	for (const WiimotedevConnection *thread : threads.values())
		list << thread->id();

	qSort(list);
	return list;
}

QList<uint> WiimotedevCore::dbusNunchukGetAccelerometrCalibration(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusNunchukGetAccelerometrCalibration();

	return {};
}

QList<uint> WiimotedevCore::dbusWiimoteGetAccelerometrCalibration(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetAccelerometrCalibration();

	return {};
}

uint8 WiimotedevCore::dbusWiimoteGetStatus(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetStatus();

	return 0;
}

uint8 WiimotedevCore::dbusWiimoteGetLedStatus(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetLedStatus();

	return 0;
}

bool WiimotedevCore::dbusWiimoteGetRumbleStatus(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetRumbleStatus();

	return false;
}

bool WiimotedevCore::dbusWiimoteSetLedStatus(uint id, uint status) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteSetLedStatus(status);

	return false;
}

bool WiimotedevCore::dbusWiimoteSetRumbleStatus(uint id, bool status) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteSetRumbleStatus(status);

	return false;
}

uint WiimotedevCore::dbusWiimoteGetCurrentLatency(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetCurrentLatency();

	return 0;
}

uint WiimotedevCore::dbusWiimoteGetAverageLatency(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetCurrentLatency();

	return 0;
}

QString WiimotedevCore::dbusWiimoteGetMacAddress(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetMacAddress();

	return QString();
}

bool WiimotedevCore::dbusIsClassicConnected(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusIsClassicConnected();

	return false;
}

bool WiimotedevCore::dbusIsNunchukConnected(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusIsNunchukConnected();

	return false;
}

bool WiimotedevCore::dbusIsWiimoteConnected(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusIsWiimoteConnected();

	return false;
}

uint WiimotedevCore::dbusWiimoteGetBatteryLife(uint id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetBatteryLife();

	return false;
}
