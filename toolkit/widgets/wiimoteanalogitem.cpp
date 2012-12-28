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

#include "wiimoteanalogitem.h"

WiimoteAnalogItem::WiimoteAnalogItem(QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  size(1),
  x(15),
  y(15),
  width(30),
  height(30)
{
}

QRectF WiimoteAnalogItem::boundingRect() const {
  return QRectF(0, 0, width, height);
}

void  WiimoteAnalogItem::setPointSize(uint s) {
  size = s;
}

void WiimoteAnalogItem::setPointPos(uint ax, uint ay) {
  x = ax;
  y = ay;
}

void WiimoteAnalogItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::white);
  painter->setBrush(Qt::NoBrush);

  //painter->drawRect(0, 0, width, height);
  painter->drawEllipse(x - size, y - size, size << 1, size << 1);
}
