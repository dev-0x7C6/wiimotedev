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

#ifndef WIIMOTECONNECTION_H
#define WIIMOTECONNECTION_H

#include <QThread>
#include <QObject>

#include "dbus/deviceevents.h"
#include "wiimotedev/wiiremote.h"

class DBusDeviceEventsAdaptorWrapper;
class WiimoteDevice;
class WiimoteConnection;

class QThread;

class WiimoteConnection : public QThread
{
    Q_OBJECT
private:
  DBusDeviceEventsAdaptorWrapper *adaptor;

  bool quitRequest;

  QList < double> wfXmotion; QList < double> nfXmotion;
  QList < double> wfYmotion; QList < double> nfYmotion;
  QList < double> wfZmotion; QList < double> nfZmotion;

  quint32 sequence;

/*  Latency  *************************************************************/
  quint32 currentLatency;
  quint32 averageLatency;

/*  Cleanup  *************************************************************/
  void classicDeviceCleanup(struct stickdata &lstick, struct stickdata &rstick);
  void nunchukDeviceCleanup(struct stickdata &stick, struct accdata &acc);
  void wiimoteDeviceCleanup(QList< struct irpoint> &points, struct accdata &acc);

  quint8 life;
  quint8 status;

public:
  WiimoteConnection(DBusDeviceEventsAdaptorWrapper *adaptor, QObject *parent = 0);
 ~WiimoteConnection();

  void quitThread();

  WiimoteDevice *wiimote;

  void setWiimoteSequence(quint32 seq) { sequence = seq; }
  quint32 getWiimoteSequence(){ return sequence; }

/* Exported methods ******************************************************/   

  inline bool dbusIsClassicConnected(){ return (status & STATUS_WIIMOTE_CLASSIC_CONNECTED); }
  inline bool dbusIsNunchukConnected(){ return (status & STATUS_WIIMOTE_NUNCHUK_CONNECTED); }
  inline bool dbusIsWiimoteConnected(){ return (status & STATUS_WIIMOTE_CONNECTED); }

  inline quint32 dbusWiimoteGetAverageLatency(){ return averageLatency; }
  inline quint32 dbusWiimoteGetBatteryLife(){ return life; }
  inline quint32 dbusWiimoteGetCurrentLatency(){ return currentLatency; }
  inline quint8 getWiiremoteStatus() { return status; }

protected:
 void run();

private:
 void cwiidClassicMesg(cwiid_classic_mesg&);

private Q_SLOTS:
 void batteryStatusRequest();

Q_SIGNALS:
 void registerConnection(WiimoteConnection*);
 void unregisterConnection(WiimoteConnection*);

Q_SIGNALS:
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
