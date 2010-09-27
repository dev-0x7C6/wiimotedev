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

#include "uinput/manager.h"

void UInputProfileManager::loadGamepadEvents(QSettings &settings) {
  settings.beginGroup("gamepad");
  disableWiimoteGamepadModule = !settings.value("configureWiimoteAsGamepad", false).toBool();
  disableClassicGamepadModule = !settings.value("configureClassicAsGamepad", false).toBool();

  if (!disableWiimoteGamepadModule)
    virtualWiimoteGamepad->uinput_open();

  if (!disableClassicGamepadModule)
    virtualClassicGamepad->uinput_open();
}

void UInputProfileManager::unloadGamepadEvents() {
  if (!disableWiimoteGamepadModule)
    virtualWiimoteGamepad->uinput_close();

  if (!disableClassicGamepadModule)
    virtualClassicGamepad->uinput_close();
}

void UInputProfileManager::dbusWiimoteAcc(quint32 id, struct accdata acc) {
  if (disableWiimoteGamepadModule)
    return;

  virtualWiimoteGamepad->wiimoteAcc(id, acc);
}

void UInputProfileManager::dbusWiimoteButtons(quint32 id, quint64 buttons) {
  if (disableWiimoteGamepadModule)
    return;

  virtualWiimoteGamepad->wiimoteButtons(id, buttons);
}

void UInputProfileManager::dbusNunchukAcc(quint32 id, struct accdata acc) {
  if (disableWiimoteGamepadModule)
    return;

  virtualWiimoteGamepad->nunchukAcc(id, acc);
}

void UInputProfileManager::dbusNunchukButtons(quint32 id, quint64 buttons) {
  if (disableWiimoteGamepadModule)
    return;

  virtualWiimoteGamepad->nunchukButtons(id, buttons);
}

void UInputProfileManager::dbusNunchukStick(quint32 id, struct stickdata stick) {
  if (disableWiimoteGamepadModule)
    return;

  virtualWiimoteGamepad->nunchukStick(id, stick.x, 0xFF - stick.y);
}

void UInputProfileManager::dbusClassicControllerButtons(quint32 id, quint64 buttons) {
  if (disableClassicGamepadModule)
    return;

  virtualClassicGamepad->classicButtons(id, buttons);
}

void UInputProfileManager::dbusClassicControllerLStick(quint32 id, struct stickdata stick) {
  if (disableClassicGamepadModule)
    return;

  virtualClassicGamepad->classicLStick(id, stick.x, 0x3F - stick.y);
}

void UInputProfileManager::dbusClassicControllerRStick(quint32 id, struct stickdata stick) {
  if (disableClassicGamepadModule)
    return;

  virtualClassicGamepad->classicRStick(id, stick.x, 0x1F - stick.y);
}
