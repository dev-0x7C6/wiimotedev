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

#ifndef DBUSSUPPORT_H
#define DBUSSUPPORT_H

// Defaults
#include <QDBusAbstractAdaptor>
#include <QDBusArgument>
#include <QDBusConnection>

#include <QList>
#include "wiimotedev.h"

// Meta-types
#include <QDBusMetaType>
#include <QMetaType>

#define QWIIMOTEDEV_REGISTER_META_TYPES                                      \
    qRegisterMetaType< QList< struct irpoint> >("QList< irpoint>");          \
    qRegisterMetaType< QList< struct accdata> >("QList< accdata>");          \
    qRegisterMetaType< QList< struct stickdata> >("QList< stickdata>");      \
    qRegisterMetaType< QList< struct deviceinfo> >("QList< deviceinfo>");    \
    qRegisterMetaType< struct irpoint>("irpoint");                           \
    qRegisterMetaType< struct accdata>("accdata");                           \
    qRegisterMetaType< struct stickdata>("stickdata");                       \
    qRegisterMetaType< struct deviceinfo>("deviceinfo");                     \
    qDBusRegisterMetaType< QList < struct irpoint> >();                      \
    qDBusRegisterMetaType< QList < struct accdata> >();                      \
    qDBusRegisterMetaType< QList < struct stickdata> >();                    \
    qDBusRegisterMetaType< QList < struct deviceinfo> >();                   \
    qDBusRegisterMetaType< struct irpoint>();                                \
    qDBusRegisterMetaType< struct accdata>();                                \
    qDBusRegisterMetaType< struct stickdata>();                              \
    qDBusRegisterMetaType< struct deviceinfo>();

#ifndef QWIIMOTEDEV_META_TYPES
#define QWIIMOTEDEV_META_TYPES

    Q_DECLARE_METATYPE(QList < deviceinfo>);
    Q_DECLARE_METATYPE(QList < irpoint>);
    Q_DECLARE_METATYPE(QList < accdata>);
    Q_DECLARE_METATYPE(QList < stickdata>);

    Q_DECLARE_METATYPE(deviceinfo);
    Q_DECLARE_METATYPE(irpoint);
    Q_DECLARE_METATYPE(accdata);
    Q_DECLARE_METATYPE(stickdata);

#endif

#ifndef QWIIMOTEDEV_MARSHALL
#define QWIIMOTEDEV_MARSHALL

    QDBusArgument& operator<<(QDBusArgument& argument, const irpoint& point);
    const QDBusArgument& operator>>(const QDBusArgument& argument, irpoint& point);
    QDBusArgument& operator<<(QDBusArgument& argument, const accdata& acc);
    const QDBusArgument& operator>>(const QDBusArgument& argument, accdata& acc);
    QDBusArgument& operator<<(QDBusArgument& argument, const stickdata& stick);
    const QDBusArgument& operator>>(const QDBusArgument& argument, stickdata& stick);
    QDBusArgument& operator<<(QDBusArgument& argument, const deviceinfo& info);
    const QDBusArgument& operator>>(const QDBusArgument& argument, deviceinfo& info);

#endif

class DBusServiceAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.service")
    Q_CLASSINFO("D-Bus Introspection", ""
    "<interface name=\"org.wiimotedev.service\">\n"
    "    <method name=\"dbusReloadSequenceList\">\n"
    "      <arg type=\"y\" direction=\"out\"/>\n"
    "    </method>\n"
    "  </interface>");

public:
    DBusServiceAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent) {
        QWIIMOTEDEV_REGISTER_META_TYPES;
        setAutoRelaySignals(true);
    }

public slots:
    bool dbusReloadSequenceList();
};

class DBusDeviceEventsAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.deviceEvents")
    Q_CLASSINFO("D-Bus Introspection", ""
    " <interface name=\"org.wiimotedev.deviceEvents\">\n"
    "    <signal name=\"dbusReportUnregistredWiiremote\">\n"
    "     <arg type=\"s\" direction=\"out\"/>\n"
    "    </signal>\n"    
    "    <signal name=\"dbusWiimoteGeneralButtons\">\n"
    "     <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"t\" direction=\"out\"/>\n"
    "    </signal>\n"
    "   <method name=\"dbusUnregistredWiiremoteList\">\n"
    "      <arg type=\"as\" direction=\"out\"/>\n"
    "    </method>\n"
    "   <method name=\"dbusWiimoteGetCurrentLatency\">\n"
    "      <arg name=\"id\" type=\"u\" direction=\"in\"/>\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </method>\n"
    "   <method name=\"dbusWiimoteGetAverageLatency\">\n"
    "      <arg name=\"id\" type=\"u\" direction=\"in\"/>\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </method>\n"
    "   <method name=\"dbusWiimoteGetLedStatus\">\n"
    "      <arg name=\"id\" type=\"u\" direction=\"in\"/>\n"
    "      <arg type=\"y\" direction=\"out\"/>\n"
    "    </method>\n"
    "    <method name=\"dbusWiimoteGetRumbleStatus\">\n"
    "      <arg type=\"b\" direction=\"out\"/>\n"
    "      <arg name=\"id\" type=\"u\" direction=\"in\"/>\n"
    "    </method>\n"
    "    <method name=\"dbusWiimoteSetLedStatus\">\n"
    "      <arg type=\"b\" direction=\"out\"/>\n"
    "      <arg name=\"id\" type=\"u\" direction=\"in\"/>\n"
    "      <arg name=\"status\" type=\"y\" direction=\"in\"/>\n"
    "    </method>\n"
    "    <method name=\"dbusWiimoteSetRumbleStatus\">\n"
    "      <arg type=\"b\" direction=\"out\"/>\n"
    "      <arg name=\"id\" type=\"u\" direction=\"in\"/>\n"
    "      <arg name=\"status\" type=\"b\" direction=\"in\"/>\n"
    "    </method>\n"    
    "    <method name=\"dbusGetDeviceList\">\n"
    "      <arg type=\"ai\" direction=\"out\"/>\n"
    "    </method>\n"
    "    <signal name=\"dbusWiimoteConnected\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusWiimoteDisconnected\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusWiimoteBatteryLife\">\n"
    "     <arg type=\"u\" direction=\"out\"/>\n"
    "     <arg type=\"y\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusWiimoteButtons\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"t\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusWiimoteStatus\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"y\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusWiimoteAcc\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"(yyydd)\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusWiimoteInfrared\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"a(nqq)\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusNunchukPlugged\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusNunchukUnplugged\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusNunchukButtons\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"t\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusNunchukStick\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"(yy)\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusNunchukAcc\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"(yyydd)\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusClassicControllerPlugged\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusClassicControllerUnplugged\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusClassicControllerButtons\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"t\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusClassicControllerLStick\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"(yy)\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <signal name=\"dbusClassicControllerRStick\">\n"
    "      <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"(yy)\" direction=\"out\"/>\n"
    "    </signal>\n"
    "  </interface>");

public:
    DBusDeviceEventsAdaptor (QObject *parent) : QDBusAbstractAdaptor(parent) {
        QWIIMOTEDEV_REGISTER_META_TYPES;
        setAutoRelaySignals(true);
    }

public slots:
    quint32 dbusWiimoteGetCurrentLatency(quint32 id);
    quint32 dbusWiimoteGetAverageLatency(quint32 id);

    QList < int> dbusGetDeviceList();
    QStringList dbusUnregistredWiiremoteList();

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint8 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
    quint8 dbusWiimoteGetLedStatus(quint32 id);

signals:
    void dbusReportUnregistredWiiremote(QString);
    void dbusWiimoteGeneralButtons(quint32, quint64);

    void dbusWiimoteConnected(quint32);
    void dbusWiimoteDisconnected(quint32);
    void dbusWiimoteBatteryLife(quint32, quint8);
    void dbusWiimoteButtons(quint32, quint64);
    void dbusWiimoteStatus(quint32, quint8);
    void dbusWiimoteInfrared(quint32, QList< struct irpoint>);
    void dbusWiimoteAcc(quint32, struct accdata);

    void dbusNunchukPlugged(quint32);
    void dbusNunchukUnplugged(quint32);
    void dbusNunchukButtons(quint32, quint64);
    void dbusNunchukStick(quint32, struct stickdata);
    void dbusNunchukAcc(quint32, struct accdata);

    void dbusClassicControllerPlugged(quint32);
    void dbusClassicControllerUnplugged(quint32);
    void dbusClassicControllerButtons(quint32, quint64);
    void dbusClassicControllerLStick(quint32, struct stickdata);
    void dbusClassicControllerRStick(quint32, struct stickdata);
};

