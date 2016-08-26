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

EIOWiimoteJoystick::EIOWiimoteJoystick(const std::string &name, const uint32_t id, Mode mode, Position horizontal, QObject *parent)
		: InputDevice(name, id)
		, m_horizontal(horizontal)
		, m_mode(mode)
		, m_dpad_invert_x(0)
		, m_dpad_invert_y(0)
		, m_home_switch_position(1)
		, m_report_buttons(1)
		, m_report_dstick(1)
		, m_report_pitch(1)
		, m_report_roll(1) {

	centerStick(EIOWiimoteJoystick::DpadStick);
	centerStick(EIOWiimoteJoystick::WiimoteAccelerometer);
	centerStick(EIOWiimoteJoystick::NunchukAccelerometer);
}

uint32_t EIOWiimoteJoystick::assign() {
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

constexpr auto UINPUT_WIIMOTE_BTN_A = BTN_A;
constexpr auto UINPUT_WIIMOTE_BTN_B = BTN_B;
constexpr auto UINPUT_WIIMOTE_BTN_1 = BTN_X;
constexpr auto UINPUT_WIIMOTE_BTN_2 = BTN_Y;
constexpr auto UINPUT_WIIMOTE_START = BTN_START;
constexpr auto UINPUT_WIIMOTE_SELECT = BTN_SELECT;

constexpr auto WIIMOTE_DPAD_LINUX_AXIS_X = ABS_HAT0X;
constexpr auto WIIMOTE_DPAD_LINUX_AXIS_Y = ABS_HAT0Y;
constexpr auto WIIMOTE_PITCH_LINUX_AXIS = ABS_X;
constexpr auto WIIMOTE_ROLL_LINUX_AXIS = ABS_RX;

void EIOWiimoteJoystick::setWiimoteButtons(uint64_t buttons) {
	if (m_report_buttons) {
		report(EV_KEY, UINPUT_WIIMOTE_BTN_A, (buttons & WIIMOTE_BTN_A) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, UINPUT_WIIMOTE_BTN_B, (buttons & WIIMOTE_BTN_B) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, UINPUT_WIIMOTE_BTN_1, (buttons & WIIMOTE_BTN_1) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, UINPUT_WIIMOTE_BTN_2, (buttons & WIIMOTE_BTN_2) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, UINPUT_WIIMOTE_START, (buttons & WIIMOTE_BTN_PLUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, UINPUT_WIIMOTE_SELECT, (buttons & WIIMOTE_BTN_MINUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, BTN_0, (buttons & WIIMOTE_BTN_RIGHT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, BTN_1, (buttons & WIIMOTE_BTN_LEFT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, BTN_2, (buttons & WIIMOTE_BTN_DOWN) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
		report(EV_KEY, BTN_3, (buttons & WIIMOTE_BTN_UP) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

		if (!m_home_switch_position)
			report(EV_KEY, BTN_MODE, (buttons & WIIMOTE_BTN_HOME) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

		sync();
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
	m_last_dpad_x = 0;
	m_last_dpad_y = 0;

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
			m_last_dpad_x = 0;
			m_last_dpad_y = 0;
			break;

		case EIOWiimoteJoystick::NunchukAccelerometer:
			m_last_nunchuk_acc_pitch = 0;
			m_last_nunchuk_acc_roll = 0;
			break;

		case EIOWiimoteJoystick::WiimoteAccelerometer:
			m_last_wiimote_acc_pitch = 0;
			m_last_wiimote_acc_roll = 0;
			break;

		default:
			break;
	}
}

void EIOWiimoteJoystick::syncAxes() {
	if (m_report_dstick) {
		switch (m_horizontal) {
			case EIOWiimoteJoystick::GamepadHorizontal:
				report(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_y);
				report(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_x * -1);
				break;

			case EIOWiimoteJoystick::GamepadVertical:
				report(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_x);
				report(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_y);
				break;
		}
	}

	if (m_report_pitch)
		report(EV_ABS, WIIMOTE_PITCH_LINUX_AXIS, m_last_wiimote_acc_pitch);

	if (m_report_roll)
		report(EV_ABS, WIIMOTE_ROLL_LINUX_AXIS, m_last_wiimote_acc_roll);

	sync();
}

bool EIOWiimoteJoystick::configure() {
	set_ev_bit(EV_KEY);
	set_ev_bit(EV_MSC);
	set_ev_bit(EV_ABS);

	if (m_report_buttons) {
		set_key_bit(BTN_0);
		set_key_bit(BTN_1);
		set_key_bit(BTN_2);
		set_key_bit(BTN_3);
		set_key_bit(BTN_GAMEPAD);
		set_key_bit(BTN_B);
		set_key_bit(BTN_X);
		set_key_bit(BTN_Y);
		set_key_bit(BTN_SELECT);
		set_key_bit(BTN_START);

		if (!m_home_switch_position)
			set_key_bit(BTN_MODE);
	}

	if (m_report_dstick) {
		set_abs_bit(WIIMOTE_DPAD_LINUX_AXIS_X);
		set_abs_bit(WIIMOTE_DPAD_LINUX_AXIS_Y);
		set_range(WIIMOTE_DPAD_LINUX_AXIS_X, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
		set_range(WIIMOTE_DPAD_LINUX_AXIS_Y, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
	}

	if (m_report_pitch) {
		set_abs_bit(WIIMOTE_PITCH_LINUX_AXIS);
		set_range(WIIMOTE_PITCH_LINUX_AXIS, WIIMOTE_PITCH_MAX, WIIMOTE_PITCH_MIN);
	}

	if (m_report_roll) {
		set_abs_bit(WIIMOTE_ROLL_LINUX_AXIS);
		set_range(WIIMOTE_ROLL_LINUX_AXIS, WIIMOTE_ROLL_MAX, WIIMOTE_ROLL_MIN);
	}
}

void EIOWiimoteJoystick::setWiimoteAcc(double pitch, double roll) {
	m_last_wiimote_acc_pitch = pitch;
	m_last_wiimote_acc_roll = roll;
	syncAxes();
}
