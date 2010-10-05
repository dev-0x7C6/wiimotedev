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

#include "classes/hashcompare.h"
#include "uinput/manager.h"

extern QMap < QString, quint32> scancodes;

enum KeyboardExtension {
  keyboardExt = 0xFFFF,
  keyboardExtMouseHWheelUp,
  keyboardExtMouseHWheelDown,
  keyboardExtMouseVWheelUp,
  keyboardExtMouseVWheelDown
};


QList < quint32> UInputProfileManager::extractScancodes(QStringList list)
{
  QList < quint32> values;
  for (register int i = 0; i < list.count(); ++i)
    if (scancodes.value(list.at(i), QString(list.at(i)).toUInt()))
      values << scancodes.value(list.at(i), QString(list.at(i)).toUInt());
  return values;
}

void UInputProfileManager::loadKeyboardEvents(QSettings &settings) {
  unloadKeyboardEvents();

  QMap < QString, quint8> keyboardConfigList;
  keyboardConfigList["keyboard-bit"] = HashCompare<QString, quint8>::BitCompare;
  keyboardConfigList["keyboard"] = HashCompare<QString, quint8>::BitCompare;
  keyboardConfigList["keyboard-equal"] = HashCompare<QString, quint8>::EqualCompare;
  keyboardConfigList["keyboard-notequal"] = HashCompare<QString, quint8>::NotEqualCompare;

  QMapIterator < QString, quint8> map(keyboardConfigList);

  while (map.hasNext()) {
    map.next();
    settings.beginGroup(map.key());
    foreach (const QString &string, settings.allKeys()) {
      KeyboardAction *action = new KeyboardAction;
      action->event = extractDeviceEvent(string);
      action->keys = extractScancodes(settings.value(string, QStringList()).toStringList());
      action->pushed = false;
      action->alghoritm = map.value();
      keyboardActions << action;
    }
    settings.endGroup();
  }

  disableKeyboardModule = keyboardActions.isEmpty();
}

void UInputProfileManager::unloadKeyboardEvents() {
  disableKeyboardModule = true;

  if (keyboardActions.isEmpty())
    return;

  foreach (KeyboardAction *action, keyboardActions)
    delete action;

  keyboardActions.clear();
}

void UInputProfileManager::processKeyboardEvents() {
  if (keyboardActions.isEmpty() || disableKeyboardModule)
    return;

  HashCompare<quint32, quint64> compare;

  foreach (KeyboardAction *action, keyboardActions) {
    if (action->event.isEmpty())
      continue;

    bool matched = compare.compare(&action->event, &lastWiiremoteButtons, action->alghoritm);

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

void UInputProfileManager::pressKeyboardExtendedButton(quint32 key) {
  switch (key) {
  case keyboardExtMouseHWheelUp:
    virtualEvent->moveMouseHWheel(1);
    break;
  case keyboardExtMouseHWheelDown:
    virtualEvent->moveMouseHWheel(-1);
    break;
  case keyboardExtMouseVWheelUp:
    virtualEvent->moveMouseVWheel(1);
    break;
  case keyboardExtMouseVWheelDown:
    virtualEvent->moveMouseVWheel(-1);
    break;
  }
}

void UInputProfileManager::releaseKeyboardExtendedButton(quint32 key) {

}

void UInputProfileManager::pressKeyboardButtons(QList < quint32> &list) {
  if (list.isEmpty())
    return;

  foreach (const quint32 key, list)
    if (key <= keyboardExt)
      virtualEvent->pressKeyboardButton(key); else
      pressKeyboardExtendedButton(key);
}

void UInputProfileManager::releaseKeyboardButtons(QList < quint32> &list) {
  if (list.isEmpty())
    return;

  foreach (const quint32 key, list)
    if (key <= keyboardExt)
      virtualEvent->releaseKeyboardButton(key); else
      releaseKeyboardExtendedButton(key);
}


