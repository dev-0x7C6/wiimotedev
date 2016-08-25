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

#pragma once

#include <atomic>

#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "dbus/deviceevents.h"
#include "dbus/daemonservice.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "wiimotedevconnection.h"
#include "wiimotedevsettings.h"

class WiimotedevCore : public QThread {
	Q_OBJECT
private:
	// Adaptor section ------------------------------------------ /
	WiimotedevDBusEventsWrapper *WiimotedevDBusEvents;
	DBusServiceAdaptorWrapper *dbusServiceAdaptor;

	// Settings ------------------------------------------------- /
	WiimotedevSettings *settings;
	QHash<QString, uint> sequence;
	QMutex m_mutex;

	std::atomic<bool> m_interrupted;

public:
	WiimotedevCore(QObject *parent = 0);
	~WiimotedevCore();

	uint32_t result;

	void interrupt();

	static const int BluetoothFlood = 100;
	static const int WaitForBluetooth = 3000;

	QHash<uint32_t, WiimotedevConnection *> threads;

protected:
	void run();

private Q_SLOTS:
	void dbusWiimoteDisconnected(uint32_t);

public Q_SLOTS:
	bool dbusIsClassicConnected(uint32_t id);
	bool dbusIsNunchukConnected(uint32_t id);
	bool dbusIsWiimoteConnected(uint32_t id);
	QList<uint> dbusNunchukGetAccelerometrCalibration(uint32_t id);
	QList<uint> dbusWiimoteGetAccelerometrCalibration(uint32_t id);
	uint32_t dbusWiimoteGetAverageLatency(uint32_t id);
	uint32_t dbusWiimoteGetBatteryLife(uint32_t id);
	uint32_t dbusWiimoteGetCurrentLatency(uint32_t id);
	QString dbusWiimoteGetMacAddress(uint32_t id);
	bool dbusWiimoteGetRumbleStatus(uint32_t id);
	bool dbusWiimoteSetLedStatus(uint32_t id, uint32_t status);
	bool dbusWiimoteSetRumbleStatus(uint32_t id, bool status);
	uint8_t dbusWiimoteGetLedStatus(uint32_t id);
	uint8_t dbusWiimoteGetStatus(uint32_t id);
	QList<uint> dbusGetWiimoteList();
	bool dbusReloadSequenceList();

Q_SIGNALS:
	void dbusReportUnregistredWiimote(QString);
};
