/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
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

#ifndef UINPUTSERVICE_H
#define UINPUTSERVICE_H

// Defaults
#include <QDBusAbstractAdaptor>
#include <QDBusAbstractInterface>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>

#include <QList>
#include <wiimotedev.h>

// Meta-types
#include <QDBusMetaType>
#include <QMetaType>

class DBusServiceAdaptor :public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.service")
    Q_CLASSINFO("D-Bus Introspection", ""
     "  <interface name=\"org.wiimotedev.service\">\n"
     "    <method name=\"isWiimotedevServiceAvailable\">\n"
     "      <arg type=\"y\" direction=\"out\"/>\n"
     "    </method>\n"
     "  </interface>\n"
     "")
public:
    DBusServiceAdaptor (QObject *parent): QDBusAbstractAdaptor(parent) {
        setAutoRelaySignals(true);
    }

public slots:
    inline bool isWiimotedevServiceAvailable() {
        bool value;
        QMetaObject::invokeMethod(parent(), "isWiimotedevServiceAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
        return value;
    }

};

class DBusServiceAdaptorWrapper :public QObject
{
    Q_OBJECT
private:
    bool registred;

public:
    DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection &connection) : QObject(parent) {
        new DBusServiceAdaptor(this);
        registred = connection.registerObject("/service", this);
    }

    inline bool isRegistred() { return registred; }

public slots:
    inline bool isWiimotedevServiceAvailable() {
        bool value;
        QMetaObject::invokeMethod(parent(), "isWiimotedevServiceAvailable", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
        return value;
    }
};

#endif // UINPUTSERVICE_H
