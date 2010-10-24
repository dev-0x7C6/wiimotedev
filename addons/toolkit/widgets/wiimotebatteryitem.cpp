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

#include "wiimotebatteryitem.h"


#include <QPainter>

WiimoteBatteryItem::WiimoteBatteryItem(QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  level(50)
{
}

QRectF WiimoteBatteryItem::boundingRect() const
{
  return QRectF(0, 0, 80, barHeight);
}

void WiimoteBatteryItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  register double x = level*0.8;

  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::white);
  painter->drawRect(0, 0, x, barHeight);
  painter->setBrush(Qt::darkGray);
  painter->drawRect(x-1, 0, 80-x, barHeight);
}
