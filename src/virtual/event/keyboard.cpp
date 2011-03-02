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

#include "virtual/event/keyboard.h"

extern QMap < QString, quint32> scancodes;

enum KeyboardExtension {
  keyboardExt = 0xFFFF,
  keyboardExtMouseHWheelUp,
  keyboardExtMouseHWheelDown,
  keyboardExtMouseVWheelUp,
  keyboardExtMouseVWheelDown
};

EventVirtualKeyboard::EventVirtualKeyboard(UInputEvent *device) :
  device(device)
{
  compareType = HashCompare< quint32, quint64>::BitCompare;
}


EventVirtualKeyboard::~EventVirtualKeyboard() {
  disconnect(this, 0, 0, 0);
  foreach (struct KeyboardAction* action, keyboardActions) {
    if (action->pushed)
      EventVirtualKeyboard::releaseKeyboardButtons(action->keys);
    delete action;
  }
  keyboardActions.clear();
}

void EventVirtualKeyboard::setCompareType(QString type) {
  compareType = HashCompare< quint32, quint64>::BitCompare;

  if (type.toLower() == QString("bitCompare").toLower())
    compareType = HashCompare< quint32, quint64>::BitCompare; else
  if (type.toLower() == QString("equal").toLower())
    compareType = HashCompare< quint32, quint64>::EqualCompare; else
  if (type.toLower() == QString("notEqual").toLower())
    compareType = HashCompare< quint32, quint64>::NotEqualCompare;

}

void EventVirtualKeyboard::addKeyboardAction(KeyboardAction &action) {
  KeyboardAction *kbdAction = new KeyboardAction;
  kbdAction->event = action.event;
  kbdAction->keys = action.keys;
  kbdAction->pushed = false;
  keyboardActions << kbdAction;
}

void EventVirtualKeyboard::clearKeyboardActions() {
  foreach (KeyboardAction *action, keyboardActions)
    delete action;
  keyboardActions.clear();
}

void EventVirtualKeyboard::dbusWiimoteGeneralButtons(quint32 id, quint64 value) {
  if (value == buttons.value(id, -1))
    return;

  buttons[id] = value;

  HashCompare< quint32, quint64> compare;

  foreach (KeyboardAction *action, keyboardActions) {
    if (action->event.isEmpty())
      continue;

    bool matched = compare.compare(&action->event, &buttons, compareType);
    if (matched && !action->pushed) {
      action->pushed = !action->pushed;
      pressKeyboardButtons(action->keys);
    } else
    if (!matched && action->pushed) {
      action->pushed = !action->pushed;
      releaseKeyboardButtons(action->keys);
    }
  }
}

void EventVirtualKeyboard::pressKeyboardExtendedButton(quint32 key) {
  switch (key) {
  case keyboardExtMouseHWheelUp:
    device->moveMouseHWheel(1);
    break;
  case keyboardExtMouseHWheelDown:
    device->moveMouseHWheel(-1);
    break;
  case keyboardExtMouseVWheelUp:
    device->moveMouseVWheel(1);
    break;
  case keyboardExtMouseVWheelDown:
    device->moveMouseVWheel(-1);
    break;
  }
}

void EventVirtualKeyboard::releaseKeyboardExtendedButton(quint32 key) {
  Q_UNUSED(key);
}

void EventVirtualKeyboard::pressKeyboardButtons(QList < quint32> &list) {
  if (list.isEmpty())
    return;

  foreach (const quint32 key, list)
    if (key <= keyboardExt)
      device->pressKeyboardButton(key); else
      pressKeyboardExtendedButton(key);
}

void EventVirtualKeyboard::releaseKeyboardButtons(QList < quint32> &list) {
  if (list.isEmpty())
    return;

  foreach (const quint32 key, list)
    if (key <= keyboardExt)
      device->releaseKeyboardButton(key); else
      releaseKeyboardExtendedButton(key);
}
