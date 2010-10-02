/**********************************************************************************
 * QWiimotedev, wiimotedev-uinput qt4 front-end                                   *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
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

signals:
  void executeRequest(QStringList);

};


#endif // CUSTOMJOBS_INTERFACE_H
