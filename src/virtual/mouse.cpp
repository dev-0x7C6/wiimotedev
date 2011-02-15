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

#include "virtual/mouse.h"
#include <math.h>
#include <QDebug>

const double RAD = M_PI/180;

VirtualMouse::VirtualMouse(DBusDeviceEventsInterface *events):
  mouse(new UInputMouse()),
  currentMode(ModeNotSet),
  events(events),
  lastX(-1.0),
  lastY(-1.0),
  order(0)
{
  memset(&config, 0, sizeof(struct MouseConfiguration));
  connect(&accelerationClock, SIGNAL(timeout()), this, SLOT(acceleration()));
  connect(&interruptClock, SIGNAL(timeout()), this, SLOT(interrupt()));
  accelerationClock.setInterval(10);
  accelerationTimeout.start();
}

VirtualMouse::~VirtualMouse() {
  if (mouse->alreadyOpened)
    mouse->uinput_close();
  delete mouse;
}

void VirtualMouse::setMouseConfiguration(MouseConfiguration &cfg) {
  config = cfg;

  if (config.mode != currentMode || !mouse->alreadyOpened)
  switch(config.mode) {
  case ModeDesktop:
    switchToDesktopMode();
    break;
  case ModeFPS:
    switchToFPSMode();
    break;
  }

  disconnect(events, 0, this, 0);
  switch (config.source) { // this reduce unused slot interrupts
  case SourceInfrared:
    connect(events, SIGNAL(dbusWiimoteInfrared(quint32,QList<irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList<irpoint>)));
    connect(events, SIGNAL(dbusWiimoteAcc(quint32,accdata)), this, SLOT(dbusWiimoteAcc(quint32,accdata)));
    break;
  case SourceNunchukStick:
    connect(events, SIGNAL(dbusNunchukStick(quint32,stickdata)), this, SLOT(dbusNunchukStick(quint32,stickdata)));
    break;
  case SourceClassicLStick:
    connect(events, SIGNAL(dbusClassicControllerLStick(quint32,stickdata)), this, SLOT(dbusClassicControllerLStick(quint32,stickdata)));
    break;
  case SourceClassicRStick:
    connect(events, SIGNAL(dbusClassicControllerRStick(quint32,stickdata)), this, SLOT(dbusClassicControllerRStick(quint32,stickdata)));
    break;
  }

  interruptClock.stop();
  interruptClock.setInterval(cfg.latency);
  if (cfg.latency)
    interruptClock.start(); else
    interruptClock.stop();
}

void VirtualMouse::switchToDesktopMode() {
  if (mouse->alreadyOpened) {
    mouse->uinput_close();
    currentMode = ModeNotSet;
  }

  qDebug() << "desktop";
  if (mouse->uinput_open(QRect(-512, -384, 1024, 768), false))
    currentMode = ModeDesktop;
}

void VirtualMouse::switchToFPSMode() {
  if (mouse->alreadyOpened) {
    mouse->uinput_close();
    currentMode = ModeNotSet;
  }

  if (mouse->uinput_open(QRect(-512, -384, 1024, 768), true)) {
    currentMode = ModeFPS;
  }
}

void VirtualMouse::dbusClassicControllerLStick(quint32 id, stickdata stick) {
  if (config.device != id)
    return;

  accVector.setX((0x1F - stick.x) * -1);
  accVector.setY((0x1F - stick.y));

  if ((accVector.x() > -0x04) && (accVector.x() < 0x04))
    accVector.setX(0);
  if ((accVector.y() > -0x04) && (accVector.y() < 0x04))
    accVector.setY(0);


  if (accVector.x() < 0)
    accVector.setX(pow(abs(accVector.x()), 1.2) * -0.3); else
    accVector.setX(pow(abs(accVector.x()), 1.2) * 0.3);

  if (accVector.y() < 0)
    accVector.setY(pow(abs(accVector.y()), 1.2) * -0.3); else
    accVector.setY(pow(abs(accVector.y()), 1.2) * 0.3);

  accelerationClock.setInterval(10);


  qDebug() << accVector;
  accelerationClock.start();

  if (!config.latency)
    interrupt();
}

void VirtualMouse::dbusClassicControllerRStick(quint32 id, stickdata stick) {
  if (config.device != id)
    return;

  accVector.setX(0x0F - stick.x);
  accVector.setY(0x0F - stick.y);

  accelerationClock.start();

  if (!config.latency)
    interrupt();
}

void VirtualMouse::dbusNunchukStick(quint32 id, stickdata) {
  if (config.device != id)
    return;

  if (!config.latency)
    interrupt();
}

void VirtualMouse::dbusWiimoteAcc(quint32 id, const accdata &table)
{
  if (config.device != id)
    return;

  memcpy(&wiimote_acc, &table, sizeof(table));
}

void VirtualMouse::dbusWiimoteInfrared(quint32 id, QList< irpoint> points) {
  if (config.device != id)
    return;

  qint16 x1;
  qint16 x2;
  qint16 y1;
  qint16 y2;

  if (points.count())

  if (points.count() == 1) {
    if (sqrt(pow(lastx1 - points.at(0).x, 2.0) + pow(lasty1 - points.at(0).y, 2.0)) <
        sqrt(pow(lastx2 - points.at(0).x, 2.0) + pow(lasty2 - points.at(0).y, 2.0))) {
      x1 = points.at(0).x;
      y1 = points.at(0).y;
      x2 = lastx2 + (x1 - lastx1);
      y2 = lasty2;
    } else {
      x2 = points.at(0).x;
      y2 = points.at(0).y;
      x1 = lastx1 + (x2 - lastx2);
      y1 = lasty1;
    }

    qDebug() << x1 << "x" << y1 << " - " << x2 << "x" << y2;
  }

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

  if (timer.elapsed() > 40) {
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
  timer.start();

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

  double ax = (x1 + x2) / 2.0;
  double ay = (y1 + y2) / 2.0;

  double x = ((1024 - (ax*cosp - ay*sinp + 512*(1-cosp) + 384*sinp)));
  double y = (((ax*sinp + ay*cosp - 512*sinp + 384*(1-cosp))));

  //qDebug() << (512-x) << "x" << (384-y);

  double aax = 512.0 - x;
  double aay = 384.0 - y;

  if (lastX == -1.0) lastX = x;
  if (lastY == -1.0) lastY = y;

  QPoint point;
  point.setX(static_cast< int>(x) - static_cast< int>(lastX));
  point.setY(static_cast< int>(y) - static_cast< int>(lastY));

  if (aax > config.deadzoneXRange || aax < -config.deadzoneXRange ||
      aay > config.deadzoneYRange || aay < -config.deadzoneYRange)
  {
    accVector.setX((aax / (512.0 -  config.deadzoneXRange)) * -config.sensitivityXMultiplier);
    accVector.setY((aay / (384.0 -  config.deadzoneYRange)) * -config.sensitivityYMultiplier);
  }

  data << point;

  lastX = x;
  lastY = y;

  accelerationTimeout.start();
  accelerationClock.start();

  if (!config.latency)
    interrupt();
}

void VirtualMouse::acceleration() {
  mouse->moveMousePointerRel(accVector.x(), accVector.y());
  //if (accelerationTimeout.elapsed() > 3000)
  //  accelerationClock.stop();
}

void VirtualMouse::interrupt() {
  foreach (const QPoint &point, data)
    mouse->moveMousePointerRel(point.x(), point.y());
  data.clear();
}
