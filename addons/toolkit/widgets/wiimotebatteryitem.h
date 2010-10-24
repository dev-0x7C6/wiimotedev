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


#ifndef WIIMOTEBATTERYITEM_H
#define WIIMOTEBATTERYITEM_H

#include <QGraphicsItem>

class WiimoteBatteryItem : public QObject, public QGraphicsItem
{
  Q_OBJECT
private:
  quint32 level;

public:
  WiimoteBatteryItem(QObject *parent = 0);
  QRectF boundingRect() const;

  static const quint8 barHeight = 2;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

public Q_SLOTS:
  void setBatteryLevel(quint32 param) { level = param; }

};

#endif // WIIMOTEBATTERYITEM_H
