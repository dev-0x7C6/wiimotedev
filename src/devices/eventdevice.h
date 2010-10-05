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

#ifndef UINPUT_EVENTDEVICE_H
#define UINPUT_EVENTDEVICE_H

#include "devices/general.h"

class UInputEvent: public UInputObject
{
public:
  bool uinput_open(bool replay = true);

  void pressKeyboardButton(quint16);
  void pressKeyboardButtonOnce(quint16);
  void releaseKeyboardButton(quint16);

  void moveMouseVWheel(qint32);
  void moveMouseHWheel(qint32);
  void moveMousePointerRel(qint32, qint32);
};

#endif // UINPUT_EVENTDEVICE_H
