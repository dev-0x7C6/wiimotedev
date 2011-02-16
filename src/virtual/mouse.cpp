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

}

void VirtualMouse::dbusWiimoteInfrared(quint32 id, QList< irpoint> points) {

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
