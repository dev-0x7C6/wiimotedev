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

#include "wiimoterumbleitem.h"
#include <QGraphicsSceneMouseEvent>

WiimoteRumbleItem::WiimoteRumbleItem(QObject *parent) :
  QObject(parent),
  QGraphicsPixmapItem(),
  status(false)
{
  setPixmap(QPixmap(":/rumble_off.png"));
  switchOff();
}

void WiimoteRumbleItem::switchOn() {
  if (status)
    return;

  setPixmap(QPixmap(":/rumble_on.png"));
  status = true;
}

void WiimoteRumbleItem::switchOff() {
  if (!status)
    return;

  setPixmap(QPixmap(":/rumble_off.png"));
  status = false;
}

void WiimoteRumbleItem::mousePressEvent (QGraphicsSceneMouseEvent *event) {
  if (status)
    switchOff(); else
    switchOn();

  emit rumbleSwitched(status);
}
