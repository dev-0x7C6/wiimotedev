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
#include <iostream>

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
      EIOInfraredMouse *device = new EIOInfraredMouse(*virtualEvent);
      device->setId(settings.value("device", uint(1)).toLongLong());
      std::cout << settings.value("acc", false).toBool() << std::endl;
      device->config().setAccEnabled(settings.value("acc", false).toBool());
      device->config().setAccMultiX(settings.value("acc.multi.x", 4).toDouble());
      device->config().setAccMultiY(settings.value("acc.multi.y", 4).toDouble());
      device->config().setAccPowX(settings.value("acc.pow.x").toDouble());
      device->config().setAccPowY(settings.value("acc.pow.y").toDouble());
      device->config().setAccTimeout(settings.value("acc.timeout", 2000).toLongLong());
      device->config().setDeadzoneX(settings.value("deadzone.x", 30).toULongLong());
      device->config().setDeadzoneY(settings.value("deadzone.y", 20).toULongLong());
      connect(dbusDeviceEventsIface, &WiimotedevDeviceEvents::dbusVirtualCursorPosition, device, &EIOInfraredMouse::dbusVirtualCursorPosition);
      connect(dbusDeviceEventsIface, &WiimotedevDeviceEvents::dbusVirtualCursorLost, device, &EIOInfraredMouse::dbusVirtualCursorLost);
      settings.endGroup();
      EIOInfraredMouses << device;
      device->start();
    }
  }
}

void UInputProfileManager::unloadInfraredEvents() {
  foreach(EIOInfraredMouse * device, EIOInfraredMouses) {
    device->interrupt();
    device->wait();
    delete device;
  }
  EIOInfraredMouses.clear();
}
