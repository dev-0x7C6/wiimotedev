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

#ifndef HASHCOMPARE_H
#define HASHCOMPARE_H

#include <QHash>

class QObject;

class HashCompare :public QObject
{
public:
  HashCompare(QObject *parent = 0);

 enum CompareStyle {
   BitCompare = 0,
   EqualCompare,
   NotEqualCompare
 };

  bool isCompare(QHash < quint32, quint64>*, QHash < quint32, quint64>*, quint8);
};



#endif // HASHCOMPARE_H
