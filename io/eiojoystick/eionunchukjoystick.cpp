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

#include "eionunchukjoystick.h"

EIONunchukJoystick::EIONunchukJoystick(QString deviceName, int id, QObject *parent)
		: QObject(parent)
		, InputDevice("Joystick")
		, m_deviceName(deviceName)
		, m_id(id)
		, m_stick_invert_x(0x00)
		, m_stick_invert_y(0x00)
		, m_report_buttons(0x01)
		, m_report_stick(0x01)
		, m_report_pitch(0x01)
		, m_report_roll(0x01)

{
}

uint32_t EIONunchukJoystick::assign() {
	return m_id;
}

void EIONunchukJoystick::setStickInvertX(bool option) {
	m_stick_invert_x = option;
}

void EIONunchukJoystick::setStickInvertY(bool option) {
	m_stick_invert_y = option;
}

void EIONunchukJoystick::setReportButtons(bool report) {
	m_report_buttons = report;
}

void EIONunchukJoystick::setReportStick(bool report) {
	m_report_stick = report;
}

void EIONunchukJoystick::setReportPitch(bool report) {
	m_report_pitch = report;
}

void EIONunchukJoystick::setReportRoll(bool report) {
	m_report_roll = report;
}

void EIONunchukJoystick::setNunchukButtons(uint64_t buttons) {
	if (!m_report_buttons)
		return;

	report(EV_KEY, BTN_A, (buttons & NUNCHUK_BTN_C) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
	report(EV_KEY, BTN_B, (buttons & NUNCHUK_BTN_Z) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
	sync();
}

void EIONunchukJoystick::centerStick(Stick id) {
	switch (id) {
		case EIONunchukJoystick::NunchukStick:
			m_last_stick_x = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
			m_last_stick_y = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
			break;

		case EIONunchukJoystick::DpadStick:
			m_last_dpad_x = 0x00;
			m_last_dpad_y = 0x00;
			break;

		case EIONunchukJoystick::NunchukAccelerometer:
			m_last_nunchuk_acc_pitch = 0x00;
			m_last_nunchuk_acc_roll = 0x00;
			break;

		case EIONunchukJoystick::WiimoteAccelerometer:
			m_last_wiimote_acc_pitch = 0x00;
			m_last_wiimote_acc_roll = 0x00;
			break;
	}
}

void EIONunchukJoystick::syncAxes() {
	if (m_report_stick) {
		report(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_X, m_last_stick_x);
		report(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_Y, m_last_stick_y);
	}

	if (m_report_pitch)
		report(EV_ABS, NUNCHUK_PITCH_LINUX_AXIS, m_last_nunchuk_acc_pitch);

	if (m_report_roll)
		report(EV_ABS, NUNCHUK_ROLL_LINUX_AXIS, m_last_nunchuk_acc_roll);

	sync();
}

bool EIONunchukJoystick::configure() {
	evbit(EV_KEY);
	evbit(EV_MSC);
	evbit(EV_ABS);

	if (m_report_buttons) {
		keybit(BTN_GAMEPAD);
		keybit(BTN_B);
	}

	if (m_report_stick) {
		absbit(NUNCHUK_STICK_LINUX_AXIS_X);
		absbit(NUNCHUK_STICK_LINUX_AXIS_Y);
		range(NUNCHUK_STICK_LINUX_AXIS_X, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
		range(NUNCHUK_STICK_LINUX_AXIS_Y, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
	}

	if (m_report_pitch) {
		absbit(NUNCHUK_PITCH_LINUX_AXIS);
		range(NUNCHUK_PITCH_LINUX_AXIS, NUNCHUK_PITCH_MAX, NUNCHUK_PITCH_MIN);
	}

	if (m_report_roll) {
		absbit(NUNCHUK_ROLL_LINUX_AXIS);
		range(NUNCHUK_ROLL_LINUX_AXIS, NUNCHUK_ROLL_MAX, NUNCHUK_ROLL_MIN);
	}
}

void EIONunchukJoystick::setNunchukStick(int32_t x, int32_t y) {
	m_last_stick_x = x;
	m_last_stick_y = y;
	syncAxes();
}

void EIONunchukJoystick::setNunchukAcc(double pitch, double roll) {
	m_last_nunchuk_acc_pitch = pitch;
	m_last_nunchuk_acc_roll = roll;
	syncAxes();
}
