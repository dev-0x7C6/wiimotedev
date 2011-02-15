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
\
#include <QTimer>
#include <QTime>
#include "devices/eventdevice.h"
#include "devices/mouse.h"
#include "interfaces/deviceevents.h"

enum MouseMode {
  ModeNotSet,
  ModeDesktop,
  ModeFPS
};

enum SourceDevice {
  SourceInfrared,
  SourceNunchukStick,
  SourceClassicLStick,
  SourceClassicRStick
};

struct MouseConfiguration {
  quint32 device;
  quint8 mode;
  quint8 source;
  double deadzoneXRange;
  double deadzoneYRange;
  double sensitivityXMultiplier;
  double sensitivityYMultiplier;
  quint16 latency;
};


class VirtualMouse: public QObject
{
  Q_OBJECT
private:
  UInputMouse *mouse;
  struct accdata wiimote_acc;

  MouseConfiguration config;
  quint8 currentMode;

  QTimer interruptClock;
  QTimer accelerationClock;
  QTime accelerationTimeout;

  qint16 lastx1;
  qint16 lastx2;
  qint16 lasty1;
  qint16 lasty2;


  QTime timer;
  quint8 order;

  DBusDeviceEventsInterface *events;
  double lastX;
  double lastY;

  QPoint accVector;

  QList < QPoint> data;

public:
  VirtualMouse(DBusDeviceEventsInterface *events);
 ~VirtualMouse();

  void setMouseConfiguration(struct MouseConfiguration &cfg);

private:
  void switchToDesktopMode();
  void switchToFPSMode();

private Q_SLOTS:
  void interrupt();
  void acceleration();

  void dbusClassicControllerLStick(quint32, stickdata);
  void dbusClassicControllerRStick(quint32, stickdata);
  void dbusNunchukStick(quint32, stickdata);
  void dbusWiimoteAcc(quint32, const accdata &table);
  void dbusWiimoteInfrared(quint32, QList< irpoint>);

};

#endif // UINPUT_MOUSE_H
