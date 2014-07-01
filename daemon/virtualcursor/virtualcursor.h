/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2014  Bartłomiej Burdukiewicz                               *
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

#ifndef VIRTUALCURSOR_H
#define VIRTUALCURSOR_H

#include <QList>
#include <QPointF>
#include "dbus/deviceevents.h"

#include <qmath.h>

class VirtualCursor
{
private:
  QPointF m_cursor;
  bool m_visible;
  int m_index;
  double m_distance;
  double m_angle[2];
  int16 m_ctable[2][2];
  int16 m_ltable[2][2];
  int m_calibrationPoint[2];

private:
  double angleDiff(double a, double b);
  double _distance(int16 p[2], int16 q[2]);

public:
  VirtualCursor();

  bool calculate(QList < struct irpoint > &points, double roll);
  bool visible();
  QPointF& cursor();
  double distance();
  double angle();
};

#endif // VIRTUALCURSOR_H
