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
  accelerationTimeout(2000),
  deadzoneXRange(65),
  deadzoneYRange(60),
  sensitivityXPower(1.5),
  sensitivityYPower(1.2),
  sensitivityXMultiplier(5),
  sensitivityYMultiplier(5),
  useAcceleration(true)
{
  connect(&clock, SIGNAL(timeout()), this, SLOT(accel()));
  clock.setInterval(5);
  if (useAcceleration)
    clock.start();
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

  if (points.count() < 2)
    return;

//  if (points.count() == 1) {
//    if (sqrt(pow(lastx1 - points.at(0).x, 2.0) + pow(lasty1 - points.at(0).y, 2.0)) <
//        sqrt(pow(lastx2 - points.at(0).x, 2.0) + pow(lasty2 - points.at(0).y, 2.0))) {
//      x1 = points.at(0).x;
//      y1 = points.at(0).y;
//      x2 = lastx2 + (x1 - lastx1);
//      y2 = lasty2;
//    } else {
//      x2 = points.at(0).x;
//      y2 = points.at(0).y;
//      x1 = lastx1 + (x2 - lastx2);
//      y1 = lasty1;
//    }
//  }

  if (points.count() == 2) {
    x1 = points.at(0).x;
    x2 = points.at(1).x;
    y1 = points.at(0).y;
    y2 = points.at(1).y;
  } else
  if (points.count() > 2)
  {
    double lenght = 1000;
    for (int i = 0; i < points.count(); ++i)
      for (int j = 0; j < points.count(); ++j) {
        double nlenght = sqrt(pow(points.at(j).x - points.at(i).x, 2.0) + pow(points.at(j).y - points.at(i).y, 2.0));

        if (nlenght < 0)
          nlenght = nlenght * (-1);

        if (lenght > nlenght) {
          x1 = points.at(i).x;
          x2 = points.at(j).x;
          y1 = points.at(i).y;
          y2 = points.at(j).y;
          lenght = nlenght;
        }
      }
  }

  lastx1 = x1;
  lastx2 = x2;
  lasty1 = y1;
  lasty2 = y2;

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

  device->moveMousePointerRel(x - lastX, y - lastY);

  lastX = x;
  lastY = y;

  if (ax > deadzoneXRange || ax < -deadzoneXRange ||
      ay > deadzoneYRange || ay < -deadzoneYRange) {
    accVectorX = pow(abs((ax / (512.0 -  deadzoneXRange)) * -sensitivityXMultiplier), sensitivityXPower);
    accVectorY = pow(abs((ay / (384.0 -  deadzoneYRange)) * -sensitivityYMultiplier), sensitivityYPower);
    if (accVectorX >= 0.0 && ax > 0.0) accVectorX = accVectorX * -1;
    if (accVectorY >= 0.0 && ay > 0.0) accVectorY = accVectorY * -1;
  }

  qDebug() << x << "x" << y;

}

void InfraredVirtualMouse::accel()
{
  device->moveMousePointerRel(accVectorX, accVectorY);
}//


