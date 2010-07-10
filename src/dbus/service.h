/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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

#ifndef SERVICE_H
#define SERVICE_H

#include "support.h"

class DBusServiceAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.service")
    Q_CLASSINFO("D-Bus Introspection", ""
"<interface name=\"org.wiimotedev.service\">\n"
"    <method name=\"dbusDisconnectWiimote\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\"/>\n"
"    </method>\n"
"    <method name=\"dbusDefaultLedStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\"/>\n"
"    </method>\n"
"    <method name=\"dbusReloadSequenceList\">\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\"/>\n"
"    </method>\n"
"  </interface>");

public:
    DBusServiceAdaptor(QObject *parent);

public slots:
    bool dbusReloadSequenceList();
};

class DBusServiceAdaptorWrapper : public QObject
{
    Q_OBJECT
private:
    bool registred;

public:
    DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection &connection);
    inline bool isRegistred() { return registred; }

public Q_SLOTS:
    bool dbusReloadSequenceList();
};

#endif //SERVICE_H
