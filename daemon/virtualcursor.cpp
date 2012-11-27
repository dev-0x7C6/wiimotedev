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

#include "virtualcursor.h"

VirtualCursor::VirtualCursor() {
  for (register int i = 0; i < 3; ++i) {
    m_ctable[i][0] = 0x00;
    m_ctable[i][1] = 0x00;
    m_ltable[i][0] = 0x00;
    m_ltable[i][1] = 0x00;
  }

  m_index = 0x00;
  m_angle[0] = 0x00;
  m_angle[1] = 0x00;
}

double VirtualCursor::angleDiff(double a, double b) {
  double diff = b - a;
  while (diff > 180) { diff -= 360; }
  while (diff <= -180) { diff += 360; }
  return diff;
}

bool VirtualCursor::calculate(QList < struct irpoint > &points, double roll) {
  switch (points.count()) {
  case 4:
  case 3:
  case 2:
    m_ltable[0][0] = m_ctable[0][0] = points.at(0).x;
    m_ltable[1][0] = m_ctable[1][0] = points.at(1).x;
    m_ltable[0][1] = m_ctable[0][1] = points.at(0).y;
    m_ltable[1][1] = m_ctable[1][1] = points.at(1).y;
    m_ltable[2][0] = -1;
    m_ltable[1][1] = -1;
    m_visible = true;
    break;
  case 1:
    m_ctable[2][0] = points.at(0).x;
    m_ctable[2][1] = points.at(0).y;

    if (m_ctable[2][0] == -1)
      m_ltable[2][0] = m_ctable[2][0];
    if (m_ctable[2][1] == -1)
      m_ltable[2][1] = m_ctable[2][1];

    m_ctable[0][0] = m_ltable[0][0] + (m_ctable[2][0] - m_ltable[2][0]);
    m_ctable[1][0] = m_ltable[1][0] + (m_ctable[2][0] - m_ltable[2][0]);
    m_ctable[0][1] = m_ltable[0][1] + (m_ctable[2][1] - m_ltable[2][1]);
    m_ctable[1][1] = m_ltable[1][1] + (m_ctable[2][1] - m_ltable[2][1]);
    m_visible = true;
    break;
  case 0:
    return false;
  }


  m_angle[0] = atan2(m_ctable[1][1] - m_ctable[0][1], m_ctable[1][0] - m_ctable[0][0]) - M_PI;
  m_angle[1] = atan2(m_ctable[0][1] - m_ctable[1][1], m_ctable[0][0] - m_ctable[1][0]) - M_PI;
  m_index = (abs(angleDiff(m_angle[0]*180/M_PI, roll)) < 45) ? 0 : 1;

  register double cosp = cos(-m_angle[m_index]);
  register double sinp = sin(-m_angle[m_index]);
  register double sx = (m_ctable[0][0] + m_ctable[1][0]) / 2.0;
  register double sy = (m_ctable[0][1] + m_ctable[1][1]) / 2.0;

  m_cursor.setX(512.0 - ((1024.0 - (sx*cosp - sy*sinp + 512.0*(1-cosp) + 384.0*sinp))));
  m_cursor.setY(384.0 - ((sx*sinp + sy*cosp - 512.0*sinp + 384.0*(1-cosp))));

  return true;
}

bool VirtualCursor::visible() {
  return m_visible;
}

QPointF& VirtualCursor::cursor() {
  return m_cursor;
}

double VirtualCursor::angle() {
  return m_angle[m_index];
}
