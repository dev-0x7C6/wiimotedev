/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/ 

#ifndef UINPUT_WIIMOTEGAMEPAD_H
#define UINPUT_WIIMOTEGAMEPAD_H

#include "devices/keyboard.h"

const qint16 NUNCHUK_STICK_MAX = (0xFF >> 1) + (0xFF >> 2);
const qint16 NUNCHUK_STICK_MIN = (0xFF >> 1) - (0xFF >> 2);

const qint16 WIIMOTE_PITCH_MAX = 180;
const qint16 WIIMOTE_PITCH_MIN =-180;
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

class WiimoteGamepadDevice: public UInputObject
{
private:
  QString deviceName;

public:
  WiimoteGamepadDevice(QString deviceName, QObject *parent = 0);
  bool uinput_open();

public:
  void setWiimoteButtons(quint64);
  void setWiimoteTilts(double, double);
  void setNunchukButtons(quint64);
  void setNunchukStick(qint32, qint32);
  void setNunchukTilts(double, double);
};

#endif // UINPUT_WIIMOTEGAMEPAD_H
