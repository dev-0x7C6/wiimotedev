/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#ifdef __syslog
    #include <syslog.h>
#endif

#include <QDBusAbstractAdaptor>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusMetaType>

#include <QTime>

#include "wiimoteconnection.h"
#include "wiimotedev.h"

Q_DECLARE_METATYPE(irpoint)
Q_DECLARE_METATYPE(accdata)
Q_DECLARE_METATYPE(stickdata)
Q_DECLARE_METATYPE(deviceinfo)

Q_DECLARE_METATYPE(QList < irpoint>)
Q_DECLARE_METATYPE(QList < accdata>)
Q_DECLARE_METATYPE(QList < stickdata>)
Q_DECLARE_METATYPE(QList < deviceinfo>)

const QString sequenceGroup("sequence");
extern QString filePathWiimotedev;

class DeviceEventsClass : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.deviceEvents")
    Q_CLASSINFO("D-Bus Introspection", ""
    " <interface name=\"org.wiimotedev.deviceEvents\">\n"
    "    <signal name=\"dbusWiimoteGeneralButtons\">\n"
    "     <arg type=\"u\" direction=\"out\"/>\n"
    "      <arg type=\"t\" direction=\"out\"/>\n"
    "    </signal>\n"
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
    DeviceEventsClass(QObject *parent);
    virtual ~DeviceEventsClass(){};

public slots:
    QList < struct deviceinfo> dbusGetDeviceList();

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint8 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
    quint8 dbusWiimoteGetLedStatus(quint32 id);

signals:
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


class ConnectionManager : public QThread
{
    Q_OBJECT

private:   
    QMap< void*, struct deviceinfo> deviceList;
    QMap< QString, quint16> wiiremoteSequence;
    QList< void*> objectList;
    bdaddr_t bdaddr_any;
    bool terminateReq;
    WiimoteConnection *connectionObject;

public:
    ConnectionManager();
   ~ConnectionManager();

protected:
    void run();  

private slots:
    void registerConnection(void *object);
    void unregisterConnection(void *object);

    void slotDBusNunchukPlugged(quint32);
    void slotDBusNunchukUnplugged(quint32);
    void slotDBusClassicControllerPlugged(quint32);
    void slotDBusClassicControllerUnplugged(quint32);

public slots:
    QList < struct deviceinfo> dbusGetDeviceList();

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint8 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
    quint8 dbusWiimoteGetLedStatus(quint32 id);

signals:
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

#endif // CONNECTIONMANAGER_H
