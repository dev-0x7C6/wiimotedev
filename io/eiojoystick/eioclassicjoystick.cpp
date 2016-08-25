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

#include "eioclassicjoystick.h"

EIOClassicJoystick::EIOClassicJoystick(QString deviceName, int id, QObject *parent)
		: QObject(parent)
		, InputDevice(deviceName.toStdString())
		, m_deviceName(deviceName)
		, m_id(id)
		, m_last_r_stick_x(0x00)
		, m_last_r_stick_y(0x00)
		, m_last_l_stick_x(0x00)
		, m_last_l_stick_y(0x00)
		, m_last_dpad_x(0x00)
		, m_last_dpad_y(0x00)
		, m_dpad_invert_x(0x00)
		, m_dpad_invert_y(0x00)
		, m_left_stick_invert_x(0x00)
		, m_left_stick_invert_y(0x00)
		, m_right_stick_invert_x(0x00)
		, m_right_stick_invert_y(0x00)
		, m_report_buttons(0x01)
		, m_report_dpad(0x01)
		, m_report_left_stick(0x01)
		, m_report_right_stick(0x01)

{
	if (m_deviceName.isEmpty())
		m_deviceName = QString::fromUtf8("Noname classic joystick");
	centerStick(EIOClassicJoystick::LeftStick);
	centerStick(EIOClassicJoystick::RightStick);
	centerStick(EIOClassicJoystick::DpadStick);
}

uint32_t EIOClassicJoystick::assign() {
	return m_id;
}

void EIOClassicJoystick::setDpadInvertX(bool option) {
	m_dpad_invert_x = option;
}

void EIOClassicJoystick::setDpadInvertY(bool option) {
	m_dpad_invert_y = option;
}

void EIOClassicJoystick::setLStickInvertX(bool option) {
	m_left_stick_invert_x = option;
}

void EIOClassicJoystick::setLStickInvertY(bool option) {
	m_left_stick_invert_y = option;
}

void EIOClassicJoystick::setRStickInvertX(bool option) {
	m_right_stick_invert_x = option;
}

void EIOClassicJoystick::setRStickInvertY(bool option) {
	m_left_stick_invert_y = option;
}

void EIOClassicJoystick::setReportButtons(bool report) {
	m_report_buttons = report;
}

void EIOClassicJoystick::setReportDStick(bool report) {
	m_report_dpad = report;
}

void EIOClassicJoystick::setReportLStick(bool report) {
	m_report_left_stick = report;
}

void EIOClassicJoystick::setReportRStick(bool report) {
	m_report_right_stick = report;
}

EIOClassicJoystick::~EIOClassicJoystick() {
}

