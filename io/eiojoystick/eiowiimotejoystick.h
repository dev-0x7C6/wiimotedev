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

#include <emulation/input-device.h>
#include <interfaces/igamepad.h>

#include <QObject>

constexpr auto WIIMOTE_PITCH_MAX = 90;
constexpr auto WIIMOTE_PITCH_MIN = -90;
constexpr auto WIIMOTE_ROLL_MAX = 180;
constexpr auto WIIMOTE_ROLL_MIN = -180;
constexpr auto WIIMOTE_BUTTON_PUSHED = 1;
constexpr auto WIIMOTE_BUTTON_RELEASED = 0;
constexpr auto WIIMOTE_DPAD_MAX = 1;
constexpr auto WIIMOTE_DPAD_MIN = -1;



class EIOWiimoteJoystick final : public IGamepad {
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

	enum Device {
		Nunchuk,
		Wiimote
	};

public:
	explicit EIOWiimoteJoystick(const std::string &name, const uint32_t id, Mode mode = DPadPositionConstant, Position horizontal = GamepadVertical);

		virtual Type type() const override;

	void setDStickInvertX(bool option);
	void setDStickInvertY(bool option);
	void setHomeSwitchPosition(bool option);
	void setReportButtons(bool report);
	void setReportDStick(bool report);
	void setReportPitch(bool report);
	void setReportRoll(bool report);

	virtual bool inputButtons(const uint64_t buttons) override;
	virtual bool inputStick(const Stick stick, const int32_t x, const int32_t y) override;
	virtual bool inputAccelerometer(const double pitch, const double roll) override;

	void setWiimoteButtons(uint64_t);
	void setWiimoteAcc(double, double);

	virtual bool configure() override;

private:
	void centerStick(Stick id);
	void syncSticks();

signals:
	void setLedState(uint32_t, uint32_t);

private:
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
	bool m_dpad_invert_x;
	bool m_dpad_invert_y;
	bool m_home_switch_position;
	bool m_report_buttons;
	bool m_report_dstick;
	bool m_report_pitch;
	bool m_report_roll;
};
