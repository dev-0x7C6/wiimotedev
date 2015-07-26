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

#include "eiomanager/manager.h"

void UInputProfileManager::setupInfraredMouse(uint assing, const QString &name, QSettings &settings) {
}

void UInputProfileManager::assignInfraredEvents(const QString &key, QSettings &settings) {
  settings.beginGroup(key);
  uint assign = settings.value("assign").toULongLong();
  QString name = settings.value("name").toString();
  setupClassicJoystick(assign, name, settings);
  settings.endGroup();
}

void UInputProfileManager::loadInfraredEvents(QSettings &settings) {
  unloadInfraredEvents();
  foreach(const QString & key, settings.childGroups()) {
    if (settings.value(QString("%1/module").arg(key), QString()).toString().toLower() == "infrared" ||
        settings.value(QString("%1/module").arg(key), QString()).toString().toLower() == "mouse") {
      settings.beginGroup(key);
      EIOInfraredMouse *device = new EIOInfraredMouse(virtualEvent, settings.value("assignWiimote", uint(1)).toLongLong());
      device->setAccelerationFeatureEnabled(settings.value("accelerationFeature", bool(false)).toBool());
      device->setAccelerationSensitivityXMultiplier(settings.value("accelerationSensitivityXMultiplier", double(8.0)).toDouble());
      device->setAccelerationSensitivityYMultiplier(settings.value("accelerationSensitivityYMultiplier", double(8.0)).toDouble());
      device->setAccelerationSensitivityXPower(settings.value("accelerationSensitivityXPower", double(1.1)).toDouble());
      device->setAccelerationSensitivityYPower(settings.value("accelerationSensitivityYPower", double(1.0)).toDouble());
      device->setAccelerationTimeoutFeatureEnabled(settings.value("accelerationTimeoutFeature", bool(true)).toBool());
      device->setAccelerationTimeoutValue(settings.value("accelerationTimeoutValue", int(2000)).toULongLong());
      device->setAimHelperFeatureEnabled(settings.value("aimHelperFeature", bool(false)).toBool());
      device->setAimHelperSensitivityXMultiplier(settings.value("aimHelperSensitivityXMultiplier", double(0.75)).toDouble());
      device->setAimHelperSensitivityYMultiplier(settings.value("aimHelperSensitivityYMultiplier", double(0.75)).toDouble());
      device->setAimHelperXRange(settings.value("aimHelperXRange", int(20)).toULongLong());
      device->setAimHelperYRange(settings.value("aimHelperYRange", int(15)).toULongLong());
      device->setDeadzoneXRange(settings.value("deadzoneXRange", int(30)).toULongLong());
      device->setDeadzoneYRange(settings.value("deadzoneYRange", int(20)).toULongLong());
      device->setInterfaceEnabled(true);
      connect(dbusDeviceEventsIface, &WiimotedevDeviceEvents::dbusVirtualCursorPosition, device, &EIOInfraredMouse::dbusVirtualCursorPosition);
      connect(dbusDeviceEventsIface, &WiimotedevDeviceEvents::dbusVirtualCursorLost, device, &EIOInfraredMouse::dbusVirtualCursorLost);
      connect(dbusDeviceEventsIface, &WiimotedevDeviceEvents::dbusWiimoteAcc, device, &EIOInfraredMouse::dbusWiimoteAcc);
      settings.endGroup();
      EIOInfraredMouses << device;
    }
  }
}

void UInputProfileManager::unloadInfraredEvents() {
  foreach(EIOInfraredMouse * device, EIOInfraredMouses)
  delete device;
  EIOInfraredMouses.clear();
}