void EIOClassicJoystick::setButtons(uint64_t buttons) {
	if (m_report_buttons) {
		report(EV_KEY, BTN_A, (buttons & CLASSIC_BTN_A) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_B, (buttons & CLASSIC_BTN_B) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_X, (buttons & CLASSIC_BTN_X) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_Y, (buttons & CLASSIC_BTN_Y) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_TL, (buttons & CLASSIC_BTN_L) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_TR, (buttons & CLASSIC_BTN_R) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_TL2, (buttons & CLASSIC_BTN_ZL) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_TR2, (buttons & CLASSIC_BTN_ZR) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_SELECT, (buttons & CLASSIC_BTN_MINUS) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_MODE, (buttons & CLASSIC_BTN_HOME) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_START, (buttons & CLASSIC_BTN_PLUS) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_0, (buttons & CLASSIC_BTN_RIGHT) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_1, (buttons & CLASSIC_BTN_LEFT) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_2, (buttons & CLASSIC_BTN_DOWN) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		report(EV_KEY, BTN_3, (buttons & CLASSIC_BTN_UP) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
		sync();
	}

	if (m_report_dpad) {
		centerStick(EIOClassicJoystick::DpadStick);

		if (buttons & CLASSIC_BTN_RIGHT)
			m_last_dpad_x = CLASSIC_DPAD_MAX;
		else if (buttons & CLASSIC_BTN_LEFT)
			m_last_dpad_x = CLASSIC_DPAD_MIN;

		if (buttons & CLASSIC_BTN_DOWN)
			m_last_dpad_y = CLASSIC_DPAD_MAX;
		else if (buttons & CLASSIC_BTN_UP)
			m_last_dpad_y = CLASSIC_DPAD_MIN;

		if (m_dpad_invert_x) m_last_dpad_x *= -0x01;

		if (m_dpad_invert_y) m_last_dpad_y *= -0x01;

		syncAxes();
	}
}

void EIOClassicJoystick::centerStick(Sticks stick) {
	switch (stick) {
		case EIOClassicJoystick::LeftStick:
			m_last_l_stick_x = (CLASSIC_LEFT_STICK_MIN + CLASSIC_LEFT_STICK_MAX) / 2;
			m_last_l_stick_y = (CLASSIC_LEFT_STICK_MIN + CLASSIC_LEFT_STICK_MAX) / 2;
			break;

		case EIOClassicJoystick::RightStick:
			m_last_r_stick_x = (CLASSIC_RIGHT_STICK_MIN + CLASSIC_RIGHT_STICK_MAX) / 2;
			m_last_r_stick_y = (CLASSIC_RIGHT_STICK_MIN + CLASSIC_RIGHT_STICK_MAX) / 2;
			break;

		case EIOClassicJoystick::DpadStick:
			m_last_dpad_x = 0;
			m_last_dpad_y = 0;
	}
}

void EIOClassicJoystick::setStick(Sticks stick, int32_t x, int32_t y) {
	switch (stick) {
		case EIOClassicJoystick::LeftStick:
			if (m_left_stick_invert_x) x = 0x41 - x;

			if (!m_left_stick_invert_y) y = 0x41 - y;

			if (CLASSIC_LEFT_STICK_MAX < x)
				x = CLASSIC_LEFT_STICK_MAX;
			else if (CLASSIC_LEFT_STICK_MIN > x)
				x = CLASSIC_LEFT_STICK_MIN;

			if (CLASSIC_LEFT_STICK_MAX < y)
				y = CLASSIC_LEFT_STICK_MAX;
			else if (CLASSIC_LEFT_STICK_MIN > y)
				y = CLASSIC_LEFT_STICK_MIN;

			m_last_l_stick_x = x;
			m_last_l_stick_y = y;
			break;

		case EIOClassicJoystick::RightStick:
			if (m_left_stick_invert_x) x = 0x1F - x;

			if (!m_left_stick_invert_y) y = 0x1F - y;

			if (CLASSIC_RIGHT_STICK_MAX < x)
				x = CLASSIC_RIGHT_STICK_MAX;
			else if (CLASSIC_RIGHT_STICK_MIN > x)
				x = CLASSIC_RIGHT_STICK_MIN;

			if (CLASSIC_RIGHT_STICK_MAX < y)
				y = CLASSIC_RIGHT_STICK_MAX;
			else if (CLASSIC_RIGHT_STICK_MIN > y)
				y = CLASSIC_RIGHT_STICK_MIN;

			m_last_r_stick_x = x;
			m_last_r_stick_y = y;
			break;

		case EIOClassicJoystick::DpadStick:
			break;
	}

	syncAxes();
}

void EIOClassicJoystick::syncAxes() {
	if (m_report_dpad) {
		report(EV_ABS, CLASSIC_DPAD_LINUX_AXIS_X, m_last_dpad_x);
		report(EV_ABS, CLASSIC_DPAD_LINUX_AXIS_Y, m_last_dpad_y);
	}

	if (m_report_left_stick) {
		report(EV_ABS, CLASSIC_RIGHT_STICK_LINUX_AXIS_X, m_last_r_stick_x);
		report(EV_ABS, CLASSIC_RIGHT_STICK_LINUX_AXIS_Y, m_last_r_stick_y);
	}

	if (m_report_right_stick) {
		report(EV_ABS, CLASSIC_LEFT_STICK_LINUX_AXIS_X, m_last_l_stick_x);
		report(EV_ABS, CLASSIC_LEFT_STICK_LINUX_AXIS_Y, m_last_l_stick_y);
	}

	sync();
}

bool EIOClassicJoystick::configure() {
	bool isValid = true;
	isValid &= set_ev_bit(EV_KEY) == 0;
	isValid &= set_ev_bit(EV_MSC) == 0;
	isValid &= set_ev_bit(EV_ABS) == 0;

	if (m_report_buttons) {
		isValid &= set_key_bit(BTN_0) == 0;
		isValid &= set_key_bit(BTN_1) == 0;
		isValid &= set_key_bit(BTN_2) == 0;
		isValid &= set_key_bit(BTN_3) == 0;
		isValid &= set_key_bit(BTN_GAMEPAD) == 0;
		isValid &= set_key_bit(BTN_B) == 0;
		isValid &= set_key_bit(BTN_X) == 0;
		isValid &= set_key_bit(BTN_Y) == 0;
		isValid &= set_key_bit(BTN_TL) == 0;
		isValid &= set_key_bit(BTN_TR) == 0;
		isValid &= set_key_bit(BTN_TL2) == 0;
		isValid &= set_key_bit(BTN_TR2) == 0;
		isValid &= set_key_bit(BTN_SELECT) == 0;
		isValid &= set_key_bit(BTN_MODE) == 0;
		isValid &= set_key_bit(BTN_START) == 0;
	}

	if (m_report_left_stick) {
		isValid &= set_abs_bit(CLASSIC_LEFT_STICK_LINUX_AXIS_X) == 0;
		isValid &= set_abs_bit(CLASSIC_LEFT_STICK_LINUX_AXIS_Y) == 0;
		set_range(CLASSIC_LEFT_STICK_LINUX_AXIS_X, CLASSIC_LEFT_STICK_MAX, CLASSIC_LEFT_STICK_MIN);
		set_range(CLASSIC_LEFT_STICK_LINUX_AXIS_Y, CLASSIC_LEFT_STICK_MAX, CLASSIC_LEFT_STICK_MIN);
	}

	if (m_report_right_stick) {
		isValid &= set_abs_bit(CLASSIC_RIGHT_STICK_LINUX_AXIS_X) == 0;
		isValid &= set_abs_bit(CLASSIC_RIGHT_STICK_LINUX_AXIS_Y) == 0;
		set_range(CLASSIC_RIGHT_STICK_LINUX_AXIS_X, CLASSIC_RIGHT_STICK_MAX, CLASSIC_RIGHT_STICK_MIN);
		set_range(CLASSIC_RIGHT_STICK_LINUX_AXIS_Y, CLASSIC_RIGHT_STICK_MAX, CLASSIC_RIGHT_STICK_MIN);
	}

	if (m_report_dpad) {
		isValid &= set_abs_bit(CLASSIC_DPAD_LINUX_AXIS_X) == 0;
		isValid &= set_abs_bit(CLASSIC_DPAD_LINUX_AXIS_Y) == 0;
		set_range(CLASSIC_DPAD_LINUX_AXIS_X, CLASSIC_DPAD_MAX, CLASSIC_DPAD_MIN);
		set_range(CLASSIC_DPAD_LINUX_AXIS_Y, CLASSIC_DPAD_MAX, CLASSIC_DPAD_MIN);
	}

	return isValid;
}
