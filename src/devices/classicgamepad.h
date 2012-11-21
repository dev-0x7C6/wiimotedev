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

#include "devices/general.h"

const qint16 CLASSIC_LEFT_STICK_MAX = 0x3B;
const qint16 CLASSIC_LEFT_STICK_MIN = 0x09;
const qint16 CLASSIC_RIGHT_STICK_MAX = 0x1B;
const qint16 CLASSIC_RIGHT_STICK_MIN = 0x02;

const qint8 CLASSIC_BUTTON_PUSHED = 0x01;
const qint8 CLASSIC_BUTTON_RELEASED = 0x00;
const qint8 CLASSIC_DPAD_MAX =  0x01;
const qint8 CLASSIC_DPAD_MIN = -0x01;

const int CLASSIC_LEFT_STICK_LINUX_AXIS_X = ABS_HAT0X;
const int CLASSIC_LEFT_STICK_LINUX_AXIS_Y = ABS_HAT0Y;
const int CLASSIC_RIGHT_STICK_LINUX_AXIS_X = ABS_HAT1X;
const int CLASSIC_RIGHT_STICK_LINUX_AXIS_Y = ABS_HAT1Y;
const int CLASSIC_DPAD_LINUX_AXIS_X = ABS_HAT2X;
const int CLASSIC_DPAD_LINUX_AXIS_Y = ABS_HAT2Y;


class ClassicGamepadDevice: public UInputObject
{
private:
  QString m_deviceName;
  qint32 m_last_r_stick_x;
  qint32 m_last_r_stick_y;
  qint32 m_last_l_stick_x;
  qint32 m_last_l_stick_y;
  qint32 m_last_dpad_x;
  qint32 m_last_dpad_y;

public:
  explicit ClassicGamepadDevice(QString deviceName);
  virtual ~ClassicGamepadDevice();

  bool uinput_open();

  enum Sticks {
    LeftStick,
    RightStick,
    DpadStick
  };

  void setButtons(quint64 buttons);
  void setStick(Sticks stick, qint32 x, qint32 y);

private:
  void centerStick(Sticks stick);
  void syncAxes();

};

#endif // UINPUT_CLASSICGAMEPAD_H
