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

#ifndef WIIREMOTE_H
#define WIIREMOTE_H

// This is wrapper class, for changes in cwiid API or when wiimotedev-daemon will use
// other library like *libwiimotedev (when is done)

#include <QObject>
#include <QTime>

#include <cwiid.h>
#include <math.h>

#include "../include/wiimotedev.h"

#define defaultReportFlags CWIID_RPT_STATUS | \
                           CWIID_RPT_BTN | \
                           CWIID_RPT_IR | \
                           CWIID_RPT_NUNCHUK | \
                           CWIID_RPT_CLASSIC | \
                           CWIID_RPT_ACC

class WiiremoteDevice : public QObject
{
private:
    bdaddr_t bdaddr;
    cwiid_wiimote_t *device;
    qint32 id;

private:
    bool isRumble;
    quint8 switchOnLeds;
    quint8 reportMode;

public:
    explicit WiiremoteDevice(QObject *parent = 0);
    virtual ~WiiremoteDevice();

    bool connectToDevice(const quint32 timeout = 3);
    bool disconnectFromDevice(const bool switchOfReport = true);

    bool getMesgStruct(int *count, union cwiid_mesg *mesg[], struct timespec *time);

    inline bool isConnected() { return (device != 0); }
    inline bool isDisconnected() { return (device == 0); }

    bool setLedStatus(quint8 led);
    bool setRumbleStatus(bool rumble);
    bool setReportMode(quint8 mode);

    quint8 getLedStatus();
    bool getRumbleStatus();
    quint8 getReportMode();
    bool getWiimoteState(struct cwiid_state &state);

    bool getDeviceCallibration(enum cwiid_ext_type ext_type, struct acc_cal *acc_cal);

    QString getWiimoteSAddr();
    bdaddr_t getWiimoteAddr();

};

#endif // WIIREMOTE_H