/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QThread>
#include <QMap>
#include <QStringList>
#include <cwiid.h>

#include "dbus/deviceevents.h"
#include "dbus/service.h"
#include "include/wiimotedev/consts.h"
#include "network/support.h"
#include "syslog/support.h"
#include "wiimotedev/connection.h"
#include "wiimotedev/settings.h"

template class QList< void*>;

class ConnectionManager : public QThread
{
    Q_OBJECT
private:
// Adaptor section ------------------------------------------ /
    DBusDeviceEventsAdaptorWrapper *dbusDeviceEventsAdaptor;
    DBusServiceAdaptorWrapper *dbusServiceAdaptor;

// Settings ------------------------------------------------- /
    WiimotedevSettings *wiimotedevSettings;

    QStringList unregisterWiiremoteList;

    MessageServerThread *tcpServerThread;

    QMap< QString, quint32> wiiremoteSequence;
    QList< void*> objectList;
    bdaddr_t bdaddr_any;
    bool terminateReq;

    WiimoteConnection *active_connection;

public:
    ConnectionManager();
   ~ConnectionManager();
    void terminateRequest();

protected:
    void run();  

private:
    WiimoteConnection* findWiiremoteObject(quint32 id);

private slots:
    void registerConnection(void *object);
    void unregisterConnection(void *object);

public Q_SLOTS:
    bool dbusIsClassicConnected(quint32 id);
    bool dbusIsNunchukConnected(quint32 id);
    bool dbusIsWiimoteConnected(quint32 id);

    quint32 dbusWiimoteGetAverageLatency(quint32 id);
    quint32 dbusWiimoteGetBatteryLife(quint32 id);
    quint32 dbusWiimoteGetCurrentLatency(quint32 id);

    bool dbusWiimoteGetRumbleStatus(quint32 id);
    bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
    quint8 dbusWiimoteGetLedStatus(quint32 id);

    quint8 dbusWiimoteGetStatus(quint32 id);

    QList < uint> dbusGetWiimoteList();
    QStringList dbusGetUnregistredWiimoteList();

    bool dbusReloadSequenceList();

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

#endif // CONNECTIONMANAGER_H
