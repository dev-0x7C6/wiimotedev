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

#include <QMutex>
#include <QMutexLocker>
#include <QThread>

#include "dbus/daemonservice.h"
#include "dbus/deviceevents.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "wiimotedevconnection.h"
#include "wiimotedevsettings.h"

class WiimotedevCore : public QThread {
	Q_OBJECT
public:
	WiimotedevCore(QObject *parent = 0);
	~WiimotedevCore();

	uint32_t result;

	void interrupt();

	static const int BluetoothFlood = 100;
	static const int WaitForBluetooth = 3000;

	QHash<uint32_t, WiimotedevConnection *> threads;

public slots:
	bool dbusIsClassicConnected(quint32 id);
	bool dbusIsNunchukConnected(quint32 id);
	bool dbusIsWiimoteConnected(quint32 id);
	QList<uint> dbusNunchukGetAccelerometrCalibration(quint32 id);
	QList<uint> dbusWiimoteGetAccelerometrCalibration(quint32 id);
	quint32 dbusWiimoteGetAverageLatency(quint32 id);
	quint32 dbusWiimoteGetBatteryLife(quint32 id);
	quint32 dbusWiimoteGetCurrentLatency(quint32 id);
	QString dbusWiimoteGetMacAddress(quint32 id);
	bool dbusWiimoteGetRumbleStatus(quint32 id);
	bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
	bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
	uint8_t dbusWiimoteGetLedStatus(quint32 id);
	uint8_t dbusWiimoteGetStatus(quint32 id);
	QList<uint> dbusGetWiimoteList();
	bool dbusReloadSequenceList();

protected:
	void run();

private slots:
	void dbusWiimoteDisconnected(quint32);

private:
	// Adaptor section ------------------------------------------ /
	WiimotedevDBusEventsWrapper *WiimotedevDBusEvents;
	DBusServiceAdaptorWrapper *dbusServiceAdaptor;

	// Settings ------------------------------------------------- /
	WiimotedevSettings *settings;
	QHash<QString, uint> sequence;
	QMutex m_mutex;

	std::atomic<bool> m_interrupted;

signals:
	void dbusReportUnregistredWiimote(QString);
};
