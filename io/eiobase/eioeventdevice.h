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

#ifndef UINPUT_EVENTDEVICE_H
#define UINPUT_EVENTDEVICE_H

#include "eiobase/eiouinputobject.h"

class EIO_EventDevice: public EIO_UInputObject {
public:
  bool uinput_open(bool replay = true);

  void pressKeyboardButton(uint16);
  void pressKeyboardButtonOnce(uint16);
  void releaseKeyboardButton(uint16);

  void moveMouseVWheel(int32);
  void moveMouseHWheel(int32);
  void moveMousePointerRel(int32, int32);
};

#endif // UINPUT_EVENTDEVICE_H
