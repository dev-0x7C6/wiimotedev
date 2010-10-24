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

#include "wiimoteleditem.h"
#include <QGraphicsSceneMouseEvent>

WiimoteLedItem::WiimoteLedItem(QObject *parent) :
  QObject(parent),
  QGraphicsPixmapItem()
{
  switchOff();
}

void WiimoteLedItem::switchOn() {
  if (status)
    return;

  setPixmap(QPixmap(":/led_on.png"));
  status = true;
}

void WiimoteLedItem::switchOff() {
  if (!status)
    return;

  setPixmap(QPixmap(":/led_off.png"));
  status = false;
}

void WiimoteLedItem::mousePressEvent (QGraphicsSceneMouseEvent *event) {
  if (status)
    switchOff(); else
    switchOn();

  emit ledSwitched(status);
}