class DBusServiceAdaptorWrapper : public QObject
{
    Q_OBJECT
private:
    bool registred;

public:
    DBusServiceAdaptorWrapper(QObject *parent, QDBusConnection &connection) : QObject(parent) {
        new DBusServiceAdaptor(this);
        registred = connection.registerObject(WIIMOTEDEV_DBUS_SERVICE_OBJECT, this);
    }

    inline bool isRegistred() { return registred; }

public Q_SLOTS:
    inline bool dbusReloadSequenceList() {
        bool value;
        QMetaObject::invokeMethod(parent(), "dbusReloadSequenceList", Qt::DirectConnection, Q_RETURN_ARG(bool, value));
        return value;
    }
};

class DBusDeviceEventsAdaptorWrapper : public QObject
{
    Q_OBJECT
private:
    bool registred;

public:
    DBusDeviceEventsAdaptorWrapper(QObject *parent, QDBusConnection &connection) : QObject(parent) {
        new DBusDeviceEventsAdaptor(this);
        registred = connection.registerObject(WIIMOTEDEV_DBUS_EVENTS_OBJECT, this);
    }

    inline bool isRegistred() { return registred; }

public Q_SLOTS:
    inline quint32 dbusWiimoteGetCurrentLatency(quint32 id){
        quint32 latency;
        QMetaObject::invokeMethod(parent(), "dbusWiimoteGetCurrentLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
        return latency;
    }

    inline quint32 dbusWiimoteGetAverageLatency(quint32 id) {
        quint32 latency;
        QMetaObject::invokeMethod(parent(), "dbusWiimoteGetAverageLatency", Qt::DirectConnection, Q_RETURN_ARG(quint32, latency), Q_ARG(quint32, id));
        return latency;
    }

    inline bool dbusWiimoteGetRumbleStatus(quint32 id){
        bool value;
        QMetaObject::invokeMethod(parent(), "dbusWiimoteGetRumbleStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id));
        return value;
    }

    inline bool dbusWiimoteSetRumbleStatus(quint32 id, bool status)
    {
        bool value;
        QMetaObject::invokeMethod(parent(), "dbusWiimoteSetRumbleStatus", Qt::DirectConnection,  Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(bool, status));
        return value;
    }

    inline quint8 dbusWiimoteGetLedStatus(quint32 id){
        quint8 value;
        QMetaObject::invokeMethod(parent(), "dbusWiimoteGetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(quint8, value), Q_ARG(quint32, id));
        return value;
    }

    inline bool dbusWiimoteSetLedStatus(quint32 id, quint8 status){
        bool value;
        QMetaObject::invokeMethod(parent(), "dbusWiimoteSetLedStatus", Qt::DirectConnection, Q_RETURN_ARG(bool, value), Q_ARG(quint32, id), Q_ARG(quint8, status));
        return value;
    }

    inline QList < int> dbusGetDeviceList()
    {
        QList < int> list;
        QMetaObject::invokeMethod(parent(), "dbusGetDeviceList", Qt::DirectConnection, Q_RETURN_ARG(QList < int>, list));
        return list;
    }

    inline QStringList dbusUnregistredWiiremoteList() {
        QStringList list;
        QMetaObject::invokeMethod(parent(), "dbusUnregistredWiiremoteList", Qt::DirectConnection, Q_RETURN_ARG(QStringList, list));
        return list;
    }

Q_SIGNALS:
    void dbusReportUnregistredWiiremote(QString);
    void dbusWiimoteGeneralButtons(quint32, quint64);

    void dbusWiimoteConnected(quint32);
    void dbusWiimoteDisconnected(quint32);
    void dbusWiimoteBatteryLife(quint32, quint8);
    void dbusWiimoteButtons(quint32, quint64);
    void dbusWiimoteStatus(quint32, quint8);
    void dbusWiimoteInfrared(quint32, QList< struct irpoint>);
    void dbusWiimoteAcc(quint32, struct accdata);

    void dbusNunchukPlugged(quint32);
    void dbusNunchukUnplugged(quint32);
    void dbusNunchukButtons(quint32, quint64);
    void dbusNunchukStick(quint32, struct stickdata);
    void dbusNunchukAcc(quint32, struct accdata);

    void dbusClassicControllerPlugged(quint32);
    void dbusClassicControllerUnplugged(quint32);
    void dbusClassicControllerButtons(quint32, quint64);
    void dbusClassicControllerLStick(quint32, struct stickdata);
    void dbusClassicControllerRStick(quint32, struct stickdata);
};

#endif // DBUSSUPPORT_H
