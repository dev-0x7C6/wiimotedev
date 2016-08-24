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

#include "eiowiimotejoystick.h"

EIOWiimoteJoystick::EIOWiimoteJoystick(QString deviceName, int id, Mode mode, Position horizontal, QObject *parent)
		: QObject(parent)
		, InputDevice(deviceName.toStdString())
		, m_deviceName(deviceName)
		, m_horizontal(horizontal)
		, m_mode(mode)
		, m_home_pressed(false)
		, m_id(id)
		, m_dpad_invert_x(0x00)
		, m_dpad_invert_y(0x00)
		, m_home_switch_position(0x01)
		, m_report_buttons(0x01)
		, m_report_dstick(0x01)
		, m_report_pitch(0x01)
		, m_report_roll(0x01) {

	centerStick(EIOWiimoteJoystick::DpadStick);
	centerStick(EIOWiimoteJoystick::WiimoteAccelerometer);
	centerStick(EIOWiimoteJoystick::NunchukAccelerometer);
}

quint32 EIOWiimoteJoystick::assign() {
	return m_id;
}

void EIOWiimoteJoystick::setDStickInvertX(bool option) {
	m_dpad_invert_x = option;
}

void EIOWiimoteJoystick::setDStickInvertY(bool option) {
	m_dpad_invert_y = option;
}

void EIOWiimoteJoystick::setHomeSwitchPosition(bool option) {
	m_home_switch_position = option;
}

void EIOWiimoteJoystick::setReportButtons(bool report) {
	m_report_buttons = report;
}

void EIOWiimoteJoystick::setReportDStick(bool report) {
	m_report_dstick = report;
}

void EIOWiimoteJoystick::setReportPitch(bool report) {
	m_report_pitch = report;
}

void EIOWiimoteJoystick::setReportRoll(bool report) {
	m_report_roll = report;
}

void EIOWiimoteJoystick::setWiimoteButtons(uint64 buttons) {
	if (m_report_buttons) {
		sendEvent(EV_KEY, BTN_A, (buttons & WIIMOTE_BTN_A) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_B, (buttons & WIIMOTE_BTN_B) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_X, (buttons & WIIMOTE_BTN_1) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_Y, (buttons & WIIMOTE_BTN_2) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_START, (buttons & WIIMOTE_BTN_PLUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_SELECT, (buttons & WIIMOTE_BTN_MINUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_0, (buttons & WIIMOTE_BTN_RIGHT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_1, (buttons & WIIMOTE_BTN_LEFT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_2, (buttons & WIIMOTE_BTN_DOWN) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		sendEvent(EV_KEY, BTN_3, (buttons & WIIMOTE_BTN_UP) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

		if (!m_home_switch_position)
			sendEvent(EV_KEY, BTN_MODE, (buttons & WIIMOTE_BTN_HOME) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

		sendEventSync();
	}

	if (m_home_switch_position && m_report_buttons) {
		if (((buttons & WIIMOTE_BTN_HOME) != WIIMOTE_BTN_HOME) && (m_home_pressed == true))
			emit setLedState(m_id, m_id);

		if (((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME) && (m_home_pressed == false))
			switch (m_horizontal) {
				case EIOWiimoteJoystick::GamepadHorizontal:
					m_horizontal = EIOWiimoteJoystick::GamepadVertical;
					emit setLedState(m_id, 1 + 2);
					break;

				case EIOWiimoteJoystick::GamepadVertical:
					m_horizontal = EIOWiimoteJoystick::GamepadHorizontal;
					emit setLedState(m_id, 4 + 8);
					break;
			}
	}

	m_home_pressed = ((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME);
	m_last_dpad_x = 0x00;
	m_last_dpad_y = 0x00;

	if (m_report_dstick) {
		if (buttons & WIIMOTE_BTN_RIGHT)
			m_last_dpad_x = WIIMOTE_DPAD_MAX;
		else if (buttons & WIIMOTE_BTN_LEFT)
			m_last_dpad_x = WIIMOTE_DPAD_MIN;

		if (buttons & WIIMOTE_BTN_DOWN)
			m_last_dpad_y = WIIMOTE_DPAD_MAX;
		else if (buttons & WIIMOTE_BTN_UP)
			m_last_dpad_y = WIIMOTE_DPAD_MIN;

		if (m_dpad_invert_x) m_last_dpad_x *= -1;

		if (m_dpad_invert_y) m_last_dpad_y *= -1;
	}

	syncAxes();
}

void EIOWiimoteJoystick::centerStick(Stick id) {
	switch (id) {
		case EIOWiimoteJoystick::DpadStick:
			m_last_dpad_x = 0x00;
			m_last_dpad_y = 0x00;
			break;

		case EIOWiimoteJoystick::NunchukAccelerometer:
			m_last_nunchuk_acc_pitch = 0x00;
			m_last_nunchuk_acc_roll = 0x00;
			break;

		case EIOWiimoteJoystick::WiimoteAccelerometer:
			m_last_wiimote_acc_pitch = 0x00;
			m_last_wiimote_acc_roll = 0x00;
			break;

		default:
			break;
	}
}

void EIOWiimoteJoystick::syncAxes() {
	if (m_report_dstick) {
		switch (m_horizontal) {
			case EIOWiimoteJoystick::GamepadHorizontal:
				sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_y);
				sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_x * -0x01);
				break;

			case EIOWiimoteJoystick::GamepadVertical:
				sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_x);
				sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_y);
				break;
		}
	}

	if (m_report_pitch)
		sendEvent(EV_ABS, WIIMOTE_PITCH_LINUX_AXIS, m_last_wiimote_acc_pitch);

	if (m_report_roll)
		sendEvent(EV_ABS, WIIMOTE_ROLL_LINUX_AXIS, m_last_wiimote_acc_roll);

	sendEventSync();
}

bool EIOWiimoteJoystick::configure() {
	evbit(EV_KEY);
	evbit(EV_MSC);
	evbit(EV_ABS);

	if (m_report_buttons) {
		keybit(BTN_0);
		keybit(BTN_1);
		keybit(BTN_2);
		keybit(BTN_3);
		keybit(BTN_GAMEPAD);
		keybit(BTN_B);
		keybit(BTN_X);
		keybit(BTN_Y);
		keybit(BTN_SELECT);
		keybit(BTN_START);

		if (!m_home_switch_position)
			keybit(BTN_MODE);
	}

	if (m_report_dstick) {
		absbit(WIIMOTE_DPAD_LINUX_AXIS_X);
		absbit(WIIMOTE_DPAD_LINUX_AXIS_Y);
		range(WIIMOTE_DPAD_LINUX_AXIS_X, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
		range(WIIMOTE_DPAD_LINUX_AXIS_Y, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
	}

	if (m_report_pitch) {
		absbit(WIIMOTE_PITCH_LINUX_AXIS);
		range(WIIMOTE_PITCH_LINUX_AXIS, WIIMOTE_PITCH_MAX, WIIMOTE_PITCH_MIN);
	}

	if (m_report_roll) {
		absbit(WIIMOTE_ROLL_LINUX_AXIS);
		range(WIIMOTE_ROLL_LINUX_AXIS, WIIMOTE_ROLL_MAX, WIIMOTE_ROLL_MIN);
	}
}

void EIOWiimoteJoystick::setWiimoteAcc(double pitch, double roll) {
	m_last_wiimote_acc_pitch = pitch;
	m_last_wiimote_acc_roll = roll;
	syncAxes();
}
