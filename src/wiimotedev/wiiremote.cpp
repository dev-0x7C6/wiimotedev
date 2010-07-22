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

#include "wiiremote.h"

WiiremoteDevice::WiiremoteDevice(QObject *parent) : QObject(parent)
{
    memset(&bdaddr, 0, sizeof(bdaddr_t));
    device = 0;
    isRumble = false;
    switchOnLeds = 0;
    reportMode = 0;
}

WiiremoteDevice::~WiiremoteDevice()
{
    if (isConnected())
        disconnectFromDevice(true);
}

bool WiiremoteDevice::connectToDevice(const quint32 timeout)
{
    memset(&bdaddr, 0, sizeof(bdaddr_t));

    if ((device = cwiid_open_timeout(&bdaddr, CWIID_FLAG_MESG_IFC, timeout)))
    {
       // memcpy(&wiimotebdaddr, &bdaddr, sizeof(bdaddr_t));
        id = cwiid_get_id(device);

        return setReportMode(reportMode = defaultReportFlags);
    }

    device = 0;
    return false;
}

bool WiiremoteDevice::disconnectFromDevice(const bool switchOfReport)
{
    if (isDisconnected())
        return false;

    if (switchOfReport)
        cwiid_set_rpt_mode(device, 0);

    cwiid_disconnect(device);

    device = 0;
    isRumble = false;
    switchOnLeds = 0;
    reportMode = 0;

    return true;
}

bool WiiremoteDevice::getMesgStruct(int *count, union cwiid_mesg *mesg[], struct timespec *time)
{
    if (isDisconnected())
        return false;

    if (cwiid_get_mesg(device, count, mesg, time)) {
        disconnectFromDevice(false);
        return false;
    }

    return true;
}

bool WiiremoteDevice::setLedStatus(quint8 led)
{
    if (isDisconnected())
        return false;

    if (cwiid_set_led(device, switchOnLeds = led)) {
        disconnectFromDevice(false);
        return false;
    }

    return true;
}

bool WiiremoteDevice::setRumbleStatus(bool rumble)
{
    if (isDisconnected())
        return false;

    if (cwiid_set_rumble(device, isRumble = rumble)) {
        disconnectFromDevice(false);
        return false;
    }

    return true;
}

bool WiiremoteDevice::setReportMode(quint8 mode)
{
    if (isDisconnected())
        return false;

    if (cwiid_set_rpt_mode(device, reportMode = mode)) {
        disconnectFromDevice(false);
        return false;
    }

    return true;
}

quint8 WiiremoteDevice::getLedStatus()
{
    if (isDisconnected())
        return 0;

    return switchOnLeds;
}

bool WiiremoteDevice::getRumbleStatus()
{
    if (isDisconnected())
        return false;

    return isRumble;
}

quint8 WiiremoteDevice::getReportMode()
{
    if (isDisconnected())
        return 0;

    return reportMode;
}

QString WiiremoteDevice::getWiimoteSAddr()
{
    char addr[17];
    ba2str(&bdaddr, addr);
    return QString::fromAscii(addr, 17);
}

bdaddr_t WiiremoteDevice::getWiimoteAddr()
{
    return bdaddr;
}

bool WiiremoteDevice::getDeviceCallibration(enum cwiid_ext_type ext_type, struct acc_cal *acc_cal)
{
    if (isDisconnected())
        return false;

    if (cwiid_get_acc_cal(device, ext_type, acc_cal)) {
        disconnectFromDevice(false);
        return false;
    }

    return true;
}

bool WiiremoteDevice::getWiimoteState(struct cwiid_state &state)
{
    if (isDisconnected())
        return false;

    if (cwiid_get_state(device, &state)){
        disconnectFromDevice(false);
        return false;
    }

    return true;
}
