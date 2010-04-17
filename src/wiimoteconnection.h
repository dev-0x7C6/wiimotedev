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

#ifndef WIIMOTECONNECTION_H
#define WIIMOTECONNECTION_H

#include <QThread>
#include <cwiid.h>
#include <math.h>

#include "wiimotedev.h"

class WiimoteConnection : public QThread
{
    Q_OBJECT

private:
    QList < double> wfXmotion;
    QList < double> wfYmotion;
    QList < double> wfZmotion;
    QList < double> nfXmotion;
    QList < double> nfYmotion;
    QList < double> nfZmotion;

    quint8 ledStatus;
    bool rumbleStatus;

    cwiid_wiimote_t *device;
    bool connected;
    bdaddr_t wiimotebdaddr;
    int id;
    quint32 sequence;

/*  Latency  *************************************************************/
    quint32 currentLatency;
    quint32 averageLatency;

/*  Cleanup  *************************************************************/
    void classicDeviceCleanup(struct stickdata &lstick, struct stickdata &rstick);
    void nunchukDeviceCleanup(struct stickdata &stick, struct accdata &acc);
    void wiimoteDeviceCleanup(QList< struct irpoint> &points, struct accdata &acc);

public:
    WiimoteConnection(QObject *parent = 0);
   ~WiimoteConnection();

    bool connectAny();
    void _disconnect();

    bdaddr_t getWiimoteAddr(){ return wiimotebdaddr; }
    QString getWiimoteSAddr();
    int getWiimoteId(){ return id; }
    void setWiimoteSequence(quint32 seq) { sequence = seq; }
    quint32 getWiimoteSequence(){ return sequence; }
    bool isConnected() { return connected; }

/* Exported methods ******************************************************/
    quint32 dbusWiimoteGetCurrentLatency(){ return currentLatency; };
    quint32 dbusWiimoteGetAverageLatency(){ return averageLatency; };

    quint8 getLedStatus();
    bool getRumbleStatus();
    void setLedStatus(quint8 status);
    void setRumbleStatus(bool status);

protected:
   void run();

signals:
   void registerConnection(void*);
   void unregisterConnection(void*);

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

#endif // WIIMOTECONNECTION_H
