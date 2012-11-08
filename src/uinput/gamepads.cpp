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

#include "uinput/manager.h"

void UInputProfileManager::loadGamepadEvents(QSettings &settings) {
  unloadGamepadEvents();

  settings.beginGroup("wiimote-gamepads");

  foreach (const QString key, settings.allKeys()) {
    bool valid;
    quint32 id = key.toUInt(&valid);
    if (!valid)
      continue;

    WiimoteGamepadDevice *device = new WiimoteGamepadDevice(settings.value(key, QString()).toString(),
                                                            WiimoteGamepadDevice::DPadPositionSwitchable,
                                                            WiimoteGamepadDevice::GamepadVertical);

    wiimoteGamepads[id] = device;
    device->uinput_open();
  }

  settings.endGroup();

  settings.beginGroup("classic-gamepads");

  QStringList list = settings.allKeys();
  list.sort();

  foreach (const QString key, list) {
    bool valid;
    quint32 id = key.toUInt(&valid);
    if (!valid)
      continue;

    ClassicGamepadDevice *device = new ClassicGamepadDevice(settings.value(key, QString()).toString());
    classicGamepads[id] = device;
    device->uinput_open();
  }

  settings.endGroup();
}

void UInputProfileManager::unloadGamepadEvents() {
  foreach (ClassicGamepadDevice *device, classicGamepads) {
    device->uinput_close();
    delete device;
  }

  foreach (WiimoteGamepadDevice *device, wiimoteGamepads) {
    device->uinput_close();
    delete device;
  }

  classicGamepads.clear();
  wiimoteGamepads.clear();
}

void UInputProfileManager::dbusWiimoteAcc(quint32 id, struct accdata acc) {
  WiimoteGamepadDevice *device = 0;
  if ((device = wiimoteGamepads.value(id, device)))
    device->setWiimoteAcc(acc.pitch, acc.roll);
}

void UInputProfileManager::dbusWiimoteButtons(quint32 id, quint64 buttons) {
  WiimoteGamepadDevice *device = 0;
  if ((device = wiimoteGamepads.value(id, device)))
    device->setWiimoteButtons(buttons);
}

void UInputProfileManager::dbusNunchukAcc(quint32 id, struct accdata acc) {
  WiimoteGamepadDevice *device = 0;
  if ((device = wiimoteGamepads.value(id, device)))
    device->setNunchukAcc(acc.pitch, acc.roll);
}

void UInputProfileManager::dbusNunchukButtons(quint32 id, quint64 buttons) {
  WiimoteGamepadDevice *device = 0;
  if ((device = wiimoteGamepads.value(id, device)))
    device->setNunchukButtons(buttons);
}

void UInputProfileManager::dbusNunchukStick(quint32 id, struct stickdata stick) {
  WiimoteGamepadDevice *device = 0;
  if ((device = wiimoteGamepads.value(id, device)))
    device->setNunchukStick(stick.x, 0xFF - stick.y);
}

void UInputProfileManager::dbusClassicControllerButtons(quint32 id, quint64 buttons) {
  ClassicGamepadDevice *device = 0;
  if ((device = classicGamepads.value(id, device)))
    device->setButtons(buttons);
}

void UInputProfileManager::dbusClassicControllerLStick(quint32 id, struct stickdata stick) {
  ClassicGamepadDevice *device = 0;
  if ((device = classicGamepads.value(id, device)))
    device->setStick(ClassicGamepadDevice::LeftStick, stick.x, /*0x3F -*/ stick.y);
}

void UInputProfileManager::dbusClassicControllerRStick(quint32 id, struct stickdata stick) {
  ClassicGamepadDevice *device = 0;
  if ((device = classicGamepads.value(id, device)))
    device->setStick(ClassicGamepadDevice::RightStick, stick.x, /*0x1F - */stick.y);
}
