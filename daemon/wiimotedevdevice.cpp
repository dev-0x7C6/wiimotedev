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

#include "wiimotedevdevice.h"
#include <QMutexLocker>

WiimotedevDevice::WiimotedevDevice(QObject *parent)
		: QObject(parent)
		, device(0)
		, haveWiimoteCallibration(false)
		, haveNunchukCallibration(false)
		, isRumble(false)
		, switchOnLeds(0)
		, reportMode(0) {
	memset(&bdaddr, 0, sizeof(bdaddr_t));
	memset(&wiimote_acc_cal, 0, sizeof(acc_cal));
	memset(&nunchuk_acc_cal, 0, sizeof(acc_cal));
}

WiimotedevDevice::~WiimotedevDevice() {
	if (isConnected())
		disconnectFromDevice(true);
}

bool WiimotedevDevice::isConnected() {
	return (device != 0);
}

bool WiimotedevDevice::isDisconnected() {
	return (device == 0);
}

bool WiimotedevDevice::connectToDevice(const uint32_t timeout) {
	QMutexLocker locker(&m_mutex);
	memset(&bdaddr, 0, sizeof(bdaddr_t));

	if ((device = cwiid_open_timeout(&bdaddr, CWIID_FLAG_MESG_IFC, timeout))) {
		id = cwiid_get_id(device);
		return true;
	}

	device = 0;
	return false;
}

bool WiimotedevDevice::disconnectFromDevice(const bool switchOfReport) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected())
		return false;

	if (switchOfReport) cwiid_set_rpt_mode(device, 0);

	cwiid_disconnect(device);
	device = 0;
	isRumble = false;
	switchOnLeds = 0;
	reportMode = 0;
	haveWiimoteCallibration = false;
	haveNunchukCallibration = false;
	return true;
}

void WiimotedevDevice::requestStatus() {
	QMutexLocker locker(&m_mutex);
	cwiid_request_status(device);
}

bool WiimotedevDevice::fetchMessage(int *count, union cwiid_mesg *mesg[], timespec *time) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected())
		return false;

	if (cwiid_get_mesg(device, count, mesg, time)) {
		disconnectFromDevice(false);
		return false;
	}

	return true;
}

bool WiimotedevDevice::setLedStatus(uint8_t led) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected()) return false;

	cwiid_set_led(device, switchOnLeds = led);
	return true;
}

bool WiimotedevDevice::setRumbleStatus(bool rumble) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected()) return false;

	cwiid_set_rumble(device, isRumble = rumble);
	return true;
}

bool WiimotedevDevice::setReportMode(uint8_t mode) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected()) return false;

	if (cwiid_set_rpt_mode(device, reportMode = mode)) {
		disconnectFromDevice(false);
		return false;
	}

	return true;
}

uint8_t WiimotedevDevice::getLedStatus() {
	if (isDisconnected()) return 0;

	return switchOnLeds;
}

bool WiimotedevDevice::getRumbleStatus() {
	if (isDisconnected())
		return false;

	return isRumble;
}

uint8_t WiimotedevDevice::getReportMode() {
	if (isDisconnected())
		return 0;

	return reportMode;
}

QString WiimotedevDevice::getWiimoteSAddr() {
	char addr[17];
	ba2str(&bdaddr, addr);
	return QString::fromLocal8Bit(addr, 17);
}

bdaddr_t WiimotedevDevice::getWiimoteAddr() {
	return bdaddr;
}

bool WiimotedevDevice::requestCallibration(enum cwiid_ext_type ext_type, acc_cal *acc_cal) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected())
		return false;

	if (cwiid_get_acc_cal(device, ext_type, acc_cal))
		return false;

	switch (ext_type) {
		case CWIID_EXT_NONE:
			wiimote_acc_cal = *acc_cal;
			haveWiimoteCallibration = true;
			break;

		case CWIID_EXT_NUNCHUK:
			nunchuk_acc_cal = *acc_cal;
			haveWiimoteCallibration = true;
			break;

		default:
			break;
	}

	return true;
}

acc_cal WiimotedevDevice::fetchWiimoteCallibration(bool &valid) {
	QMutexLocker locker(&m_mutex);
	valid = haveWiimoteCallibration;
	return wiimote_acc_cal;
}

acc_cal WiimotedevDevice::fetchNunchukCallibration(bool &valid) {
	QMutexLocker locker(&m_mutex);
	valid = haveNunchukCallibration;
	return nunchuk_acc_cal;
}

bool WiimotedevDevice::getWiimoteState(struct cwiid_state &state) {
	QMutexLocker locker(&m_mutex);

	if (isDisconnected()) return false;

	if (cwiid_get_state(device, &state)) {
		disconnectFromDevice(false);
		return false;
	}

	return true;
}
