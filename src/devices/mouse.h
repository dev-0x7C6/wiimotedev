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

#ifndef UINPUT_MOUSE_H
#define UINPUT_MOUSE_H

#include "devices/general.h"

#include <QRect>

class UInputMouse: public UInputObject
{
public:
  UInputMouse() :UInputObject() {}

  bool uinput_open(QRect absRect = QRect(-512, -384, 1024, 768), bool abs = false);

  void moveMousePointerRel(qint32 x, qint32 y);
  void moveMousePointerAbs(qint32 x, qint32 y);
  void moveMouseVWheel(qint32 direction);
  void moveMouseHWheel(qint32 direction);
  void pressMouseButton(quint16 button);
  void releaseMouseButton(quint16 button);
};

#endif // UINPUT_MOUSE_H
