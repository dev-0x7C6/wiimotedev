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

#ifndef UINPUT_WIIMOTEGAMEPAD_H
#define UINPUT_WIIMOTEGAMEPAD_H

#include "devices/general.h"

const qint16 NUNCHUK_STICK_MAX = 0xFF - 0x15;
const qint16 NUNCHUK_STICK_MIN = 0x00 + 0x15;

const qint16 WIIMOTE_PITCH_MAX = 90;
const qint16 WIIMOTE_PITCH_MIN =-90;
const qint16 WIIMOTE_ROLL_MAX = 180;
const qint16 WIIMOTE_ROLL_MIN =-180;

#define NUNCHUK_PITCH_MAX WIIMOTE_PITCH_MAX
#define NUNCHUK_PITCH_MIN WIIMOTE_PITCH_MIN
#define NUNCHUK_ROLL_MAX WIIMOTE_ROLL_MAX
#define NUNCHUK_ROLL_MIN WIIMOTE_ROLL_MIN

const qint8 WIIMOTE_BUTTON_PUSHED = 1;
const qint8 WIIMOTE_BUTTON_RELEASED = 0;

#define NUNCHUK_BUTTON_PUSHED WIIMOTE_BUTTON_PUSHED
#define NUNCHUK_BUTTON_RELEASED WIIMOTE_BUTTON_RELEASED

const qint8 WIIMOTE_DPAD_MAX = 1;
const qint8 WIIMOTE_DPAD_MIN =-1;

const int NUNCHUK_STICK_LINUX_AXIS_X = ABS_RX;
const int NUNCHUK_STICK_LINUX_AXIS_Y = ABS_RY;
const int WIIMOTE_DPAD_LINUX_AXIS_X = ABS_X;
const int WIIMOTE_DPAD_LINUX_AXIS_Y = ABS_Y;
const int WIIMOTE_PITCH_LINUX_AXIS = ABS_HAT0X;
const int WIIMOTE_ROLL_LINUX_AXIS = ABS_HAT1X;
const int NUNCHUK_PITCH_LINUX_AXIS = ABS_HAT2X;
const int NUNCHUK_ROLL_LINUX_AXIS = ABS_HAT3X;


//void setButtons(quint64 buttons);
//void setStick(Sticks stick, qint32 x, qint32 y);

//private:
//void centerStick(Sticks stick);
//void syncSticks();

class WiimoteGamepadDevice: public UInputObject
{
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

public:
  WiimoteGamepadDevice(QString deviceName, Mode mode = DPadPositionConstant, Position horizontal = GamepadVertical);
  bool uinput_open();


  void setWiimoteButtons(quint64);
  void setWiimoteAcc(double, double);

  void setNunchukButtons(quint64);
  void setNunchukStick(qint32, qint32);
  void setNunchukAcc(double, double);

private:
  void centerStick(Stick id);
  void syncAxes();
};

#endif // UINPUT_WIIMOTEGAMEPAD_H
