/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2014  Bartłomiej Burdukiewicz                               *
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

#include "eioinfraredmouse.h"
#include <math.h>
#include <QCursor>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

EIO_InfraredMouse::EIO_InfraredMouse(EIO_EventDevice *device, uint id) :
  device(device),
  id(id),
  mode(EIO_InfraredMouse::RelativeDevice),
  lastX(-0xFFFF),
  lastY(-0xFFFF),
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
  interfaceEnabled(true),
  useAcceleration(true),
  useAimHelper(true),
  useAccelerationTimeout(true)
{
  accelerationClockTimeout.setInterval(5);

  if (mode == EIO_InfraredMouse::AbsoluteDevice)  {
    connect(&accelerationClockTimeout, SIGNAL(timeout()), this, SLOT(axisAccelerationTimeout()));

  }
  memset(&wiimote_acc, 0, sizeof(wiimote_acc));

  moves[0] = -0xFFFF;
  moves[1] = -0xFFFF;
  moves[2] = 0;
  moves[3] = 0;
  moves[4] = 0;
  moves[5] = 0;
}

EIO_InfraredMouse::~EIO_InfraredMouse()
{
  accelerationClockTimeout.stop();
  disconnect(&accelerationClockTimeout, 0, 0, 0);
}

void EIO_InfraredMouse::setDeviceId(int _id) {
  id = _id;
}

void EIO_InfraredMouse::setInterfaceEnabled(bool enabled) {
  interfaceEnabled = enabled;
}

void EIO_InfraredMouse::setAccelerationTimeoutValue(int value) {
  accelerationTimeout = value;
}

void EIO_InfraredMouse::setAimHelperXRange(int value) {
  aimHelperXRange = value;
}

void EIO_InfraredMouse::setAimHelperYRange(int value) {
  aimHelperYRange = value;
}

void EIO_InfraredMouse::setDeadzoneXRange(int value) {
  deadzoneXRange = value;
}

void EIO_InfraredMouse::setDeadzoneYRange(int value) {
  deadzoneYRange = value;
}

void EIO_InfraredMouse::setAimHelperSensitivityXMultiplier(double sensitivity) {
  aimHelperSensitivityXMultiplier = sensitivity;
}

void EIO_InfraredMouse::setAimHelperSensitivityYMultiplier(double sensitivity) {
  aimHelperSensitivityYMultiplier = sensitivity;
}

void EIO_InfraredMouse::setAccelerationSensitivityXPower(double sensitivity) {
  sensitivityXPower = sensitivity;
}

void EIO_InfraredMouse::setAccelerationSensitivityYPower(double sensitivity) {
  sensitivityYPower = sensitivity;
}

void EIO_InfraredMouse::setAccelerationSensitivityXMultiplier(double sensitivity) {
  sensitivityXMultiplier = sensitivity;
}

void EIO_InfraredMouse::setAccelerationSensitivityYMultiplier(double sensitivity) {
  sensitivityYMultiplier = sensitivity;
}

void EIO_InfraredMouse::setAccelerationFeatureEnabled(bool enabled) {
  useAcceleration = enabled;
}

void EIO_InfraredMouse::setAimHelperFeatureEnabled(bool enabled) {
  useAimHelper = enabled;
}

void EIO_InfraredMouse::setAccelerationTimeoutFeatureEnabled(bool enabled) {
  accelerationTimeout = enabled;
}

void EIO_InfraredMouse::dbusWiimoteAcc(uint _id, const accdata &table)
{
  if (id != _id)
    return;

  memcpy(&wiimote_acc, &table, sizeof(table));
}

void EIO_InfraredMouse::dbusVirtualCursorLost(uint _id) {
  if ((id != _id) || (!interfaceEnabled))
    return;

   accelerationTimeoutValue = 0;
  if (useAcceleration && accelerationTimeout)
    accelerationClockTimeout.start();
}

void EIO_InfraredMouse::dbusVirtualCursorPosition(uint _id, double x, double y, double size, double angle) {
  if ((id != _id) || (!interfaceEnabled))
    return;

  if (mode == EIO_InfraredMouse::RelativeDevice) {
    if (moves[0] == -0xFFFF)
      moves[0] = x;
    if (moves[1] == -0xFFFF)
      moves[1] = y;
    moves[2] = moves[0] - x;
    moves[3] = moves[1] - y;
    moves[0] = x;
    moves[1] = y;

    for (register int i = 0; i < 5; ++i) {
      moves[4] += (moves[2] / 5.0);
      moves[5] += (moves[3] / 5.0);
      device->moveMousePointerRel(moves[4], moves[5]);
      moves[4] -= static_cast< int>(moves[4]);
      moves[5] -= static_cast< int>(moves[5]);
    }

    if (useAcceleration) {
      useAccelerationTimeout = false;
      if (x < -deadzoneXRange || x > deadzoneXRange) {
        if (x < -deadzoneXRange) x += deadzoneXRange;
        if (x > deadzoneXRange) x -= deadzoneXRange;
        bool invert = (x > 0.0);
        if (!invert) x *= -1;
        accVectorX = pow((x / (512.0 -  deadzoneXRange))* sensitivityXMultiplier, sensitivityXPower);
        if (accVectorX >= 0.0 && invert) accVectorX = accVectorX * -1;
      } else
        accVectorX = 0;

      if (y < -deadzoneYRange || y > deadzoneYRange) {
        if (y < -deadzoneYRange) y += deadzoneYRange;
        if (y > deadzoneYRange) y -= deadzoneYRange;
        bool invert = (y > 0.0);
        if (!invert) y *= -1;
        accVectorY = pow((y / (384.0 -  deadzoneYRange)) * sensitivityYMultiplier, sensitivityYPower);
        if (accVectorY >= 0.0 && invert) accVectorY = accVectorY * -1;
      } else
        accVectorY = 0;

      accelerationClockTimeout.stop();
    }

    axisAccelerationX();
    axisAccelerationY();
  }

  if (mode == EIO_InfraredMouse::AbsoluteDevice) {
    QRect rect = QApplication::desktop()->screenGeometry(1);

    x = x * (rect.width()/1024.0);
    y = y * (rect.height()/768.0);
    x += (512 *(rect.width()/1024.0));
    y += (384 *(rect.height()/768.0));

    QCursor::setPos(x, y);
  }
}

void EIO_InfraredMouse::axisAccelerationX()
{
  if (useAcceleration)
    accVectorXAccumulation += accVectorX;
  accVectorXAccumulation += moveX;
  device->moveMousePointerRel(accVectorXAccumulation, 0);
  accVectorXAccumulation -= static_cast< int>(accVectorXAccumulation);

  moveX = 0;
}

void EIO_InfraredMouse::axisAccelerationY()
{
  if (useAcceleration)
    accVectorYAccumulation += accVectorY;
  accVectorYAccumulation += moveY;
  device->moveMousePointerRel(0, accVectorYAccumulation);
  accVectorYAccumulation -= static_cast< int>(accVectorYAccumulation);

  moveY = 0;
}

void EIO_InfraredMouse::axisAccelerationTimeout()
{
  accelerationTimeoutValue += accelerationClockTimeout.interval();
  if (accelerationTimeoutValue <= accelerationTimeout) {
    axisAccelerationX();
    axisAccelerationY();
  } else
    accelerationClockTimeout.stop();
}

