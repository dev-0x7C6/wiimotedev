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

	uint result;

	void interrupt();

	static const int BluetoothFlood = 100;
	static const int WaitForBluetooth = 3000;

	QHash<uint, WiimotedevConnection *> threads;

protected:
	void run();

private Q_SLOTS:
	void dbusWiimoteDisconnected(uint);

public Q_SLOTS:
	bool dbusIsClassicConnected(uint id);
	bool dbusIsNunchukConnected(uint id);
	bool dbusIsWiimoteConnected(uint id);
	QList<uint> dbusNunchukGetAccelerometrCalibration(uint id);
	QList<uint> dbusWiimoteGetAccelerometrCalibration(uint id);
	uint dbusWiimoteGetAverageLatency(uint id);
	uint dbusWiimoteGetBatteryLife(uint id);
	uint dbusWiimoteGetCurrentLatency(uint id);
	QString dbusWiimoteGetMacAddress(uint id);
	bool dbusWiimoteGetRumbleStatus(uint id);
	bool dbusWiimoteSetLedStatus(uint id, uint status);
	bool dbusWiimoteSetRumbleStatus(uint id, bool status);
	uint8 dbusWiimoteGetLedStatus(uint id);
	uint8 dbusWiimoteGetStatus(uint id);
	QList<uint> dbusGetWiimoteList();
	bool dbusReloadSequenceList();

Q_SIGNALS:
	void dbusReportUnregistredWiimote(QString);
};
