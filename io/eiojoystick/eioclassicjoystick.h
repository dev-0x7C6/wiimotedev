/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2010  Bartłomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
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

#ifndef UINPUT_CLASSICGAMEPAD_H
#define UINPUT_CLASSICGAMEPAD_H

#include "eiobase/eiouinputobject.h"

const int16 CLASSIC_LEFT_STICK_MAX = 0x3B;
const int16 CLASSIC_LEFT_STICK_MIN = 0x09;
const int16 CLASSIC_RIGHT_STICK_MAX = 0x1B;
const int16 CLASSIC_RIGHT_STICK_MIN = 0x02;

const int8 CLASSIC_BUTTON_PUSHED = 0x01;
const int8 CLASSIC_BUTTON_RELEASED = 0x00;
const int8 CLASSIC_DPAD_MAX =  0x01;
const int8 CLASSIC_DPAD_MIN = -0x01;

const int CLASSIC_LEFT_STICK_LINUX_AXIS_X = ABS_HAT0X;
const int CLASSIC_LEFT_STICK_LINUX_AXIS_Y = ABS_HAT0Y;
const int CLASSIC_RIGHT_STICK_LINUX_AXIS_X = ABS_HAT1X;
const int CLASSIC_RIGHT_STICK_LINUX_AXIS_Y = ABS_HAT1Y;
const int CLASSIC_DPAD_LINUX_AXIS_X = ABS_HAT2X;
const int CLASSIC_DPAD_LINUX_AXIS_Y = ABS_HAT2Y;

class EIO_ClassicJoystick: public EIO_UInputObject
{
private:
  QString m_deviceName;
  int m_id;
  int32 m_last_r_stick_x;
  int32 m_last_r_stick_y;
  int32 m_last_l_stick_x;
  int32 m_last_l_stick_y;
  int32 m_last_dpad_x;
  int32 m_last_dpad_y;
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
  explicit EIO_ClassicJoystick(QString deviceName, int id);
  virtual ~EIO_ClassicJoystick();

  bool create();

  enum Sticks {
    LeftStick,
    RightStick,
    DpadStick
  };

  int assign();
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

  void setButtons(uint64 buttons);
  void setStick(Sticks stick, int32 x, int32 y);

private:
  void centerStick(Sticks stick);
  void syncAxes();

};

#endif // UINPUT_CLASSICGAMEPAD_H
