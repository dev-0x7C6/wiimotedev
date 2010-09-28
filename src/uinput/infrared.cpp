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

double acc_plus(int min, int max, int value, double sensitivy)
{
  if ((value <= min) || (value >= max))
    return 0.0;
  return ((value - min) / (double)(max - min)) * sensitivy;
}

double acc_minus(int max, int min, int value, double sensitivy)
{
  if ((value <= min) || (value >= max))
    return 0.0;
  return ((value - max) / (double)(min - max)) * -sensitivy;
}

void UInputProfileManager::loadInfraredEvents(QSettings &settings) {
  unloadInfraredEvents();

  settings.beginGroup("infrared");

  irWiimoteId = settings.value(profiles::infrared::wiimoteid, 0).toInt();
  irMode = settings.value(profiles::infrared::mode, 0).toInt();
  irAlghoritm = settings.value(profiles::infrared::alghoritm, 0).toInt();
  irXSensitivity = settings.value(profiles::infrared::sensx, 0).toDouble();
  irYSensitivity = settings.value(profiles::infrared::sensy, 0).toDouble();
  irXFreeZone = settings.value(profiles::infrared::freezonex, 0).toInt();
  irYFreeZone = settings.value(profiles::infrared::freezoney, 0).toInt();
  irTimeout = settings.value(profiles::infrared::timeout, 2000).toInt();
  irLatency = settings.value(profiles::infrared::latency, 8).toInt();
  irRange = settings.value(profiles::infrared::range, QRect(-512, -384, 1024, 768)).toRect();

  if (irWiimoteId) {
    infraredTimer.setInterval(irLatency);
    infraredTimer.start();
    infraredTimeout.setInterval(irTimeout);
    infraredTimeout.start();
    virtualAbsoluteMouse->uinput_open(irRange, true);
  }

  settings.endGroup();
}

void UInputProfileManager::unloadInfraredEvents() {
  moveX = 0;
  moveY = 0;
  timeout = false;

  infraredTimer.stop();
  infraredTimeout.stop();

//  virtualAbsoluteMouse->uinput_close();

}

void UInputProfileManager::dbusWiimoteInfrared(quint32 id, QList< irpoint> points) {
  if (id != irWiimoteId)
    return;

  timeout = false;

  if (irAlghoritm == mouseEmulationAlghoritm2points) {
    switch (points.count()) {
    case 4:
    case 3:
    case 2:
      x = (points.at(0).x + points.at(1).x) >> 1;
      y = (points.at(0).y + points.at(1).y) >> 1;
      break;
    case 1:
      break;

    }
  } else
    if (irAlghoritm == mouseEmulationAlghoritm1point) {
      x = points.at(0).x;
      y = points.at(0).y;
    }


  switch (irMode)
  {
  case mouseEmulationModeAbs:
    virtualAbsoluteMouse->moveMousePointerAbs(-(x - 0x0200), y - 0x0180);
    break;

  case mouseEmulationModeAcc:
    if (cursor.x() == 0) cursor.setX(x);
    if (cursor.y() == 0) cursor.setY(y);

    moveX = -((x) - cursor.x());
    moveY = (y) - cursor.y();

    cursor.setX(x);
    cursor.setY(y);

    moveX -= acc_plus(512 + irXFreeZone, 1024, cursor.x(), irXSensitivity);
    moveX -= acc_minus(512 - irXFreeZone, 0, cursor.x(), irXSensitivity);
    moveY += acc_plus(384 + irYFreeZone, 768, cursor.y(), irYSensitivity);
    moveY += acc_minus(384 - irYFreeZone, 0, cursor.y(), irYSensitivity);
  }
}

void UInputProfileManager::infraredAccSection()
{
  if (timeout)
    return;

  virtualEvent->moveMousePointerRel(moveX, moveY);
}
