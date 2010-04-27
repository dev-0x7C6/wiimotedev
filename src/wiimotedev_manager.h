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

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#ifdef __syslog
    #include <syslog.h>
#endif

#include "wiimotedev_connection.h"
#include "wiimotedev_settings.h"

#include "syslog_support.h"
#include "tcp_support.h"
#include "dbus_support.h"
#include "wiimotedev.h"

const QString sequenceSection("sequence");
const QString tcpSection("tcp/");
const QString wiimotedevSection("wiimotedev/");

const QString wiimotedevDBusIface("DBusInterface");
const QString wiimotedevTCPIface("TCPInterface");

const QString tcpPort("port");

const bool defDBusInterfaceEnabled = true;
const bool defTCPInterfaceEnabled = false;
const quint16 defTCPPort = 50091;

extern QString filePathWiimotedev;

class ConnectionManager : public QThread
{
    Q_OBJECT
private:
    DBusDeviceEventsAdaptorWrapper *dbusDeviceEventsAdaptor;
    DBusServiceAdaptorWrapper *dbusServiceAdaptor;

    bool DBusInterface;
    bool TCPInterface;

    quint16 tcpPort;

    MessageServerThread *tcpServerThread;

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

private:
    WiimoteConnection* findWiiremoteObject(quint32 id);

private slots:
    void registerConnection(void *object);
    void unregisterConnection(void *object);

    void slotDBusNunchukPlugged(quint32);
    void slotDBusNunchukUnplugged(quint32);
    void slotDBusClassicControllerPlugged(quint32);
    void slotDBusClassicControllerUnplugged(quint32);

public slots:
    QList < struct deviceinfo> dbusGetDeviceList();

    quint32 dbusWiimoteGetCurrentLatency(quint32 id);
    quint32 dbusWiimoteGetAverageLatency(quint32 id);

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint8 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
    quint8 dbusWiimoteGetLedStatus(quint32 id);

    bool dbusReloadSequenceList();

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

#endif // CONNECTIONMANAGER_H
