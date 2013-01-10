/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#ifndef INFRAREDCAMERAVIEW_H
#define INFRAREDCAMERAVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QMenu>

#include "linux/usr/include/wiimotedev/deviceevents.h"

#include "linux/usr/include/wiimotedev/consts.h"

const double defaultPointMultiplier = 2.5;


class InfraredCameraView : public QGraphicsView {
  Q_OBJECT
private:
  QString macAddress;

public:
  InfraredCameraView(WiimotedevDeviceEvents *iface, uint id = 1, QWidget *parent = 0);
  ~InfraredCameraView();

  enum Points {
    ir0source = 0,
    ir1source,
    ir2source,
    ir3source,
    ir4source
  };

  enum IROrder {
    LeftToRight,
    RightToLeft
  };

  enum AnalogMode {
    modeNone,
    modeClassic,
    modeNunchuk
  };

  void setWiimoteId(uint id);

protected:
  virtual void resizeEvent(QResizeEvent*);

private:
  QGraphicsLineItem *line;
  QGraphicsPixmapItem *cursor;

  QGraphicsEllipseItem *infraredPoints[4];
  QGraphicsLineItem *infraredLine[4];
  QTime timer;

  WiimotedevDeviceEvents *iface;
  uint wiimoteId;

  QGraphicsScene scene;
  QTimer infraredTimeout;

  double widthMultiplier;
  double heightMultiplier;
  double dotSizeMultiplier;

  QString getReadableWiiremoteSequence(uint64);

private slots:
  void infraredCleanup();

  void dbusVirtualCursorPosition(uint id, double x, double y, double size, double angle);
  void dbusVirtualCursorLost(uint id);
  void dbusVirtualCursorFound(uint id);

  void dbusWiimoteAcc(uint id, const accdata &acc);
  void dbusWiimoteConnected(uint id);
  void dbusWiimoteDisconnected(uint id);
  void dbusWiimoteInfrared(uint id, const QList< struct irpoint> &points);
};

#endif // INFRAREDCAMERAVIEW_H
