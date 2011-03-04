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


#ifndef INFRARED_VIRTUAL_MOUSE_H
#define INFRARED_VIRTUAL_MOUSE_H

#include "devices/eventdevice.h"
#include "headers/consts.h"

#include <QTimer>
#include <QTime>
#include <QCursor>

class InfraredVirtualMouse: public QObject
{
  Q_OBJECT
//general
  UInputEvent *device;
  quint32 id;

// timers
  QTimer accelerationClockTimeout;
  int accelerationTimeoutValue;

//variables
  int calibrationState;
  qint16 lastx1;
  qint16 lastx2;
  qint16 lasty1;
  qint16 lasty2;
  qint16 lastsx1;
  qint16 lastsy1;
  int lastPointCount;
  double lastX;
  double lastY;
  double accVectorX;
  double accVectorY;
  double accVectorXAccumulation;
  double accVectorYAccumulation;
  struct accdata wiimote_acc;

  QList< irpoint> lastPoints;

//settings
  int accelerationTimeout;
  int aimHelperXRange;
  int aimHelperYRange;
  int deadzoneXRange;
  int deadzoneYRange;
  double aimHelperSensitivityXMultiplier;
  double aimHelperSensitivityYMultiplier;
  double sensitivityXPower;
  double sensitivityYPower;
  double sensitivityXMultiplier;
  double sensitivityYMultiplier;


  bool interfaceEnabled;
  bool useAcceleration;
  bool useAimHelper;
  bool useAccelerationTimeout;

public:
  InfraredVirtualMouse(UInputEvent *device, quint32 id);
 ~InfraredVirtualMouse();

  enum Calibration {
    CalibrationNeeded,
    CalibrationNormal,
    CalibrationInverted
  };

  void setDeviceId(int);
  void setInterfaceEnabled(bool);
  void setAccelerationTimeoutValue(int);
  void setAimHelperXRange(int);
  void setAimHelperYRange(int);
  void setDeadzoneXRange(int);
  void setDeadzoneYRange(int);
  void setAimHelperSensitivityXMultiplier(double);
  void setAimHelperSensitivityYMultiplier(double);
  void setAccelerationSensitivityXPower(double);
  void setAccelerationSensitivityYPower(double);
  void setAccelerationSensitivityXMultiplier(double);
  void setAccelerationSensitivityYMultiplier(double);
  void setAccelerationFeatureEnabled(bool);
  void setAimHelperFeatureEnabled(bool);
  void setAccelerationTimeoutFeatureEnabled(bool);

public Q_SLOTS:
  void dbusVirtualCursorPosition(quint32, double, double, double, double);
  void dbusVirtualCursorLost(quint32);
  void dbusWiimoteAcc(quint32, const accdata&);

private:
  void axisAccelerationX();
  void axisAccelerationY();

private Q_SLOTS:
  void axisAccelerationTimeout();


};
#endif // INFRARED_VIRTUAL_MOUSE_H
