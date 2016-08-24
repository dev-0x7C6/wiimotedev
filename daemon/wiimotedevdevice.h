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

#include "../config.h"

#include <atomic>
#include <QObject>
#include "static/libcwiid/cwiid.h"
#include "linux/usr/include/wiimotedev/consts.h"

#define defaultReportFlags     \
	CWIID_RPT_STATUS |         \
		CWIID_RPT_BTN |        \
		CWIID_RPT_IR |         \
		CWIID_RPT_NUNCHUK |    \
		CWIID_RPT_CLASSIC |    \
		CWIID_RPT_MOTIONPLUS | \
		CWIID_RPT_ACC

#include <QMutex>

class WiimotedevDevice : public QObject {
	Q_OBJECT
private:
	bdaddr_t bdaddr;
	std::atomic<cwiid_wiimote_t *> device;
	int32 id;
	std::atomic<bool> haveWiimoteCallibration;
	std::atomic<bool> haveNunchukCallibration;
	QMutex m_mutex;

private:
	std::atomic<bool> isRumble;
	std::atomic<uint8> switchOnLeds;
	std::atomic<uint8> reportMode;
	acc_cal wiimote_acc_cal;
	acc_cal nunchuk_acc_cal;

public:
	explicit WiimotedevDevice(QObject *parent = 0);
	virtual ~WiimotedevDevice();

	bool connectToDevice(const uint timeout = 3);
	bool disconnectFromDevice(const bool switchOfReport = true);

	bool fetchMessage(int *count, union cwiid_mesg *mesg[], timespec *time);

	bool isConnected();
	bool isDisconnected();

	bool setLedStatus(uint8 led);
	bool setRumbleStatus(bool rumble);
	bool setReportMode(uint8 mode = defaultReportFlags);

	void requestStatus();

	uint8 getLedStatus();
	bool getRumbleStatus();
	uint8 getReportMode();
	bool getWiimoteState(cwiid_state &state);

	bool requestCallibration(enum cwiid_ext_type ext_type, acc_cal *acc_cal);
	acc_cal fetchWiimoteCallibration(bool &valid);
	acc_cal fetchNunchukCallibration(bool &valid);

	QString getWiimoteSAddr();
	bdaddr_t getWiimoteAddr();
};
