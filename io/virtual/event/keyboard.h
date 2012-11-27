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

#ifndef EVENT_VIRTUAL_KEYBOARD_H
#define EVENT_VIRTUAL_KEYBOARD_H

#include "eiobase/eioeventdevice.h"
#include "headers/consts.h"
#include "helper/hashcompare.h"

#include <QTimer>
#include <QTime>

#include <QMap>

struct KeyboardAction {
  QHash< quint32, quint64> event;
  QList< quint32> keys;
  bool pushed;
};

class EventVirtualKeyboard: public QObject
{
  Q_OBJECT
//general
  EIO_EventDevice *device;
  quint32 id;
  quint32 compareType;

  QHash <quint32, quint64> buttons;

  QList < KeyboardAction*> keyboardActions;

public:
  EventVirtualKeyboard(EIO_EventDevice *device);
 ~EventVirtualKeyboard();

  void addKeyboardAction(KeyboardAction&);
  void clearKeyboardActions();
  void setCompareType(QString);

public Q_SLOTS:
  void dbusWiimoteGeneralButtons(quint32, quint64);

private:
  void pressKeyboardButtons(QList < quint32>&);
  void releaseKeyboardButtons(QList < quint32>&);
  void pressKeyboardExtendedButton(quint32);
  void releaseKeyboardExtendedButton(quint32);

};
#endif // EVENT_VIRTUAL_KEYBOARD_H