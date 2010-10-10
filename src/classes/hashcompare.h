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

#ifndef HASHCOMPARE_H
#define HASHCOMPARE_H

#include <QHash>
#include <QHashIterator>

template <class typeKey, class typeValue>
class HashCompare : public QObject
{
public:
  HashCompare(QObject *parent = 0);

 enum CompareStyle {
   BitCompare = 0,
   EqualCompare,
   NotEqualCompare
 };

 bool compare(QHash<typeKey, typeValue>*, QHash<typeKey, typeValue>*, quint8);
};


template <class typeKey, class typeValue>
HashCompare<typeKey, typeValue>::HashCompare (QObject *parent) :
  QObject(parent)
{
}


template <class typeKey, class typeValue>
bool HashCompare<typeKey, typeValue>::compare (QHash<typeKey, typeValue> *first, QHash<typeKey, typeValue> *second, quint8 style) {
  if (first->isEmpty() || second->isEmpty())
    return false;

  bool matched = true;
  QHashIterator<typeKey, typeValue> map(*first);

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


#endif // HASHCOMPARE_H
