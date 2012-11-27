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


#ifndef NUNCHUKJOYSTICK_H
#define NUNCHUKJOYSTICK_H

#include "eiobase/eiouinputobject.h"

const qint16 NUNCHUK_STICK_MAX = 0xFF - 0x15;
const qint16 NUNCHUK_STICK_MIN = 0x00 + 0x15;
const qint16 NUNCHUK_PITCH_MAX = 90;
const qint16 NUNCHUK_PITCH_MIN =-90;
const qint16 NUNCHUK_ROLL_MAX = 180;
const qint16 NUNCHUK_ROLL_MIN =-180;
const qint8 NUNCHUK_BUTTON_PUSHED = 1;
const qint8 NUNCHUK_BUTTON_RELEASED = 0;

const int NUNCHUK_STICK_LINUX_AXIS_X = ABS_RX;
const int NUNCHUK_STICK_LINUX_AXIS_Y = ABS_RY;
const int NUNCHUK_PITCH_LINUX_AXIS = ABS_HAT2X;
const int NUNCHUK_ROLL_LINUX_AXIS = ABS_HAT3X;

class EIO_NunchukJoystick : public QObject, public EIO_UInputObject
{
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
  int m_id;
  bool m_stick_invert_x;
  bool m_stick_invert_y;
  bool m_report_buttons;
  bool m_report_stick;
  bool m_report_pitch;
  bool m_report_roll;

public:
  EIO_NunchukJoystick(QString deviceName, int id, QObject *parent = 0);
  bool create();

  int assign();
  void setStickInvertX(bool option);
  void setStickInvertY(bool option);
  void setReportButtons(bool report);
  void setReportStick(bool report);
  void setReportPitch(bool report);
  void setReportRoll(bool report);

  void setNunchukButtons(quint64);
  void setNunchukStick(qint32, qint32);
  void setNunchukAcc(double, double);

private:
  void centerStick(Stick id);
  void syncAxes();

signals:
  void setLedState(quint32, quint32);
};

#endif // NUNCHUKJOYSTICK_H
