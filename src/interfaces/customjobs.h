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

#ifndef CUSTOMJOBS_INTERFACE_H
#define CUSTOMJOBS_INTERFACE_H

#include "interfaces.h"

class DBusCustomJobsInterface : public QDBusAbstractInterface
{
  Q_OBJECT
public:
  static inline const char *staticInterfaceName() { return "org.wiimotedev.customJobs"; }

public:
  DBusCustomJobsInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0)
    :QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent){
	QWIIMOTEDEV_REGISTER_META_TYPES;
  }

Q_SIGNALS:
  void executeRequest(QStringList);

};


#endif // CUSTOMJOBS_INTERFACE_H
