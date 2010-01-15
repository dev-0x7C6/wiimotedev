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

#ifndef WIIMOTECONNECTION_H
#define WIIMOTECONNECTION_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include "cwiid.h"

const quint8 device_disconnected = 0x00;
const quint8 device_wiimote_connected = 0x01;
const quint8 device_nunchuk_connected = 0x02;
const quint8 device_classic_connected = 0x04;

extern int verboseLevel;

class WiimoteConnection : public QThread
{
    Q_OBJECT

private:
    QTimer *timer;
    cwiid_wiimote_t *device;
    quint8 batteryLife;
    quint8 newBatteryLife;

    quint8 deviceStatus;

    bool connected;
    bdaddr_t wiimotebdaddr;
    int id;
    int sequence;

    struct acc_cal wiimote_calibration;
    struct acc_cal nunchuk_calibration;

private slots:
    void getBatteryStatus();

public:
    WiimoteConnection(QObject *parent = 0);
   ~WiimoteConnection();

    bool connectAny();
    void _disconnect();

    bdaddr_t getWiimoteAddr(){ return wiimotebdaddr; }
    QString getWiimoteSAddr();
    int getWiimoteId(){ return id; }
    void setWiimoteSequence(int seq) { sequence = seq; }
    int getWiimoteSequence(){ return sequence; }
    bool isConnected() { return connected; }

    void setLedStatus(quint8 led){ cwiid_command(device, CWIID_CMD_LED, led); }

protected:
   void run();

signals:
   void registerConnection(void*);
   void unregisterConnection(void*);

   void wiimoteStatusChanged(void*, quint8);
   void batteryLifeChanged(void*, quint8);
   void buttonStatusChanged(void*, quint64);
   void infraredTableChanged(void*, cwiid_ir_mesg);

signals:
   void dbusBatteryLifeChanged(quint32, quint8);
   void dbusButtonStatusChanged(quint32, quint64);
   void dbusInfraredTableChanged(quint32, QStringList);
   void dbusWiimoteStatusChanged(quint32, quint8);
   void dbusNunchukAccTableChanged(quint32, quint8, quint8, quint8, qreal, qreal);
   void dbusWiimoteAccTableChanged(quint32, quint8, quint8, quint8, qreal, qreal);

};

#endif // WIIMOTECONNECTION_H
