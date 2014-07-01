/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2014  Bartłomiej Burdukiewicz                               *
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

#include "eioremotekeyboard.h"

extern QMap < QString, uint> scancodes;

enum KeyboardExtension {
  keyboardExt = 0xFFFF,
  keyboardExtMouseWheelLeft,
  keyboardExtMouseWheelRight,
  keyboardExtMouseWheelUp,
  keyboardExtMouseWheelDown
};

EIO_RemoteKeyboard::EIO_RemoteKeyboard(EIO_EventDevice *device) :
  device(device)
{
  compareType = HashCompare< uint, uint64>::BitCompare;
}


EIO_RemoteKeyboard::~EIO_RemoteKeyboard() {
  disconnect(this, 0, 0, 0);
  foreach (struct KeyboardAction* action, keyboardActions) {
    EIO_RemoteKeyboard::releaseKeyboardButtons(action->keys);
    delete action;
  }
  keyboardActions.clear();
}

void EIO_RemoteKeyboard::setCompareType(QString type) {
  compareType = HashCompare< uint, uint64>::BitCompare;

  if (type.toLower() == QString("bitCompare").toLower())
    compareType = HashCompare< uint, uint64>::BitCompare; else
  if (type.toLower() == QString("equal").toLower())
    compareType = HashCompare< uint, uint64>::EqualCompare; else
  if (type.toLower() == QString("notEqual").toLower())
    compareType = HashCompare< uint, uint64>::NotEqualCompare;

}

void EIO_RemoteKeyboard::addKeyboardAction(KeyboardAction &action) {
  KeyboardAction *kbdAction = new KeyboardAction;
  kbdAction->event = action.event;
  kbdAction->keys = action.keys;
  kbdAction->pushed = false;
  keyboardActions << kbdAction;
}

void EIO_RemoteKeyboard::clearKeyboardActions() {
  foreach (KeyboardAction *action, keyboardActions)
    delete action;
  keyboardActions.clear();
}

void EIO_RemoteKeyboard::dbusWiimoteGeneralButtons(uint id, uint64 value) {
  if (value == buttons.value(id, -1))
    return;

  buttons[id] = value;

  HashCompare< uint, uint64> compare;

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

void EIO_RemoteKeyboard::pressKeyboardExtendedButton(uint key) {
  switch (key) {
  case keyboardExtMouseWheelLeft:
    device->moveMouseHWheel(-1);
    break;
  case keyboardExtMouseWheelRight:
    device->moveMouseHWheel(1);
    break;
  case keyboardExtMouseWheelUp:
    device->moveMouseVWheel(1);
    break;
  case keyboardExtMouseWheelDown:
    device->moveMouseVWheel(-1);
    break;
  }
}

void EIO_RemoteKeyboard::releaseKeyboardExtendedButton(uint key) {
  Q_UNUSED(key);
}

void EIO_RemoteKeyboard::pressKeyboardButtons(QList < uint> &list) {
  if (list.isEmpty())
    return;

  foreach (const uint key, list) {
    if (key <= keyboardExt)
      device->pressKeyboardButton(key); else
      pressKeyboardExtendedButton(key);
  }
}

void EIO_RemoteKeyboard::releaseKeyboardButtons(QList < uint> &list) {
  if (list.isEmpty())
    return;

  foreach (const uint key, list)
    if (key <= keyboardExt)
      device->releaseKeyboardButton(key); else
      releaseKeyboardExtendedButton(key);
}
