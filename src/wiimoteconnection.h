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
#include <QMetaType>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <cwiid.h>
#include "wiimotedev.h"

class WiimoteConnection : public QThread
{
    Q_OBJECT

private:
    QTimer *timer;
    cwiid_wiimote_t *device;

    bool connected;
    bdaddr_t wiimotebdaddr;
    int id;
    int sequence;

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
