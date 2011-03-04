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

void UInputProfileManager::loadInfraredEvents(QSettings &settings) {
  unloadInfraredEvents();

  foreach (const QString &key, settings.childGroups()) {
    if (settings.value(QString("%1/module").arg(key), QString()).toString().toLower() == "infrared" ||
        settings.value(QString("%1/module").arg(key), QString()).toString().toLower() == "mouse") {
      settings.beginGroup(key);
      InfraredVirtualMouse *mouse = new InfraredVirtualMouse(virtualEvent, settings.value("assignWiimote", quint32(1)).toLongLong());
      mouse->setAccelerationFeatureEnabled(settings.value("accelerationFeature", bool(false)).toBool());
      mouse->setAccelerationSensitivityXMultiplier(settings.value("accelerationSensitivityXMultiplier", double(8.0)).toDouble());
      mouse->setAccelerationSensitivityYMultiplier(settings.value("accelerationSensitivityYMultiplier", double(8.0)).toDouble());
      mouse->setAccelerationSensitivityXPower(settings.value("accelerationSensitivityXPower", double(1.1)).toDouble());
      mouse->setAccelerationSensitivityYPower(settings.value("accelerationSensitivityYPower", double(1.0)).toDouble());
      mouse->setAccelerationTimeoutFeatureEnabled(settings.value("accelerationTimeoutFeature", bool(true)).toBool());
      mouse->setAccelerationTimeoutValue(settings.value("accelerationTimeoutValue", int(2000)).toULongLong());
      mouse->setAimHelperFeatureEnabled(settings.value("aimHelperFeature", bool(false)).toBool());
      mouse->setAimHelperSensitivityXMultiplier(settings.value("aimHelperSensitivityXMultiplier", double(0.75)).toDouble());
      mouse->setAimHelperSensitivityYMultiplier(settings.value("aimHelperSensitivityYMultiplier", double(0.75)).toDouble());
      mouse->setAimHelperXRange(settings.value("aimHelperXRange", int(20)).toULongLong());
      mouse->setAimHelperYRange(settings.value("aimHelperYRange", int(15)).toULongLong());
      mouse->setDeadzoneXRange(settings.value("deadzoneXRange", int(30)).toULongLong());
      mouse->setDeadzoneYRange(settings.value("deadzoneYRange", int(20)).toULongLong());
      mouse->setInterfaceEnabled(true);
      connect(dbusDeviceEventsIface, SIGNAL(dbusVirtualCursorPosition(quint32,double,double,double,double)), mouse, SLOT(dbusVirtualCursorPosition(quint32,double,double,double,double)));
      connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteAcc(quint32,accdata)), mouse, SLOT(dbusWiimoteAcc(quint32,accdata)));
      settings.endGroup();
      virtualMouses << mouse;
    }
  }
}

void UInputProfileManager::unloadInfraredEvents() {
  foreach (InfraredVirtualMouse *mouse, virtualMouses)
    delete mouse;
  virtualMouses.clear();
}
