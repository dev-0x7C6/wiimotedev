/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#include "manager.h"

void UInputProfileManager::setupClassicJoystick(uint assign, const QString &name, QSettings &settings) {
  EIO_ClassicJoystick *device = new EIO_ClassicJoystick(name, assign);
  device->setDpadInvertX(settings.value("DStickInvertX", 0x00).toBool());
  device->setDpadInvertY(settings.value("DStickInvertY", 0x00).toBool());
  device->setLStickInvertX(settings.value("LStickInvertX", 0x00).toBool());
  device->setLStickInvertY(settings.value("LStickInvertY", 0x00).toBool());
  device->setRStickInvertX(settings.value("RStickInvertX", 0x00).toBool());
  device->setRStickInvertY(settings.value("RStickInvertY", 0x00).toBool());
  device->setReportButtons(settings.value("ReportButtons", 0x01).toBool());
  device->setReportDStick(settings.value("ReportDStick", 0x01).toBool());
  device->setReportLStick(settings.value("ReportLStick", 0x01).toBool());
  device->setReportRStick(settings.value("ReportRStick", 0x01).toBool());

  if (device->create())
    EIO_ClassicJoysticks << device; else
    delete device;
}

void UInputProfileManager::setupWiimoteJoystick(uint assign, const QString &name, QSettings &settings) {
  EIO_WiimoteJoystick *device = new EIO_WiimoteJoystick(name, assign,
                                                          EIO_WiimoteJoystick::DPadPositionSwitchable,
                                                          EIO_WiimoteJoystick::GamepadVertical);
  device->setDStickInvertX(settings.value("DStickInvertX", 0x00).toBool());
  device->setDStickInvertY(settings.value("DStickInvertY", 0x00).toBool());
  device->setHomeSwitchPosition(settings.value("PositionSwitchable", 0x01).toBool());
  device->setReportButtons(settings.value("ReportButtons", 0x01).toBool());
  device->setReportDStick(settings.value("ReportDStick", 0x01).toBool());
  device->setReportPitch(settings.value("ReportPitch", 0x01).toBool());
  device->setReportRoll(settings.value("ReportRoll", 0x01).toBool());

  if (device->create())
    EIO_WiimoteJoysticks << device; else
    delete device;
}

void UInputProfileManager::setupNunchukJoystick(uint assign, const QString &name, QSettings &settings) {
  EIO_NunchukJoystick *device = new EIO_NunchukJoystick(name, assign);
  device->setStickInvertX(settings.value("DStickInvertX", 0x00).toBool());
  device->setStickInvertY(settings.value("DStickInvertY", 0x00).toBool());
  device->setReportButtons(settings.value("ReportButtons", 0x01).toBool());
  device->setReportStick(settings.value("ReportDStick", 0x01).toBool());
  device->setReportPitch(settings.value("ReportPitch", 0x01).toBool());
  device->setReportRoll(settings.value("ReportRoll", 0x01).toBool());

  if (device->create())
    EIO_NunchukJoysticks << device; else
    delete device;
}

void UInputProfileManager::assignJoystickEvents(const QString &key, QSettings &settings) {
  settings.beginGroup(key);

  uint assign = settings.value("assign").toULongLong();
  QString device = settings.value("device").toString().toLower();
  QString name = settings.value("name").toString();

  if (device == QString::fromUtf8("classic"))
    setupClassicJoystick(assign, name, settings); else
  if (device == QString::fromUtf8("nunchuk"))
    setupNunchukJoystick(assign, name, settings); else
  if (device == QString::fromUtf8("wiimote"))
    setupWiimoteJoystick(assign, name, settings);

  settings.endGroup();
}

void UInputProfileManager::freeJoystickEvents() {
  foreach (EIO_ClassicJoystick *device, EIO_ClassicJoysticks) {
    device->uinput_close();
    delete device;
  }

  foreach (EIO_NunchukJoystick *device, EIO_NunchukJoysticks) {
    device->uinput_close();
    delete device;
  }

  foreach (EIO_WiimoteJoystick *device, EIO_WiimoteJoysticks) {
    device->uinput_close();
    delete device;
  }

  EIO_ClassicJoysticks.clear();
  EIO_NunchukJoysticks.clear();
  EIO_WiimoteJoysticks.clear();
}

void UInputProfileManager::dbusWiimoteAcc(uint id, struct accdata acc) { 
  for (register int i = 0; i < EIO_WiimoteJoysticks.count(); ++i)
    if (EIO_WiimoteJoysticks[i]->assign() == id) {
      EIO_WiimoteJoysticks[i]->setWiimoteAcc(acc.pitch, acc.roll);
      return;
    }
}

void UInputProfileManager::dbusWiimoteButtons(uint id, uint64 buttons) { 
  for (register int i = 0; i < EIO_WiimoteJoysticks.count(); ++i)
    if (EIO_WiimoteJoysticks[i]->assign() == id) {
      EIO_WiimoteJoysticks[i]->setWiimoteButtons(buttons);
      return;
    }
}

void UInputProfileManager::dbusNunchukAcc(uint id, struct accdata acc) {
  for (register int i = 0; i < EIO_NunchukJoysticks.count(); ++i)
    if (EIO_NunchukJoysticks[i]->assign() == id) {
      EIO_NunchukJoysticks[i]->setNunchukAcc(acc.pitch, acc.roll);
      return;
    }
}

void UInputProfileManager::dbusNunchukButtons(uint id, uint64 buttons) {
  for (register int i = 0; i < EIO_NunchukJoysticks.count(); ++i)
    if (EIO_NunchukJoysticks[i]->assign() == id) {
      EIO_NunchukJoysticks[i]->setNunchukButtons(buttons);
      return;
    }
}

void UInputProfileManager::dbusNunchukStick(uint id, struct stickdata stick) {
  for (register int i = 0; i < EIO_NunchukJoysticks.count(); ++i)
    if (EIO_NunchukJoysticks[i]->assign() == id) {
      EIO_NunchukJoysticks[i]->setNunchukStick(stick.x, 0xFF - stick.y);
      return;
    }
}

void UInputProfileManager::dbusClassicControllerButtons(uint id, uint64 buttons) {  
  for (register int i = 0; i < EIO_ClassicJoysticks.count(); ++i)
    if (EIO_ClassicJoysticks[i]->assign() == id) {
      EIO_ClassicJoysticks[i]->setButtons(buttons);
      return;
    }
}

void UInputProfileManager::dbusClassicControllerLStick(uint id, struct stickdata stick) {
  for (register int i = 0; i < EIO_ClassicJoysticks.count(); ++i)
    if (EIO_ClassicJoysticks[i]->assign() == id) {
      EIO_ClassicJoysticks[i]->setStick(EIO_ClassicJoystick::LeftStick, stick.x, stick.y);
      return;
    }
}

void UInputProfileManager::dbusClassicControllerRStick(uint id, struct stickdata stick) {
  for (register int i = 0; i < EIO_ClassicJoysticks.count(); ++i)
    if (EIO_ClassicJoysticks[i]->assign() == id) {
      EIO_ClassicJoysticks[i]->setStick(EIO_ClassicJoystick::RightStick, stick.x, stick.y);
      return;
    }
}
