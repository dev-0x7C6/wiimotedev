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

EIONunchukJoystick::EIONunchukJoystick(const std::string &name, const uint32_t id)
		: IGamepad(name, id)
		, m_stick_invert_x(0)
		, m_stick_invert_y(0)
		, m_report_buttons(1)
		, m_report_stick(1)
		, m_report_pitch(1)
		, m_report_roll(1)

{
}

IGamepad::Type EIONunchukJoystick::type() const { return Type::Nunchuk; }

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

bool EIONunchukJoystick::inputButtons(const uint64_t buttons) {
	if (!m_report_buttons)
		return false;

	report(EV_KEY, BTN_A, (buttons & NUNCHUK_BTN_C) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
	report(EV_KEY, BTN_B, (buttons & NUNCHUK_BTN_Z) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
	sync();

	return true;
}

bool EIONunchukJoystick::inputStick(const EIONunchukJoystick::Stick stick, const int32_t x, const int32_t y) {
	if (Stick::NunchukStick == stick) {
		m_last_stick_x = x;
		m_last_stick_y = y;
	}
	return syncSticks();
}

bool EIONunchukJoystick::inputAccelerometer(const double pitch, const double roll) {
	m_last_nunchuk_acc_pitch = pitch;
	m_last_nunchuk_acc_roll = roll;
	syncSticks();
}

bool EIONunchukJoystick::centerStick(Stick id) {
	switch (id) {
		case Stick::NunchukStick:
			m_last_stick_x = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
			m_last_stick_y = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
			break;

		case Stick::NunchukAccelerometer:
			m_last_nunchuk_acc_pitch = 0;
			m_last_nunchuk_acc_roll = 0;
			break;

		default:
			return false;
	}

	return true;
}

bool EIONunchukJoystick::syncSticks() {
	if (m_report_stick) {
		report(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_X, m_last_stick_x);
		report(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_Y, m_last_stick_y);
	}

	if (m_report_pitch)
		report(EV_ABS, NUNCHUK_PITCH_LINUX_AXIS, m_last_nunchuk_acc_pitch);

	if (m_report_roll)
		report(EV_ABS, NUNCHUK_ROLL_LINUX_AXIS, m_last_nunchuk_acc_roll);

	return sync();
}

bool EIONunchukJoystick::configure() {
	set_ev_bit(EV_KEY);
	set_ev_bit(EV_MSC);
	set_ev_bit(EV_ABS);

	if (m_report_buttons) {
		set_key_bit(BTN_GAMEPAD);
		set_key_bit(BTN_B);
	}

	if (m_report_stick) {
		set_abs_bit(NUNCHUK_STICK_LINUX_AXIS_X);
		set_abs_bit(NUNCHUK_STICK_LINUX_AXIS_Y);
		set_range(NUNCHUK_STICK_LINUX_AXIS_X, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
		set_range(NUNCHUK_STICK_LINUX_AXIS_Y, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
	}

	if (m_report_pitch) {
		set_abs_bit(NUNCHUK_PITCH_LINUX_AXIS);
		set_range(NUNCHUK_PITCH_LINUX_AXIS, NUNCHUK_PITCH_MAX, NUNCHUK_PITCH_MIN);
	}

	if (m_report_roll) {
		set_abs_bit(NUNCHUK_ROLL_LINUX_AXIS);
		set_range(NUNCHUK_ROLL_LINUX_AXIS, NUNCHUK_ROLL_MAX, NUNCHUK_ROLL_MIN);
	}
}
