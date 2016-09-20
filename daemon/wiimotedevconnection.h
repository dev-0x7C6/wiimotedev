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

#include <QThread>
#include <atomic>
#include <memory>

#include "static/libcwiid/cwiid.h"
#include "virtualcursor/virtualcursor.h"
#include "linux/usr/include/wiimotedev/consts.h"

class QElapsedTimer;
class WiimotedevDevice;

const uint8_t classicLToleranceValue = (0x3F >> 2);
const uint8_t classicRToleranceValue = (0x1F >> 2);
const uint8_t classicLStickMaxX = (0x3F >> 1) + classicLToleranceValue;
const uint8_t classicLStickMinX = (0x3F >> 1) - classicLToleranceValue;
const uint8_t classicLStickMaxY = (0x3F >> 1) + classicLToleranceValue;
const uint8_t classicLStickMinY = (0x3F >> 1) - classicLToleranceValue;
const uint8_t classicRStickMaxX = (0x1F >> 1) + classicRToleranceValue;
const uint8_t classicRStickMinX = (0x1F >> 1) - classicRToleranceValue;
const uint8_t classicRStickMaxY = (0x1F >> 1) + classicRToleranceValue;
const uint8_t classicRStickMinY = (0x1F >> 1) - classicRToleranceValue;
const uint8_t nunchukToleranceValue = (0xFF >> 2);
const uint8_t nunchukStickMaxX = (0xFF >> 1) + nunchukToleranceValue;
const uint8_t nunchukStickMinX = (0xFF >> 1) - nunchukToleranceValue;
const uint8_t nunchukStickMaxY = (0xFF >> 1) + nunchukToleranceValue;
const uint8_t nunchukStickMinY = (0xFF >> 1) - nunchukToleranceValue;

class WiimotedevConnection : public QThread {
	Q_OBJECT
private:
	std::unique_ptr<WiimotedevDevice> m_device;
	std::atomic<bool> m_interrupted;
	std::atomic<double> m_batteryLife;
	std::atomic<uint64_t> m_powerSaveTimeout;

	enum DeviceType {
		ix_wiimote_device = 0,
		ix_nunchuk_device,
		ix_classic_device,
		ix_general_device,
		ix_all_devices
	};

	enum StickIndex {
		ix_classic_lstick = 0,
		ix_classic_rstick,
		ix_nunchuk_stick,
		ix_all_sticks
	};

	enum AxisIndex {
		ix_x_axis = 0,
		ix_y_axis
	};

	enum StateIndex {
		ix_max = 0,
		ix_min
	};

	std::unique_ptr<VirtualCursor> m_virtualCursor;

	QList<double> wfXmotion;
	QList<double> nfXmotion;
	QList<double> wfYmotion;
	QList<double> nfYmotion;
	QList<double> wfZmotion;
	QList<double> nfZmotion;
	double wcounter, ncounter;

	struct stickdata stick[ix_all_sticks];
	uint64_t cstate[ix_all_devices];
	uint64_t lstate[ix_all_devices];
	std::atomic<bool> m_available[ix_general_device];

	acc_cal calibration[ix_all_devices - 1];
	accdata acc[ix_all_devices - 1];

	int m_id;
	struct acc_cal nunchukAcc;
	struct acc_cal wiimoteAcc;

	QList<struct irpoint> current_ir_table;
	QList<struct irpoint> last_ir_table;

	std::atomic<uint32_t> m_currentLatency;
	std::atomic<uint32_t> m_averageLatency;
	double m_bufferLatency;
	int32_t m_bufferCounter;

	bool m_virtualCursorVisible;

private:
	void cwiid_process_wiimote_init();
	void cwiid_process_wiimote_done();
	void cwiid_process_wiimote_clear();

	void cwiid_process_wiimote_buttons(uint16_t buttons);
	void cwiid_process_wiimote_acc(uint8_t cwiid_acc[3]);
	void cwiid_process_wiimote_ir(cwiid_ir_src ir[4]);
	void cwiid_process_wiimote_status(uint8_t battery);
	void cwiid_process_wiimote_error();

	void cwiid_process_classic_init();
	void cwiid_process_classic_done();
	void cwiid_process_classic_clear();

	void cwiid_process_classic_lstick(uint8_t cwiid_stick[2]);
	void cwiid_process_classic_rstick(uint8_t cwiid_stick[2]);
	void cwiid_process_classic_buttons(uint16_t cwiid_buttons);
	void cwiid_process_classic_status(cwiid_ext_type type);

	void cwiid_process_nunchuk_init();
	void cwiid_process_nunchuk_done();
	void cwiid_process_nunchuk_clear();

	void cwiid_process_nunchuk_buttons(uint8_t cwiid_buttons);
	void cwiid_process_nunchuk_stick(uint8_t cwiid_stick[2]);
	void cwiid_process_nunchuk_acc(uint8_t cwiid_acc[3]);
	void cwiid_process_nunchuk_status(cwiid_ext_type type);

	void cwiid_process_motionplus_status(cwiid_ext_type type);
	void cwiid_process_motionplus(uint16_t angle[], uint8_t low_speed[]);

	double calcVirtualCursorDiff(double c1[], double c2[]);
	void calcAccelerometerValues(uint8_t acc[3], acc_cal &cal, accdata &out);

	void connect_animation();
	void disconnect_animation();

public:
	explicit WiimotedevConnection(WiimotedevDevice *device, int id, QObject *parent = 0);
	~WiimotedevConnection();

	uint32_t id() const;

	void interrupt();

	void setPowerSafeTimeout(int64_t timeout);
	int64_t powerSafeTimeout();

public:
	bool dbusIsClassicConnected();
	bool dbusIsNunchukConnected();
	bool dbusIsWiimoteConnected();

	QList<uint> dbusNunchukGetAccelerometrCalibration();
	QList<uint> dbusWiimoteGetAccelerometrCalibration();

	uint32_t dbusWiimoteGetAverageLatency();
	uint32_t dbusWiimoteGetBatteryLife();
	uint32_t dbusWiimoteGetCurrentLatency();
	QString dbusWiimoteGetMacAddress();

	uint8_t dbusWiimoteGetLedStatus();

	bool dbusWiimoteGetRumbleStatus();
	bool dbusWiimoteSetLedStatus(uint32_t status);
	bool dbusWiimoteSetRumbleStatus(bool status);
	uint8_t dbusWiimoteGetStatus();

protected:
	void run();

signals:
	void dbusVirtualCursorPosition(uint32_t, double, double, double, double);
	void dbusVirtualCursorFound(uint32_t);
	void dbusVirtualCursorLost(uint32_t);
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);
	void dbusWiimoteConnected(uint32_t);
	void dbusWiimoteDisconnected(uint32_t);
	void dbusWiimoteBatteryLife(uint32_t, uint8_t);
	void dbusWiimoteButtons(uint32_t, uint64_t);
	void dbusWiimoteInfrared(uint32_t, QList<struct irpoint>);
	void dbusWiimoteAcc(uint32_t, struct accdata);
	void dbusNunchukPlugged(uint32_t);
	void dbusNunchukUnplugged(uint32_t);
	void dbusNunchukButtons(uint32_t, uint64_t);
	void dbusNunchukStick(uint32_t, struct stickdata);
	void dbusNunchukAcc(uint32_t, struct accdata);
	void dbusClassicControllerPlugged(uint32_t);
	void dbusClassicControllerUnplugged(uint32_t);
	void dbusClassicControllerButtons(uint32_t, uint64_t);
	void dbusClassicControllerLStick(uint32_t, struct stickdata);
	void dbusClassicControllerRStick(uint32_t, struct stickdata);
};
