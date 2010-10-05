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

#ifndef CAR_H
#define CAR_H

#include <QGraphicsItem>
#include <QObject>
#include <QBrush>

#include "src/interfaces/deviceevents.h"

class Car : public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
public:
  Car(quint32 id = 0);
 ~Car();
  QRectF boundingRect() const;
  QBrush color;

public slots:
// slots for signals from DeviceEventsClass
  void dbusWiimoteButtons(quint32 id, quint64 value);
  void dbusWiimoteAcc(quint32 id, struct accdata acc);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  void timerEvent(QTimerEvent *event);

private:
  DBusDeviceEventsInterface *iface; // DBus interface
  quint32 wiimoteId; // store wiimote id

  bool useRoll, carBreak;
  qreal wheelsAngle;
  qreal speed;
};

#endif // CAR_H
