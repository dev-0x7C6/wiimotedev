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

#ifndef UINPUT_CLASSICGAMEPAD_H
#define UINPUT_CLASSICGAMEPAD_H

#include "devices/keyboard.h"

const qint16 CLASSIC_LEFT_STICK_MAX = (0x3F >> 1) + (0x3F >> 2) + 4;
const qint16 CLASSIC_LEFT_STICK_MIN = (0x3F >> 1) - (0x3F >> 2) - 4;
const qint16 CLASSIC_RIGHT_STICK_MAX = (0x3F >> 1) + (0x3F >> 2) + 4;
const qint16 CLASSIC_RIGHT_STICK_MIN = (0x3F >> 1) - (0x3F >> 2) - 4;

const qint8 CLASSIC_BUTTON_PUSHED = 1;
const qint8 CLASSIC_BUTTON_RELEASED = 0;
const qint8 CLASSIC_DPAD_MAX = 1;
const qint8 CLASSIC_DPAD_MIN =-1;

class ClassicGamepadDevice: public UInputObject
{
private:
  QString deviceName;

public:
  ClassicGamepadDevice(QString deviceName, QObject *parent = 0);
  bool uinput_open();

public:
  void setButtons(quint64);
  void setLeftStick(qint32, qint32);
  void setRightStick(qint32, qint32);
};

#endif // UINPUT_CLASSICGAMEPAD_H
