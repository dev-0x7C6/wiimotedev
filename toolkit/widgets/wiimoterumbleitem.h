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

#ifndef WIIMOTERUMBLEITEM_H
#define WIIMOTERUMBLEITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>

class WiimoteRumbleItem : public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
private:
  bool status;

public:
  WiimoteRumbleItem(QObject *parent = 0);

protected:
  virtual void mousePressEvent (QGraphicsSceneMouseEvent*);

public Q_SLOTS:
  void switchOn();
  void switchOff();

Q_SIGNALS:
  void rumbleSwitched(bool);

};

#endif // WIIMOTERUMBLEITEM_H
