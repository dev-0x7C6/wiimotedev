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

  UInputEvent *device;
  quint32 id;

  QTimer clock;
  QTime timeout;
  int accVectorX;
  int accVectorY;
  struct accdata wiimote_acc;

  quint8 firstPoint;
  quint8 secondPoint;

  QList< irpoint> lastPoints;

  qint16 lastx1;
  qint16 lastx2;
  qint16 lasty1;
  qint16 lasty2;
  qint16 lastsx1;
  qint16 order;
  double lastX;
  double lastY;

  int lastPointCount;

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

  bool useAcceleration;
  bool useAimHelper;

public:
  InfraredVirtualMouse(UInputEvent *device, quint32 id);
 ~InfraredVirtualMouse();


public Q_SLOTS:
  void dbusWiimoteInfrared(quint32, QList< irpoint>);
  void dbusWiimoteAcc(quint32, const accdata&);

private Q_SLOTS:
  void accel();


};
#endif // INFRARED_VIRTUAL_MOUSE_H
