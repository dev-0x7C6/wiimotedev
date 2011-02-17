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

#include "virtual/mouse/infrared.h"
#include <math.h>
#include <QDebug>

InfraredVirtualMouse::InfraredVirtualMouse(UInputEvent *device, quint32 id) :
  device(device),
  id(id),
  lastPointCount(0),
  accelerationTimeout(2000),
  deadzoneXRange(100),
  deadzoneYRange(50),
  aimHelperXRange(20),
  aimHelperYRange(15),
  aimHelperSensitivityXMultiplier(0.2),
  aimHelperSensitivityYMultiplier(0.2),
  sensitivityXPower(1.3),
  sensitivityYPower(1.1),
  sensitivityXMultiplier(5),
  sensitivityYMultiplier(4),
  useAcceleration(true),
  useAimHelper(true)
{
  connect(&clock, SIGNAL(timeout()), this, SLOT(accel()));
  clock.setInterval(10);
  if (useAcceleration)
    clock.start();

  lastsx1 = -1;

}

InfraredVirtualMouse::~InfraredVirtualMouse()
{
}

void InfraredVirtualMouse::dbusWiimoteAcc(quint32 _id, const accdata &table)
{
  if (id != _id)
    return;

  memcpy(&wiimote_acc, &table, sizeof(table));
}

void InfraredVirtualMouse::dbusWiimoteInfrared(quint32 _id, QList< irpoint> points)
{
  if (id != _id)
    return;

  qint16 x1;
  qint16 x2;
  qint16 y1;
  qint16 y2;
  qint16 sx1;
  qint16 sy1;

  if (points.count() > 2)
    return;

  if (points.count() == 2) {
    x1 = points.at(0).x;
    x2 = points.at(1).x;
    y1 = points.at(0).y;
    y2 = points.at(1).y;
    lastsx1 = -1;
    lastx1 = x1;
    lastx2 = x2;
    lasty1 = y1;
    lasty2 = y2;
  } else {
    sx1 = points.at(0).x;

    if (lastsx1 == -1)
      lastsx1 = sx1;

    x1 = lastx1 + (sx1 - lastsx1);
    x2 = lastx2 + (sx1 - lastsx1);
    y1 = lasty1;
    y2 = lasty2;
  }



  qDebug() << "count: " << points.count() << "   "<<  x1 << "x" << y1 << "  " << x2 << "x" << y2;

  double roll = -wiimote_acc.roll;

  if (roll < 0)

  roll = 360 - wiimote_acc.roll;



  if (timeout.elapsed() > 40) {
    if (cos(roll*M_PI/180) > 0) {
      if (x2 < x1)
        order = 1; else
        order = 0;
    } else {
      if (x2 > x1)
        order = 1; else
        order = 0;
    }
  }
  timeout.start();

  double p;

  if (order == 1) {
    p = -(atan2(y2 - y1, x2 - x1)-M_PI);
  } else {
    p = -(atan2(y1 - y2, x1 - x2)-M_PI);
  }

#ifdef __amd64 // 64-bit processors only
  register double cosp = cos(p);
  register double sinp = sin(p);
#endif

#ifdef i386  // 32-bit processors
  register float cosp = cos(p);
  register float sinp = sin(p);
#endif

  register float sx = (x1 + x2) / 2.0;
  register float sy = (y1 + y2) / 2.0;
  double x = ((1024 - (sx*cosp - sy*sinp + 512*(1-cosp) + 384*sinp)));
  double y = (((sx*sinp + sy*cosp - 512*sinp + 384*(1-cosp))));
  double ax = 512.0 - x;
  double ay = 384.0 - y;

  if (lastX == -1.0) lastX = x;
  if (lastY == -1.0) lastY = y;

  if (useAimHelper) {
    int helperX = (x - lastX) * aimHelperSensitivityXMultiplier;
    int helperY = (y - lastY) * aimHelperSensitivityYMultiplier;
    if (ax < -aimHelperXRange || ax > aimHelperXRange)
      helperX = x - lastX;
    if (ay < -aimHelperYRange || ay > aimHelperYRange)
      helperY = y - lastY;
    device->moveMousePointerRel(helperX, helperY);
  } else
    device->moveMousePointerRel(x - lastX, y - lastY);

  lastX = x;
  lastY = y;
  lastPointCount = points.count();

  if (useAcceleration) {
    if (ax < -deadzoneXRange || ax > deadzoneXRange) {
      if (ax < -deadzoneXRange) ax += deadzoneXRange;
      if (ax > deadzoneXRange) ax -= deadzoneXRange;
      bool invert = (ax > 0.0);
      if (!invert) ax *= -1;
      accVectorX = pow((ax / (512.0 -  deadzoneXRange) * sensitivityXMultiplier), sensitivityXPower);
      if (accVectorX >= 0.0 && invert) accVectorX = accVectorX * -1;
    } else
      accVectorX = 0;

    if (ay < -deadzoneYRange || ay > deadzoneYRange) {
      if (ay < -deadzoneYRange) ay += deadzoneYRange;
      if (ay > deadzoneYRange) ay -= deadzoneYRange;
      bool invert = (ay > 0.0);
      if (!invert) ay *= -1;
      accVectorY = pow((ay / (384.0 -  deadzoneYRange) * sensitivityYMultiplier), sensitivityYPower);
      if (accVectorY >= 0.0 && invert) accVectorY = accVectorY * -1;
    } else
      accVectorY = 0;
    device->moveMousePointerRel(accVectorX, accVectorY);
  }
}

void InfraredVirtualMouse::accel()
{
  device->moveMousePointerRel(accVectorX, accVectorY);
}


