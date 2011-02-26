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
  lastx1(0),
  lastx2(0),
  lasty1(0),
  lasty2(0),
  lastsx1(-1),
  lastsy1(-1),
  lastPointCount(0),
  lastX(0),
  lastY(0),
  accVectorX(0),
  accVectorY(0),
  accVectorXAccumulation(0),
  accVectorYAccumulation(0),
  accelerationTimeout(2000),
  deadzoneXRange(50),
  deadzoneYRange(20),
  aimHelperXRange(30),
  aimHelperYRange(20),
  aimHelperSensitivityXMultiplier(0.75),
  aimHelperSensitivityYMultiplier(0.75),
  sensitivityXPower(1.3),
  sensitivityYPower(1.0),
  sensitivityXMultiplier(8),
  sensitivityYMultiplier(8),
  calibrationState(CalibrationNeeded),
  interfaceEnabled(true),
  useAcceleration(true),
  useAimHelper(true),
  useAccelerationTimeout(true)
{
  connect(&accelerationClockTimeout, SIGNAL(timeout()), this, SLOT(axisAccelerationTimeout()));
  if (useAcceleration && accelerationTimeout)
    accelerationClockTimeout.start(accelerationTimeout);
  memset(&wiimote_acc, 0, sizeof(wiimote_acc));
}

InfraredVirtualMouse::~InfraredVirtualMouse()
{
  accelerationClockTimeout.stop();
  disconnect(&accelerationClockTimeout, 0, 0, 0);
}


void InfraredVirtualMouse::setDeviceId(int _id) {
  id = _id;
}

void InfraredVirtualMouse::setInterfaceEnabled(bool enabled) {
  if (interfaceEnabled = enabled)
    calibrationState = CalibrationNeeded;
}

void InfraredVirtualMouse::setAccelerationTimeoutValue(int value) {
  if (accelerationTimeout = value)
      accelerationClockTimeout.start(accelerationTimeout);
}

void InfraredVirtualMouse::setAimHelperXRange(int value) {
  aimHelperXRange = value;
}

void InfraredVirtualMouse::setAimHelperYRange(int value) {
  aimHelperYRange = value;
}

void InfraredVirtualMouse::setDeadzoneXRange(int value) {
  deadzoneXRange = value;
}

void InfraredVirtualMouse::setDeadzoneYRange(int value) {
  deadzoneYRange = value;
}

void InfraredVirtualMouse::setAimHelperSensitivityXMultiplier(double sensitivity) {
  aimHelperSensitivityXMultiplier = sensitivity;
}

void InfraredVirtualMouse::setAimHelperSensitivityYMultiplier(double sensitivity) {
  aimHelperSensitivityYMultiplier = sensitivity;
}

void InfraredVirtualMouse::setAccelerationSensitivityXPower(double sensitivity) {
  sensitivityXPower = sensitivity;
}

void InfraredVirtualMouse::setAccelerationSensitivityYPower(double sensitivity) {
  sensitivityYPower = sensitivity;
}

void InfraredVirtualMouse::setAccelerationSensitivityXMultiplier(double sensitivity) {
  sensitivityXMultiplier = sensitivity;
}

void InfraredVirtualMouse::setAccelerationSensitivityYMultiplier(double sensitivity) {
  sensitivityYMultiplier = sensitivity;
}

void InfraredVirtualMouse::setAccelerationFeatureEnabled(bool enabled) {
  useAcceleration = enabled;
}

void InfraredVirtualMouse::setAimHelperFeatureEnabled(bool enabled) {
  useAimHelper = enabled;
}

void InfraredVirtualMouse::setAccelerationTimeoutFeatureEnabled(bool enabled) {
  if (accelerationTimeout = enabled)
    accelerationClockTimeout.start(accelerationTimeout);
}

void InfraredVirtualMouse::dbusWiimoteAcc(quint32 _id, const accdata &table)
{
  if (id != _id)
    return;

  memcpy(&wiimote_acc, &table, sizeof(table));
}

void InfraredVirtualMouse::dbusWiimoteInfrared(quint32 _id, QList< irpoint> points)
{
  if ((id != _id) || (!interfaceEnabled))
    return;

  qint16 x1, x2, y1, y2, sx1, sy1;

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

  double ax, ay, x, y, moveX, moveY, bounceX = 0, bounceY = 0;

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

  if (useAcceleration) {
    useAccelerationTimeout = false;
    if (ax < -deadzoneXRange || ax > deadzoneXRange) {
      if (ax < -deadzoneXRange) ax += deadzoneXRange;
      if (ax > deadzoneXRange) ax -= deadzoneXRange;
      bool invert = (ax > 0.0);
      if (!invert) ax *= -1;
      bounceX = (ax / (512.0 -  deadzoneXRange));
      accVectorX = pow(bounceX * sensitivityXMultiplier, sensitivityXPower);
      if (accVectorX >= 0.0 && invert) accVectorX = accVectorX * -1;
    } else
      accVectorX = 0;

    if (ay < -deadzoneYRange || ay > deadzoneYRange) {
      if (ay < -deadzoneYRange) ay += deadzoneYRange;
      if (ay > deadzoneYRange) ay -= deadzoneYRange;
      bool invert = (ay > 0.0);
      if (!invert) ay *= -1;
      bounceY = (ay / (384.0 -  deadzoneYRange));
      accVectorY = pow(bounceY * sensitivityYMultiplier, sensitivityYPower);
      if (accVectorY >= 0.0 && invert) accVectorY = accVectorY * -1;
    } else
      accVectorY = 0;
  }

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

  axisAccelerationX();
  axisAccelerationY();

  lastPoints = points;
}

void InfraredVirtualMouse::axisAccelerationX()
{
  if (!useAccelerationTimeout) {
    accVectorXAccumulation += accVectorX;
    device->moveMousePointerRel(accVectorXAccumulation, 0);
    accVectorXAccumulation -= static_cast< int>(accVectorXAccumulation);
  }
}

void InfraredVirtualMouse::axisAccelerationY()
{
  if (!useAccelerationTimeout) {
    accVectorYAccumulation += accVectorY;
    device->moveMousePointerRel(0, accVectorYAccumulation);
    accVectorYAccumulation -= static_cast< int>(accVectorYAccumulation);
  }
}

void InfraredVirtualMouse::axisAccelerationTimeout()
{
  useAccelerationTimeout = true;
}

