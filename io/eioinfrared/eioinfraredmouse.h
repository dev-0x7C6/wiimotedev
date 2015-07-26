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


#ifndef INFRARED_VIRTUAL_MOUSE_H
#define INFRARED_VIRTUAL_MOUSE_H

#include "eiobase/eioeventdevice.h"
#include "linux/usr/include/wiimotedev/consts.h"

#include <QTimer>
#include <QTime>

class EIOInfraredMouse: public QObject {
  Q_OBJECT
  //general
  EIOEventDevice *device;
  uint id;
  uint8 mode;

  // timers
  QTimer accelerationClockTimeout;
  int accelerationTimeoutValue;

  //variables
  double moves[6];
  double moveX;
  double moveY;
  double lastX;
  double lastY;
  double accVectorX;
  double accVectorY;
  double accVectorXAccumulation;
  double accVectorYAccumulation;
  struct accdata wiimote_acc;

  QList<irpoint> lastPoints;

  //settings
  int accelerationTimeout;
  int deadzoneXRange;
  int deadzoneYRange;
  int aimHelperXRange;
  int aimHelperYRange;
  int outzoneXRange;
  int outzoneYRange;

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
  EIOInfraredMouse(EIOEventDevice *device, uint id);
  ~EIOInfraredMouse();

  enum AxisMode {
    AbsoluteDevice = 0,
    RelativeDevice = 1
  };

  void setDeviceId(int);
  void setInterfaceEnabled(bool);
  void setAccelerationTimeoutValue(int);
  void setAimHelperXRange(int);
  void setAimHelperYRange(int);
  void setDeadzoneXRange(int);
  void setDeadzoneYRange(int);
  void setOutzoneXRange(int);
  void setOutzoneYRange(int);
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
  void dbusVirtualCursorPosition(uint, double, double, double, double);
  void dbusVirtualCursorLost(uint);
  void dbusWiimoteAcc(uint, const accdata &);

private:
  void axisAccelerationX();
  void axisAccelerationY();

private Q_SLOTS:
  void axisAccelerationTimeout();


};
#endif // INFRARED_VIRTUAL_MOUSE_H
