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
  firstPoint(0),
  secondPoint(1),
  lastPointCount(0),
  accelerationTimeoutValue(2000),
  deadzoneXRange(50),
  deadzoneYRange(20),
  aimHelperXRange(50),
  aimHelperYRange(20),
  aimHelperSensitivityXMultiplier(0.5),
  aimHelperSensitivityYMultiplier(0.5),
  sensitivityXPower(1.1),
  sensitivityYPower(1.0),
  sensitivityXMultiplier(2),
  sensitivityYMultiplier(0.5),
  calibrationState(CalibrationNeeded),
  useAcceleration(true),
  useAimHelper(false),
  accelerationTimeout(true)
{
  connect(&axisClockX, SIGNAL(timeout()), this, SLOT(axisAccelerationX()));
  connect(&axisClockY, SIGNAL(timeout()), this, SLOT(axisAccelerationY()));
  connect(&accelerationClockTimeout, SIGNAL(timeout()), this, SLOT(axisAccelerationTimeout()));
  axisClockX.setInterval(1);
  axisClockY.setInterval(1);
  if (useAcceleration) {
    axisClockX.start();
    axisClockY.start();
    if (accelerationTimeoutValue)
      accelerationClockTimeout.start(accelerationTimeoutValue);
  }

  lastsx1 = -1;
  memset(&wiimote_acc, 0, sizeof(wiimote_acc));
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

int NWD(int a, int b)
{
    while( 1 )
    {
        a = a % b;
        if( a == 0 )
          return b;
        b = b % a;
        if( b == 0 )
          return a;
    }
}

double sabs(double x) {
  if (x >= 0.0) {
    if (x >= 1.0)
      return x; else
      return 1.0;
  } else {
    if (x <= -1.0)
      return -x; else
      return 1.0;
  }
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
    if (lastPoints.count() == 1)
      calibrationState = CalibrationNeeded;

    x1 = points.at(0).x;
    x2 = points.at(1).x;
    y1 = points.at(0).y;
    y2 = points.at(1).y;

    lastsx1 = -1;
    lastsy1 = -1;
    lastx1 = x1;
    lastx2 = x2;
    lasty1 = y1;
    lasty2 = y2;
  } else {
    if (lastPointCount == 0) {
      lastPoints = points;
      return;
    }

    sx1 = points.at(0).x;
    sy1 = points.at(0).y;

    if (lastsx1 == -1)
      lastsx1 = sx1;
    if (lastsy1 == -1)
      lastsy1 = sy1;

    x1 = lastx1 + (sx1 - lastsx1);
    x2 = lastx2 + (sx1 - lastsx1);
    y1 = lasty1 + (sy1 - lastsy1);
    y2 = lasty2 + (sy1 - lastsy1);
  }

  double p = -(atan2(y2 - y1, x2 - x1) - M_PI);

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

  double diff = cosp + cos(wiimote_acc.roll*(M_PI/180));
  if (diff < 0)
    diff *= -1;

  double ax, ay, x, y, moveX, moveY;

  if (calibrationState == CalibrationNeeded) {
    if (diff < 0.2)
      calibrationState = CalibrationInverted; else
      calibrationState = CalibrationNormal;
  }

  switch (calibrationState) {
  case CalibrationNormal:
    x = ((1024 - (sx*cosp - sy*sinp + 512*(1-cosp) + 384*sinp)));
    y = (((sx*sinp + sy*cosp - 512*sinp + 384*(1-cosp))));
    ax = 512.0 - x;
    ay = 384.0 - y;
    moveX = lastX - ax;
    moveY = lastY - ay;
    break;
  case CalibrationInverted:
    p = -(atan2(y1 - y2, x1 - x2) - M_PI);
    x = ((1024 - (sx*cosp - sy*sinp + 512*(1-cosp) + 384*sinp)));
    y = (((sx*sinp + sy*cosp - 512*sinp + 384*(1-cosp))));
    ax = (512.0 - x) *-1;
    ay = (384.0 - y) *-1;
    moveX = lastX - ax;
    moveY = lastY - ay;
    break;
  }


  if (lastX == -1.0) lastX = ax;
  if (lastY == -1.0) lastY = ay;
  lastX = ax;
  lastY = ay;
  lastPointCount = points.count();

  if (useAimHelper) {
    int helperX = moveX * aimHelperSensitivityXMultiplier;
    int helperY = moveY * aimHelperSensitivityYMultiplier;
    if (ax < -aimHelperXRange || ax > aimHelperXRange)
      helperX = moveX;
    if (ay < -aimHelperYRange || ay > aimHelperYRange)
      helperY = moveY;
    device->moveMousePointerRel(helperX, helperY);
  } else {
    device->moveMousePointerRel(moveX, moveY);
  }

  if (useAcceleration) {
    accelerationTimeout = false;
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
  }
  lastPoints = points;
}

void InfraredVirtualMouse::axisAccelerationX()
{
  if (!accelerationTimeout) {
    accVectorXAccumulation += accVectorX;
    device->moveMousePointerRel(accVectorXAccumulation, 0);
    accVectorXAccumulation -= static_cast< int>(accVectorXAccumulation);
  }
}

void InfraredVirtualMouse::axisAccelerationY()
{
  if (!accelerationTimeout) {
    accVectorYAccumulation += accVectorY;
    device->moveMousePointerRel(0, accVectorYAccumulation);
    accVectorYAccumulation -= static_cast< int>(accVectorYAccumulation);
  }
}

void InfraredVirtualMouse::axisAccelerationTimeout()
{
  accelerationTimeout = true;
}

