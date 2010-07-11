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

#ifndef DEVICE_EVENTS_H
#define DEVICE_EVENTS_H

#include "support.h"

class DBusDeviceEventsAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.deviceEvents")
    Q_CLASSINFO("D-Bus Introspection", ""
" <interface name=\"org.wiimotedev.deviceEvents\">\n"
"    <method name=\"dbusGetWiimoteList\">\n"
"      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
"      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
"    </method>\n"
"    <method name=\"dbusGetUnregistredWiimoteList\">\n"
"      <arg direction=\"out\" type=\"as\" name=\"list\" />\n"
"    </method>\n"
"    <method name=\"dbusIsClassicConnected\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusIsNunchukConnected\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusIsWiimoteConnected\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetAverageLatency\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetBatteryLife\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetCurrentLatency\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetLedStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"led\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetRumbleStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"b\" name=\"rumble\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteSetLedStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"in\" type=\"u\" name=\"status\" />\n"
"      <arg direction=\"out\" type=\"b\" name=\"led\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteSetRumbleStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"in\" type=\"b\" name=\"status\" />\n"
"      <arg direction=\"out\" type=\"b\" name=\"rumble\" />\n"
"    </method>\n"
"    <signal name=\"dbusClassicControllerButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerLStick\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerPlugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerRStick\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerUnplugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukAcc\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yyydd)\" name=\"acc\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukPlugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukStick\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukUnplugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteAcc\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yyydd)\" name=\"acc\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteBatteryLife\">\n"
"     <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"     <arg direction=\"out\" type=\"y\" name=\"life\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteConnected\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteDisconnected\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteGeneralButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteInfrared\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"a(nqq)\" name=\"table\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteStatus\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </signal>\n"
"    <signal name=\"dbusReportUnregistredWiimote\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"mac\" />\n"
"    </signal>\n"
"  </interface>");

public:
    DBusDeviceEventsAdaptor (QObject *parent);

public Q_SLOTS:
    bool dbusIsClassicConnected(quint32 id);
    bool dbusIsNunchukConnected(quint32 id);
    bool dbusIsWiimoteConnected(quint32 id);

    quint32 dbusWiimoteGetAverageLatency(quint32 id);
    quint32 dbusWiimoteGetBatteryLife(quint32 id);
    quint32 dbusWiimoteGetCurrentLatency(quint32 id);

    quint8 dbusWiimoteGetLedStatus(quint32 id);
    quint8 dbusWiimoteGetStatus(quint32 id);

    QList < uint> dbusGetWiimoteList();
    QStringList dbusGetUnregistredWiimoteList();

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);

signals:
    void dbusReportUnregistredWiimote(QString);
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

class DBusDeviceEventsAdaptorWrapper : public QObject
{
    Q_OBJECT
private:
    bool registred;

public:
    DBusDeviceEventsAdaptorWrapper(QObject *parent, QDBusConnection &connection);

    inline bool isRegistred() { return registred; }

public Q_SLOTS:
    bool dbusIsClassicConnected(quint32 id);
    bool dbusIsNunchukConnected(quint32 id);
    bool dbusIsWiimoteConnected(quint32 id);

    quint32 dbusWiimoteGetAverageLatency(quint32 id);
    quint32 dbusWiimoteGetBatteryLife(quint32 id);
    quint32 dbusWiimoteGetCurrentLatency(quint32 id);

    quint8 dbusWiimoteGetLedStatus(quint32 id);
    quint8 dbusWiimoteGetStatus(quint32 id);

    QList < uint> dbusGetWiimoteList();
    QStringList dbusGetUnregistredWiimoteList();

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);

Q_SIGNALS:
    void dbusReportUnregistredWiimote(QString);
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

#endif //DEVICE_EVENTS_H
