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

#ifndef EVENT_VIRTUAL_KEYBOARD_H
#define EVENT_VIRTUAL_KEYBOARD_H

#include "eiobase/eioeventdevice.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "helper/hashcompare.h"

#include <QTimer>
#include <QTime>

#include <QMap>

struct KeyboardAction {
  QHash<uint, uint64> event;
  QList<uint> keys;
  bool pushed;
};

class EIO_RemoteKeyboard: public QObject {
  Q_OBJECT
  //general
  EIO_EventDevice *device;
  uint id;
  uint compareType;

  QHash <uint, uint64> buttons;

  QList <KeyboardAction *> keyboardActions;

public:
  EIO_RemoteKeyboard(EIO_EventDevice *device);
  ~EIO_RemoteKeyboard();

  void addKeyboardAction(KeyboardAction &);
  void clearKeyboardActions();
  void setCompareType(QString);

public Q_SLOTS:
  void dbusWiimoteGeneralButtons(uint, uint64);

private:
  void pressKeyboardButtons(QList <uint> &);
  void releaseKeyboardButtons(QList <uint> &);
  void pressKeyboardExtendedButton(uint);
  void releaseKeyboardExtendedButton(uint);

};
#endif // EVENT_VIRTUAL_KEYBOARD_H
