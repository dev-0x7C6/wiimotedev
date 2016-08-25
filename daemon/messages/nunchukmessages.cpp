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

#include "wiimotedevconnection.h"
#include "wiimotedevdevice.h"
#include <QReadWriteLock>

void WiimotedevConnection::cwiid_process_nunchuk_init() {
	cwiid_process_nunchuk_clear();
}

void WiimotedevConnection::cwiid_process_nunchuk_done() {
	cwiid_process_nunchuk_clear();
}

void WiimotedevConnection::cwiid_process_nunchuk_clear() {
	cstate[ix_nunchuk_device] = 0x00;
	lstate[ix_nunchuk_device] = 0x00;
	stick[ix_nunchuk_stick].x = 0x00;
	stick[ix_nunchuk_stick].y = 0x00;
	m_available[ix_nunchuk_device] = false;
}

void WiimotedevConnection::cwiid_process_nunchuk_buttons(uint8_t cwiid_buttons) {
	cstate[ix_nunchuk_device] &= NUNCHUK_BUTTON_NOTMASK;

	if (cwiid_buttons & CWIID_NUNCHUK_BTN_C) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_C;

	if (cwiid_buttons & CWIID_NUNCHUK_BTN_Z) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_Z;
}

void WiimotedevConnection::cwiid_process_nunchuk_stick(uint8_t cwiid_stick[2]) {
	cstate[ix_nunchuk_device] &= NUNCHUK_STICK_NOTMASK;

	if (stick[ix_nunchuk_stick].x != cwiid_stick[ix_x_axis] ||
		stick[ix_nunchuk_stick].y != cwiid_stick[ix_y_axis]) {
		stick[ix_nunchuk_stick].x = cwiid_stick[ix_x_axis];
		stick[ix_nunchuk_stick].y = cwiid_stick[ix_y_axis];
		emit dbusNunchukStick(m_id, stick[ix_nunchuk_stick]);
	}

	if (stick[ix_nunchuk_stick].x > nunchukStickMaxX)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_RIGHT;
	else if (stick[ix_nunchuk_stick].x < nunchukStickMinX)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_LEFT;

	if (stick[ix_nunchuk_stick].y > nunchukStickMaxY)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_UP;
	else if (stick[ix_nunchuk_stick].y < nunchukStickMinY)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_DOWN;
}

