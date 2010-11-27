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

#ifndef VIRTUAL_MOUSE_H
#define VIRTUAL_MOUSE_H

#include "devices/mouse.h"

struct MouseConfiguration {
  quint32 device;
  quint8 mode;
  double deadzoneXRange;
  double deadzoneYRange;
  double sensitivityXPower;
  double sensitivityYPower;
  quint16 latency;
};

class VirtualMouse: public QObject
{
  Q_OBJECT
private:
  UInputMouse *mouse;
  quint32 currentMode;

  MouseConfiguration config;

public:
  VirtualMouse();
 ~VirtualMouse();

  enum MouseMode {
    ModeNotSet,
    ModeDesktop,
    ModeFPS
  };

private:
  void setMouseConfiguration(struct MouseConfiguration &cfg);
  void switchToDesktopMode();
  void switchToFPSMode();

public slots:
  void dbusClassicControllerLStick(quint32, stickdata);
  void dbusClassicControllerRStick(quint32, stickdata);
  void dbusNunchukStick(quint32, stickdata);
  void dbusWiimoteInfrared(quint32, QList< irpoint>);

};

#endif // UINPUT_MOUSE_H
