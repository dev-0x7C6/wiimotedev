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

#include <QObject>
#include "eiobase/eiouinputobject.h"

constexpr auto CLASSIC_LEFT_STICK_MAX = 0x3B;
constexpr auto CLASSIC_LEFT_STICK_MIN = 0x09;
constexpr auto CLASSIC_RIGHT_STICK_MAX = 0x1B;
constexpr auto CLASSIC_RIGHT_STICK_MIN = 0x02;

constexpr auto CLASSIC_BUTTON_PUSHED = 0x01;
constexpr auto CLASSIC_BUTTON_RELEASED = 0x00;
constexpr auto CLASSIC_DPAD_MAX = 0x01;
constexpr auto CLASSIC_DPAD_MIN = -0x01;

constexpr auto CLASSIC_LEFT_STICK_LINUX_AXIS_X = ABS_X;
constexpr auto CLASSIC_LEFT_STICK_LINUX_AXIS_Y = ABS_Y;
constexpr auto CLASSIC_RIGHT_STICK_LINUX_AXIS_X = ABS_RX;
constexpr auto CLASSIC_RIGHT_STICK_LINUX_AXIS_Y = ABS_RY;
constexpr auto CLASSIC_DPAD_LINUX_AXIS_X = ABS_HAT0X;
constexpr auto CLASSIC_DPAD_LINUX_AXIS_Y = ABS_HAT0Y;

class EIOClassicJoystick : public QObject, public InputDevice {
	Q_OBJECT
private:
	QString m_deviceName;
	uint32_t m_id;
	int32_t m_last_r_stick_x;
	int32_t m_last_r_stick_y;
	int32_t m_last_l_stick_x;
	int32_t m_last_l_stick_y;
	int32_t m_last_dpad_x;
	int32_t m_last_dpad_y;
	bool m_dpad_invert_x;
	bool m_dpad_invert_y;
	bool m_left_stick_invert_x;
	bool m_left_stick_invert_y;
	bool m_right_stick_invert_x;
	bool m_right_stick_invert_y;
	bool m_report_buttons;
	bool m_report_dpad;
	bool m_report_left_stick;
	bool m_report_right_stick;

public:
	explicit EIOClassicJoystick(QString deviceName, int id, QObject *parent = 0);
	virtual ~EIOClassicJoystick();

	enum Sticks {
		LeftStick,
		RightStick,
		DpadStick
	};

	uint32_t assign();

	void setDpadInvertX(bool option);
	void setDpadInvertY(bool option);
	void setLStickInvertX(bool option);
	void setLStickInvertY(bool option);
	void setRStickInvertX(bool option);
	void setRStickInvertY(bool option);
	void setReportButtons(bool report);
	void setReportDStick(bool report);
	void setReportLStick(bool report);
	void setReportRStick(bool report);

	void setButtons(uint64_t buttons);
	void setStick(Sticks stick, int32_t x, int32_t y);

private:
	void centerStick(Sticks stick);
	void syncAxes();

protected:
	virtual bool configure() override;
};
