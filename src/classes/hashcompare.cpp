/**********************************************************************************
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

#include <QObject>
#include <QHashIterator>

#include "hashcompare.h"

HashCompare::HashCompare(QObject *parent) :
  QObject(parent)
{
}

bool HashCompare::isCompare(QHash < quint32, quint64> *first, QHash < quint32, quint64> *second, quint8 style) {
  if (first->isEmpty() || second->isEmpty())
    return false;

  bool matched = true;
  QHashIterator < quint32, quint64> map(*first);

  while (map.hasNext()) {
    map.next();

    switch (style) {
    case HashCompare::BitCompare:
      matched &= ((map.value() & second->value(map.key(), 0)) == map.value());
      break;
    case HashCompare::EqualCompare:
      matched &= (map.value() == second->value(map.key(), 0));
      break;
    case HashCompare::NotEqualCompare:
      matched &= (map.value() != second->value(map.key(), 0));
      break;
    }

    if (!matched)
      break;
  }

  return matched;
}
