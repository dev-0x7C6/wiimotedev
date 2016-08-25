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

#ifndef UINPUT_WIIMOTEGAMEPAD_H
#define UINPUT_WIIMOTEGAMEPAD_H

#include "eiobase/eiouinputobject.h"

const int16_t WIIMOTE_PITCH_MAX = 90;
const int16_t WIIMOTE_PITCH_MIN = -90;
const int16_t WIIMOTE_ROLL_MAX = 180;
const int16_t WIIMOTE_ROLL_MIN = -180;
const int8_t WIIMOTE_BUTTON_PUSHED = 1;
const int8_t WIIMOTE_BUTTON_RELEASED = 0;
const int8_t WIIMOTE_DPAD_MAX = 1;
const int8_t WIIMOTE_DPAD_MIN = -1;

const int WIIMOTE_DPAD_LINUX_AXIS_X = ABS_HAT0X;
const int WIIMOTE_DPAD_LINUX_AXIS_Y = ABS_HAT0Y;
const int WIIMOTE_PITCH_LINUX_AXIS = ABS_X;
const int WIIMOTE_ROLL_LINUX_AXIS = ABS_RX;

class EIOWiimoteJoystick : public QObject, public InputDevice {
	Q_OBJECT
public:
	enum Position {
		GamepadHorizontal,
		GamepadVertical
	};

	enum Mode {
		DPadPositionConstant,
		DPadPositionSwitchable
	};

	enum Stick {
		NunchukStick,
		DpadStick,
		WiimoteAccelerometer,
		NunchukAccelerometer
	};

	enum Device {
		Nunchuk,
		Wiimote
	};

private:
	QString m_deviceName;
	int m_last_stick_x;
	int m_last_stick_y;
	int m_last_dpad_x;
	int m_last_dpad_y;
	int m_last_nunchuk_acc_pitch;
	int m_last_nunchuk_acc_roll;
	int m_last_wiimote_acc_pitch;
	int m_last_wiimote_acc_roll;
	Position m_horizontal;
	Mode m_mode;
	bool m_home_pressed;
	uint32_t m_id;
	bool m_dpad_invert_x;
	bool m_dpad_invert_y;
	bool m_home_switch_position;
	bool m_report_buttons;
	bool m_report_dstick;
	bool m_report_pitch;
	bool m_report_roll;

public:
	EIOWiimoteJoystick(QString deviceName, int id, Mode mode = DPadPositionConstant, Position horizontal = GamepadVertical, QObject *parent = 0);

	uint32_t assign();

	void setDStickInvertX(bool option);
	void setDStickInvertY(bool option);
	void setHomeSwitchPosition(bool option);
	void setReportButtons(bool report);
	void setReportDStick(bool report);
	void setReportPitch(bool report);
	void setReportRoll(bool report);

	void setWiimoteButtons(uint64_t);
	void setWiimoteAcc(double, double);

private:
	void centerStick(Stick id);
	void syncAxes();

signals:
	void setLedState(uint32_t, uint32_t);

	// InputDevice interface
protected:
	virtual bool configure() override;
};

#endif // UINPUT_WIIMOTEGAMEPAD_H
