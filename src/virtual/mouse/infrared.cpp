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
  accelerationClockTimeout.setInterval(10);
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
  accelerationTimeout = enabled;
}

void InfraredVirtualMouse::dbusWiimoteAcc(quint32 _id, const accdata &table)
{
  if (id != _id)
    return;

  memcpy(&wiimote_acc, &table, sizeof(table));
}

void InfraredVirtualMouse::dbusVirtualCursorLost(quint32 _id) {
  if ((id != _id) || (!interfaceEnabled))
    return;

   accelerationTimeoutValue = 0;
  if (useAcceleration && accelerationTimeout)
    accelerationClockTimeout.start();
}

void InfraredVirtualMouse::dbusVirtualCursorPosition(quint32 _id, double x, double y, double size, double angle) {
  if ((id != _id) || (!interfaceEnabled))
    return;

  if (lastX == -1.0) lastX = x;
  if (lastY == -1.0) lastY = y;
  double moveX = lastX - x;
  double moveY = lastY - y;
  lastX = x;
  lastY = y;

  if (useAcceleration) {
    useAccelerationTimeout = false;
    if (x < -deadzoneXRange || x > deadzoneXRange) {
      if (x < -deadzoneXRange) x += deadzoneXRange;
      if (x > deadzoneXRange) x -= deadzoneXRange;
      bool invert = (x > 0.0);
      if (!invert) x *= -1;
      double bounceX = (x / (512.0 -  deadzoneXRange));
      accVectorX = pow(bounceX * sensitivityXMultiplier, sensitivityXPower);
      if (accVectorX >= 0.0 && invert) accVectorX = accVectorX * -1;
    } else
      accVectorX = 0;

    if (y < -deadzoneYRange || y > deadzoneYRange) {
      if (y < -deadzoneYRange) y += deadzoneYRange;
      if (y > deadzoneYRange) y -= deadzoneYRange;
      bool invert = (y > 0.0);
      if (!invert) y *= -1;
      double bounceY = (y / (384.0 -  deadzoneYRange));
      accVectorY = pow(bounceY * sensitivityYMultiplier, sensitivityYPower);
      if (accVectorY >= 0.0 && invert) accVectorY = accVectorY * -1;
    } else
      accVectorY = 0;
  }

  if (useAimHelper) {
    int helperX = moveX * aimHelperSensitivityXMultiplier;
    int helperY = moveY * aimHelperSensitivityYMultiplier;
    if (x < -aimHelperXRange || x > aimHelperXRange)
      helperX = moveX;
    if (y < -aimHelperYRange || y > aimHelperYRange)
      helperY = moveY;
    device->moveMousePointerRel(helperX, helperY);
  } else {
    device->moveMousePointerRel(moveX, moveY);
  }

  accelerationClockTimeout.stop();
  axisAccelerationX();
  axisAccelerationY();
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
   accelerationTimeoutValue += accelerationClockTimeout.interval();
   if (accelerationTimeoutValue <= accelerationTimeout) {
     axisAccelerationX();
     axisAccelerationY();
   } else
     accelerationClockTimeout.stop();
}