void WiimotedevConnection::cwiid_process_nunchuk_acc(uint8_t cwiid_acc[3]) {
	cstate[ix_nunchuk_device] &= NUNCHUK_TILT_NOTMASK;
	cstate[ix_nunchuk_device] &= NUNCHUK_SHIFT_NOTMASK;
	calcAccelerometerValues(cwiid_acc, calibration[ix_nunchuk_device], acc[ix_nunchuk_device]);

	if (acc[ix_nunchuk_device].pitch < -45.0)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_FRONT;
	else if (acc[ix_nunchuk_device].pitch > 45.0)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_BACK;

	if (acc[ix_nunchuk_device].roll > 45.0)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_RIGHT;
	else if (acc[ix_nunchuk_device].roll < -45.0)
		cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_LEFT;

	emit dbusNunchukAcc(m_id, acc[ix_nunchuk_device]);
	double x = static_cast<double>((cwiid_acc[0] - calibration[ix_nunchuk_device].zero[0])) /
		static_cast<double>((calibration[ix_nunchuk_device].one[0] - calibration[ix_nunchuk_device].zero[0]));
	double y = static_cast<double>((cwiid_acc[1] - calibration[ix_nunchuk_device].zero[1])) /
		static_cast<double>((calibration[ix_nunchuk_device].one[1] - calibration[ix_nunchuk_device].zero[1]));
	double z = static_cast<double>((cwiid_acc[2] - calibration[ix_nunchuk_device].zero[2])) /
		static_cast<double>((calibration[ix_nunchuk_device].one[2] - calibration[ix_nunchuk_device].zero[2]));
	double vacc = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	if (vacc > 2.2) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_SHIFT_SHAKE;

	double nxpow = sqrt(pow(x, 2));
	double nypow = sqrt(pow(y, 2));
	double nzpow = sqrt(pow(z, 2));

	if (nxpow > 2.0 || nypow > 2.0 || nzpow > 2.0) {
		ncounter = 10;

		if (nxpow > 2.0) nfXmotion << x;

		if (nypow > 2.0) nfYmotion << y;

		if (nzpow > 2.0) nfZmotion << z;
	}

	if (ncounter == 0 && (nfXmotion.count() > 0 || nfYmotion.count() > 0 || nfZmotion.count() > 0)) {
		double nxvalue = (nfXmotion.count() > 0) ? nfXmotion.last() : 0.0;
		double nyvalue = (nfYmotion.count() > 0) ? nfYmotion.last() : 0.0;
		double nzvalue = (nfZmotion.count() > 0) ? nfZmotion.last() : 0.0;

		if (nfXmotion.count() > 1)
			for (int i = nfXmotion.count() - 1; i > 0; --i)
				if (nxvalue > 0)
					nxvalue += (nfXmotion.at(i - 1) > 0) ? nfXmotion.at(i - 1) : 0;
				else
					nxvalue += (nfXmotion.at(i - 1) < 0) ? nfXmotion.at(i - 1) : 0;

		if (nfYmotion.count() > 1)
			for (int i = nfYmotion.count() - 1; i > 0; --i)
				if (nyvalue > 0)
					nyvalue += (nfYmotion.at(i - 1) > 0) ? nfYmotion.at(i - 1) : 0;
				else
					nyvalue += (nfYmotion.at(i - 1) < 0) ? nfYmotion.at(i - 1) : 0;

		if (nfZmotion.count() > 1)
			for (int i = nfZmotion.count() - 1; i > 0; --i)
				if (nzvalue > 0)
					nzvalue += (nfZmotion.at(i - 1) > 0) ? nfZmotion.at(i - 1) : 0;
				else
					nzvalue += (nfZmotion.at(i - 1) < 0) ? nfZmotion.at(i - 1) : 0;

		nfXmotion.clear();
		nfYmotion.clear();
		nfZmotion.clear();
		double nxvalueabs = (nxvalue > 0) ? nxvalue : -nxvalue;
		double nyvalueabs = (nyvalue > 0) ? nyvalue : -nyvalue;
		double nzvalueabs = (nzvalue > 0) ? nzvalue : -nzvalue;

		if (nxvalueabs > 20)
			nyvalueabs -= 80;
		else if (nxvalueabs > 10)
			nyvalueabs -= 40;
		else if (nxvalueabs > 5)
			nyvalueabs -= 20;

		if (nyvalue < -4)
			nyvalueabs += 60;
		else if (nyvalue < -3)
			nyvalueabs += 40;
		else if (nyvalue < -2)
			nyvalueabs += 20;
		else if (nyvalue < -1)
			nyvalueabs += 10;

		if ((nxvalueabs > nyvalueabs) && (nxvalueabs > nzvalueabs))
			cstate[ix_nunchuk_device] |= ((nxvalue > 0) ? NUNCHUK_BTN_SHIFT_RIGHT : NUNCHUK_BTN_SHIFT_LEFT);
		else if ((nyvalueabs > nxvalueabs) && (nyvalueabs > nzvalueabs))
			cstate[ix_nunchuk_device] |= ((nyvalue > 0) ? NUNCHUK_BTN_SHIFT_FORWARD : NUNCHUK_BTN_SHIFT_BACKWARD);
	} else
		ncounter--;
}

void WiimotedevConnection::cwiid_process_nunchuk_status(cwiid_ext_type type) {
	switch (type) {
		case CWIID_EXT_NONE:
			if (m_available[ix_nunchuk_device]) {
				cwiid_process_nunchuk_clear();
				emit dbusNunchukUnplugged(m_id);
			}

			break;

		case CWIID_EXT_NUNCHUK:
			if (!m_available[ix_nunchuk_device]) {
				m_available[ix_nunchuk_device] = true;
				m_device->requestCallibration(CWIID_EXT_NUNCHUK, &calibration[ix_nunchuk_device]);
				emit dbusNunchukPlugged(m_id);
			}

			break;

		default:
			break;
	}
}
