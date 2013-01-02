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

#ifndef ADAPTORS_CUSTOMJOBS_H
#define ADAPTORS_CUSTOMJOBS_H

#include "adaptors.h"

class DBusCustomJobsAdaptor :public QDBusAbstractAdaptor
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.customJobs")
  Q_CLASSINFO("D-Bus Introspection", ""
    "  <interface name=\"org.wiimotedev.customJobs\">\n"
    "    <signal name=\"executeRequest\">\n"
    "      <arg type=\"as\" direction=\"out\"/>\n"
    "    </signal>"
    "  </interface>\n"
    "")
public:
  DBusCustomJobsAdaptor (QObject *parent);

Q_SIGNALS:
  void executeRequest(QStringList);

};


class DBusCustomJobsAdaptorWrapper :public QObject
{
  Q_OBJECT
private:
  bool registred;

public:
  DBusCustomJobsAdaptorWrapper(QObject *parent, QDBusConnection connection);

  inline bool isRegistred() { return registred; }

  inline void slotExecuteRequest(QStringList params) { emit executeRequest(params); }

Q_SIGNALS:
  void executeRequest(QStringList);
};


inline DBusCustomJobsAdaptorWrapper::DBusCustomJobsAdaptorWrapper(QObject *parent, QDBusConnection connection):
    QObject(parent)
{
  new DBusCustomJobsAdaptor(this);
  registred = connection.registerObject("/customJobs", this);
}

inline DBusCustomJobsAdaptor::DBusCustomJobsAdaptor(QObject *parent): QDBusAbstractAdaptor(parent)
{
  setAutoRelaySignals(true);
}

#endif // ADAPTORS_CUSTOMJOBS_H
