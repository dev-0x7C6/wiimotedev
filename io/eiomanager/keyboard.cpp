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

#include "helper/hashcompare.h"
#include "eiomanager/manager.h"

extern QMap < QString, quint32> scancodes;

void UInputProfileManager::assignKeyboardEvents(const QString &key, QSettings &settings) {
  freeKeyboardEvents();

  settings.beginGroup(key);
  EIO_RemoteKeyboard *device = new EIO_RemoteKeyboard(virtualEvent);
  foreach (const QString &string, settings.allKeys()) {
    if (string.toLower() == "module")
      continue;

    KeyboardAction action;
    action.event = extractDeviceEvent(string);
    if (action.event.isEmpty()) {
      continue;
    }

    action.keys = extractScancodes(settings.value(string, QStringList()).toStringList());
    if (action.keys.isEmpty()) {
      continue;
    }

    action.pushed = false;
    device->addKeyboardAction(action);
  }

  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), device, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));

  settings.endGroup();
  EIO_RemoteKeyboards << device;
}


void UInputProfileManager::freeKeyboardEvents() {
  foreach (EIO_RemoteKeyboard *device, EIO_RemoteKeyboards)
    delete device;
  EIO_RemoteKeyboards.clear();
}
