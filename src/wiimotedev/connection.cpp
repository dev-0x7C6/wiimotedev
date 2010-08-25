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
#include <QThread>
#include <QTimer>
#include <QTime>

#include "wiimotedev/connection.h"

extern bool additional_debug;

WiimoteConnection::WiimoteConnection(DBusDeviceEventsAdaptorWrapper *adaptor, QObject *parent)
    :QThread(parent), adaptor(adaptor)
{
    if (additional_debug)
        syslog_message("> in WiimoteConnection");

    if (adaptor) {
        connect(this, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), adaptor, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteConnected(quint32)), adaptor, SIGNAL(dbusWiimoteConnected(quint32)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteDisconnected(quint32)), adaptor, SIGNAL(dbusWiimoteDisconnected(quint32)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), adaptor, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteButtons(quint32,quint64)), adaptor, SIGNAL(dbusWiimoteButtons(quint32,quint64)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteStatus(quint32,quint8)), adaptor, SIGNAL(dbusWiimoteStatus(quint32,quint8)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), adaptor, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), adaptor, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusNunchukPlugged(quint32)), adaptor, SIGNAL(dbusNunchukPlugged(quint32)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusNunchukUnplugged(quint32)), adaptor, SIGNAL(dbusNunchukUnplugged(quint32)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), adaptor, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusNunchukButtons(quint32,quint64)), adaptor, SIGNAL(dbusNunchukButtons(quint32,quint64)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), adaptor, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusClassicControllerPlugged(quint32)), adaptor, SIGNAL(dbusClassicControllerPlugged(quint32)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusClassicControllerUnplugged(quint32)), adaptor, SIGNAL(dbusClassicControllerUnplugged(quint32)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), adaptor, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), adaptor, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), Qt::DirectConnection);
        connect(this, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), adaptor, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), Qt::DirectConnection);
    }

    setTerminationEnabled(true);
    Device = new WiiremoteDevice;
}

WiimoteConnection::~WiimoteConnection()
{
    if (additional_debug)
        syslog_message("> in ~WiimoteConnection");

    if (adaptor) {
        disconnect(this, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), adaptor, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)));
        disconnect(this, SIGNAL(dbusWiimoteConnected(quint32)), adaptor, SIGNAL(dbusWiimoteConnected(quint32)));
        disconnect(this, SIGNAL(dbusWiimoteDisconnected(quint32)), adaptor, SIGNAL(dbusWiimoteDisconnected(quint32)));
        disconnect(this, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), adaptor, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)));
        disconnect(this, SIGNAL(dbusWiimoteButtons(quint32,quint64)), adaptor, SIGNAL(dbusWiimoteButtons(quint32,quint64)));
        disconnect(this, SIGNAL(dbusWiimoteStatus(quint32,quint8)), adaptor, SIGNAL(dbusWiimoteStatus(quint32,quint8)));
        disconnect(this, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), adaptor, SIGNAL(dbusWiimoteInfrared(quint32,QList< struct irpoint>)));
        disconnect(this, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), adaptor, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)));
        disconnect(this, SIGNAL(dbusNunchukPlugged(quint32)), adaptor, SIGNAL(dbusNunchukPlugged(quint32)));
        disconnect(this, SIGNAL(dbusNunchukUnplugged(quint32)), adaptor, SIGNAL(dbusNunchukUnplugged(quint32)));
        disconnect(this, SIGNAL(dbusNunchukStick(quint32,struct stickdata)), adaptor, SIGNAL(dbusNunchukStick(quint32,struct stickdata)));
        disconnect(this, SIGNAL(dbusNunchukButtons(quint32,quint64)), adaptor, SIGNAL(dbusNunchukButtons(quint32,quint64)));
        disconnect(this, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), adaptor, SIGNAL(dbusNunchukAcc(quint32,struct accdata)));
        disconnect(this, SIGNAL(dbusClassicControllerPlugged(quint32)), adaptor, SIGNAL(dbusClassicControllerPlugged(quint32)));
        disconnect(this, SIGNAL(dbusClassicControllerUnplugged(quint32)), adaptor, SIGNAL(dbusClassicControllerUnplugged(quint32)));
        disconnect(this, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), adaptor, SIGNAL(dbusClassicControllerButtons(quint32,quint64)));
        disconnect(this, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)), adaptor, SIGNAL(dbusClassicControllerLStick(quint32,struct stickdata)));
        disconnect(this, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)), adaptor, SIGNAL(dbusClassicControllerRStick(quint32,struct stickdata)));
    }

    delete Device;
}

void WiimoteConnection::run()
{
    if (Device->isDisconnected())
        return;

    emit dbusWiimoteConnected(sequence);

    int count;
    union cwiid_mesg *mesg;
    struct timespec time;

    double x, y, z, roll, pitch, vacc;

    struct cwiid_ir_mesg ir_mesg;
    memset(&ir_mesg, 0, sizeof(struct cwiid_ir_mesg));

    bool ButtonRequest = false;
    unsigned char BatteryLife, NewBatteryLife;

    unsigned long long WiimoteButtons = 0x00;
    unsigned long long WiimoteButtonsTmp = 0x00;

    struct acc_cal wiimote_calibration;

    memset(&wiimote_calibration, 0, sizeof(struct acc_cal));

    double LastWiimoteAccX = 0.0;
    double LastWiimoteAccY = 0.0;
    double LastWiimoteAccZ = 0.0;

    struct accdata acc;

//  Wiimote temporary variables **************************************************** /
    QList< irpoint> wiimoteIrTable;
    struct irpoint wiimotePoint;
    bool sendIrSignal = false;

    struct accdata wiimoteAccdata;

    wiimoteAccdata.x = 0xFF >> 1;
    wiimoteAccdata.y = 0xFF >> 1;
    wiimoteAccdata.z = 0xFF >> 1;
    wiimoteAccdata.pitch = 0.0;
    wiimoteAccdata.roll = 0.0;

//  Nunchuk temporary variables **************************************************** /
    struct stickdata nunchukStickdata;
    struct accdata nunchukAccdata;

    nunchukStickdata.x = 0;
    nunchukStickdata.y = 0;

    nunchukAccdata.x = 0xFF >> 1;
    nunchukAccdata.y = 0xFF >> 1;
    nunchukAccdata.z = 0xFF >> 1;
    nunchukAccdata.pitch = 0.0;
    nunchukAccdata.roll = 0.0;

    double nunchukAccX = 0.0;
    double nunchukAccY = 0.0;
    double nunchukAccZ = 0.0;

    const quint8 nunchukToleranceValue = (0xFF >> 2);

    const quint8 nunchukStickMaxX = (0xFF >> 1) + nunchukToleranceValue;
    const quint8 nunchukStickMinX = (0xFF >> 1) - nunchukToleranceValue;
    const quint8 nunchukStickMaxY = (0xFF >> 1) + nunchukToleranceValue;
    const quint8 nunchukStickMinY = (0xFF >> 1) - nunchukToleranceValue;

    struct acc_cal nunchuk_calibration;
    memset(&nunchuk_calibration, 0, sizeof(struct acc_cal));

//  Classic temporary variables **************************************************** /
    struct stickdata classicRStickdata, classicLStickdata;
    classicRStickdata.x = 0;
    classicRStickdata.y = 0;
    classicLStickdata.x = 0;
    classicLStickdata.y = 0;
    unsigned short int LastClassicButtons = 0x00;

    const quint8 classicLToleranceValue = (0x3F >> 2);
    const quint8 classicRToleranceValue = (0x1F >> 2);

    const quint8 classicLStickMaxX = (0x3F >> 1) + classicLToleranceValue;
    const quint8 classicLStickMinX = (0x3F >> 1) - classicLToleranceValue;
    const quint8 classicLStickMaxY = (0x3F >> 1) + classicLToleranceValue;
    const quint8 classicLStickMinY = (0x3F >> 1) - classicLToleranceValue;
    const quint8 classicRStickMaxX = (0x1F >> 1) + classicRToleranceValue;
    const quint8 classicRStickMinX = (0x1F >> 1) - classicRToleranceValue;
    const quint8 classicRStickMaxY = (0x1F >> 1) + classicRToleranceValue;
    const quint8 classicRStickMinY = (0x1F >> 1) - classicRToleranceValue;

    QTime latencyTimer;

    Device->getDeviceCallibration(CWIID_EXT_NONE, &wiimote_calibration);

    double wxvalueabs, wxvalue, nxvalueabs, nxvalue, wxpow, nxpow = 0.0;
    double wyvalueabs, wyvalue, nyvalueabs, nyvalue, wypow, nypow = 0.0;
    double wzvalueabs, wzvalue, nzvalueabs, nzvalue, wzpow, nzpow = 0.0;

    qint32 wcounter, ncounter = 0;

    currentLatency = 0;
    averageLatency = 0;

    quitRequest = false;
    bool nunchukPlugged = false;
    bool classicPlugged = false;

    status = STATUS_WIIMOTE_CONNECTED;

    int batteryRequest = 0;

    while (Device->getMesgStruct(&count, &mesg, &time) && !quitRequest) {
        currentLatency = latencyTimer.elapsed();
        latencyTimer.start();

        if (!averageLatency)
            averageLatency = currentLatency;

        averageLatency = (averageLatency + currentLatency) >> 1;

        if (batteryRequest > 10000) {
            batteryStatusRequest();
            batteryRequest = 0;
        }

        batteryRequest += 1;

        for (register int i = 0; i < count; ++i) switch (mesg[i].type) {

// Disconnect section *************************************************************************************************************** /
            case CWIID_MESG_ERROR:
            // Classic cleanup ****************************************************************************************************** /
                if (classicPlugged) classicDeviceCleanup(classicLStickdata, classicRStickdata);

            // Nunchuk cleanup ****************************************************************************************************** /
                if (nunchukPlugged) nunchukDeviceCleanup(nunchukStickdata, nunchukAccdata);

           // Wiimote cleanup ****************************************************************************************************** /
                Device->disconnectFromDevice(false);
                WiimoteButtons = 0;
                wiimoteDeviceCleanup(wiimoteIrTable, wiimoteAccdata);
                ButtonRequest = true;

                break;

// Status section ******************************************************************************************************************* /
            case CWIID_MESG_STATUS:
                NewBatteryLife = life = static_cast< unsigned char>(100.0 * (mesg[i].status_mesg.battery / static_cast< double>(CWIID_BATTERY_MAX)));
                if (BatteryLife != NewBatteryLife) {
                    BatteryLife = NewBatteryLife;
                    emit dbusWiimoteBatteryLife(sequence, BatteryLife);
                }

                switch (mesg[i].status_mesg.ext_type) {

                // Status section ************************************************************************************************** /
                    case CWIID_EXT_NONE:
                    // Classic unpluged ******************************************************************************************** /
                        if (classicPlugged) {
                            classicPlugged = false;
                            WiimoteButtons &= CLASSIC_BUTTON_NOTMASK;
                            WiimoteButtons &= CLASSIC_LSTICK_NOTMASK;
                            WiimoteButtons &= CLASSIC_RSTICK_NOTMASK;
                            classicDeviceCleanup(classicLStickdata, classicRStickdata);
                            ButtonRequest = true;
                        }

                    // Nunchuk unpluged ******************************************************************************************** /
                        if (nunchukPlugged) {
                            nunchukPlugged = false;
                            WiimoteButtons &= NUNCHUK_BUTTON_NOTMASK;
                            WiimoteButtons &= NUNCHUK_STICK_NOTMASK;
                            WiimoteButtons &= NUNCHUK_SHIFT_NOTMASK;
                            WiimoteButtons &= NUNCHUK_TILT_NOTMASK;
                            nunchukDeviceCleanup(nunchukStickdata, nunchukAccdata);
                            ButtonRequest = true;
                        }

                        status = STATUS_WIIMOTE_CONNECTED;
                        emit dbusWiimoteStatus(sequence, status);

                        break;

                // Nunchuk pluged ************************************************************************************************** /
                    case CWIID_EXT_NUNCHUK:
                        Device->getDeviceCallibration(CWIID_EXT_NUNCHUK, &nunchuk_calibration);
                        if (!nunchukPlugged)
                        {
                            nunchukPlugged = true;
                            status = STATUS_WIIMOTE_CONNECTED | STATUS_WIIMOTE_NUNCHUK_CONNECTED;
                            emit dbusNunchukPlugged(sequence);
                            emit dbusWiimoteStatus(sequence, status);
                        }
                        break;

                // Classic pluged ************************************************************************************************** /
                    case CWIID_EXT_CLASSIC:
                        if (!classicPlugged)
                        {
                            classicPlugged = true;
                            status = STATUS_WIIMOTE_CONNECTED | STATUS_WIIMOTE_CLASSIC_CONNECTED;
                            emit dbusClassicControllerPlugged(sequence);
                            emit dbusWiimoteStatus(sequence, status);
                        }
                        break;

                    default:
                        break;
                }
                break;

// Infrared section ***************************************************************************************************************** /
            case CWIID_MESG_IR:

                for (register int j = 0; j < 4; ++j) if (mesg[i].ir_mesg.src[j].valid) {
                    if (!sendIrSignal)
                        wiimoteIrTable.clear();

                    wiimotePoint.size =  (mesg[i].ir_mesg.src[j].size <= 0) ? 1 : mesg[i].ir_mesg.src[j].size; // when wiiremote extension present size of points are -1 ? libcwiid bug ?
                    wiimotePoint.x = mesg[i].ir_mesg.src[j].pos[0];
                    wiimotePoint.y = mesg[i].ir_mesg.src[j].pos[1];
                    sendIrSignal = true;
                    wiimoteIrTable << wiimotePoint;
                }

                if (sendIrSignal) {
                    emit dbusWiimoteInfrared(sequence, wiimoteIrTable);
                    sendIrSignal = false;
                }
                break;

            case CWIID_MESG_BTN:
                WiimoteButtons &= WIIMOTE_BUTTON_NOTMASK;
                ButtonRequest = true;

                if (mesg[i].btn_mesg.buttons & CWIID_BTN_1) WiimoteButtons |= WIIMOTE_BTN_1;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_2) WiimoteButtons |= WIIMOTE_BTN_2;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_A) WiimoteButtons |= WIIMOTE_BTN_A;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_B) WiimoteButtons |= WIIMOTE_BTN_B;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_MINUS) WiimoteButtons |= WIIMOTE_BTN_MINUS;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_PLUS)  WiimoteButtons |= WIIMOTE_BTN_PLUS;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_HOME)  WiimoteButtons |= WIIMOTE_BTN_HOME;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_RIGHT) WiimoteButtons |= WIIMOTE_BTN_RIGHT;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_LEFT)  WiimoteButtons |= WIIMOTE_BTN_LEFT;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_DOWN)  WiimoteButtons |= WIIMOTE_BTN_DOWN;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_UP)    WiimoteButtons |= WIIMOTE_BTN_UP;

                emit dbusWiimoteButtons(sequence, WiimoteButtons);
                break;

            case CWIID_MESG_ACC:
                x =  static_cast< double>((mesg[i].acc_mesg.acc[0] - wiimote_calibration.zero[0])) /
                     (wiimote_calibration.one[0] - wiimote_calibration.zero[0]);
                y =  static_cast< double>((mesg[i].acc_mesg.acc[1] - wiimote_calibration.zero[1])) /
                     (wiimote_calibration.one[1] - wiimote_calibration.zero[1]);
                z =  static_cast< double>((mesg[i].acc_mesg.acc[2] - wiimote_calibration.zero[2])) /
                     (wiimote_calibration.one[2] - wiimote_calibration.zero[2]);


                if ((LastWiimoteAccX != x) || (LastWiimoteAccY != y) || (LastWiimoteAccZ != z))
                {
                    LastWiimoteAccX = x;
                    LastWiimoteAccY = y;
                    LastWiimoteAccZ = z;

                    roll = atan(x / z);
                    if (z <= 0.0)
                       roll += 3.14159265358979323 * ((x > 0.0) ? 1 : -1);
                    roll *= -1;
                    if (z)
                        pitch = atan(y / z * cos(roll)); else pitch = 0.0;

                    acc.x = mesg[i].acc_mesg.acc[0];
                    acc.y = mesg[i].acc_mesg.acc[1];
                    acc.z = mesg[i].acc_mesg.acc[2];
                    acc.pitch = pitch;
                    acc.roll = roll;

                    emit dbusWiimoteAcc(sequence, acc);

                    WiimoteButtonsTmp = WiimoteButtons;
                    WiimoteButtons &= WIIMOTE_TILT_NOTMASK;
                    WiimoteButtons &= WIIMOTE_SHIFT_NOTMASK;

                    if (pitch > 0.30) WiimoteButtons |= WIIMOTE_BTN_TILT_FRONT; else
                    if (pitch < -0.30) WiimoteButtons |= WIIMOTE_BTN_TILT_BACK;
                    if (roll < -0.45) WiimoteButtons |= WIIMOTE_BTN_TILT_RIGHT; else
                    if (roll > 0.45) WiimoteButtons |= WIIMOTE_BTN_TILT_LEFT;

                    vacc = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

                    if (vacc > 2.2) WiimoteButtons |= WIIMOTE_BTN_SHIFT_SHAKE;

                    wxpow = sqrt(pow(x, 2));
                    wypow = sqrt(pow(y, 2));
                    wzpow = sqrt(pow(z, 2));

                    if ((wxpow > 2.0) || (wypow > 2.0) || (wzpow > 2.0))
                    {
                        wcounter = 10;
                        if (wxpow > 2.0) wfXmotion << x;
                        if (wypow > 2.0) wfYmotion << y;
                        if (wzpow > 2.0) wfZmotion << z;
                    }

                    if ((wcounter == 0) && ((wfXmotion.count() > 0) || (wfYmotion.count() > 0) || (wfZmotion.count() > 0)))
                    {
                        wxvalue = (wfXmotion.count() > 0) ? wfXmotion.last() : 0.0;
                        wyvalue = (wfYmotion.count() > 0) ? wfYmotion.last() : 0.0;
                        wzvalue = (wfZmotion.count() > 0) ? wfZmotion.last() : 0.0;
                        if (wfXmotion.count() > 1)
                            for (int i = wfXmotion.count() - 1; i > 0; --i)
                                if (wxvalue > 0) wxvalue += (wfXmotion.at(i - 1) > 0) ? wfXmotion.at(i - 1) : 0; else
                                                 wxvalue += (wfXmotion.at(i - 1) < 0) ? wfXmotion.at(i - 1) : 0;
                        if (wfYmotion.count() > 1)
                            for (int i = wfYmotion.count() - 1; i > 0; --i)
                                if (wyvalue > 0) wyvalue += (wfYmotion.at(i - 1) > 0) ? wfYmotion.at(i - 1) : 0; else
                                                 wyvalue += (wfYmotion.at(i - 1) < 0) ? wfYmotion.at(i - 1) : 0;
                        if (wfZmotion.count() > 1)
                            for (int i = wfZmotion.count() - 1; i > 0; --i)
                                if (wzvalue > 0) wzvalue += (wfZmotion.at(i - 1) > 0) ? wfZmotion.at(i - 1) : 0; else
                                                 wzvalue += (wfZmotion.at(i - 1) < 0) ? wfZmotion.at(i - 1) : 0;

                        wfXmotion.clear();
                        wfYmotion.clear();
                        wfZmotion.clear();

                        wxvalueabs = (wxvalue > 0) ? wxvalue : -wxvalue;
                        wyvalueabs = (wyvalue > 0) ? wyvalue : -wyvalue;
                        wzvalueabs = (wzvalue > 0) ? wzvalue : -wzvalue;


                        if (wxvalueabs > 20) wyvalueabs -= 80; else
                        if (wxvalueabs > 10) wyvalueabs -= 40; else
                        if (wxvalueabs > 5) wyvalueabs -= 20;

                        if (wyvalue < -4) wyvalueabs += 60; else
                        if (wyvalue < -3) wyvalueabs += 40; else
                        if (wyvalue < -2) wyvalueabs += 20; else
                        if (wyvalue < -1) wyvalueabs += 10;

                        if ((wxvalueabs > wyvalueabs) && (wxvalueabs > wzvalueabs))
                        {
                            WiimoteButtons |= ((wxvalue > 0) ? WIIMOTE_BTN_SHIFT_RIGHT : WIIMOTE_BTN_SHIFT_LEFT);
                        } else
                        if ((wyvalueabs > wxvalueabs) && (wyvalueabs > wzvalueabs))
                        {
                            WiimoteButtons |= ((wyvalue > 0) ? WIIMOTE_BTN_SHIFT_FORWARD : WIIMOTE_BTN_SHIFT_BACKWARD);
                        }

                    } else wcounter--;

                    if (WiimoteButtonsTmp != WiimoteButtons)
                    {
                        ButtonRequest = true;
                        emit dbusWiimoteButtons(sequence, WiimoteButtons);
                    }
                }
                break;

// Nunchuk messages ***************************************************************************************************************** /
            case CWIID_MESG_NUNCHUK:             
                WiimoteButtonsTmp = WiimoteButtons;

            // Nunchuk button section *********************************************************************************************** /
                WiimoteButtons &= NUNCHUK_BUTTON_NOTMASK;

                if (mesg[i].nunchuk_mesg.buttons & CWIID_NUNCHUK_BTN_C) WiimoteButtons |= NUNCHUK_BTN_C;
                if (mesg[i].nunchuk_mesg.buttons & CWIID_NUNCHUK_BTN_Z) WiimoteButtons |= NUNCHUK_BTN_Z;

            // Nunchuk stick section ************************************************************************************************ /
                WiimoteButtons &= NUNCHUK_STICK_NOTMASK;

                if (nunchukStickdata.x != mesg[i].nunchuk_mesg.stick[0] || nunchukStickdata.y != mesg[i].nunchuk_mesg.stick[1]) {
                    nunchukStickdata.x = mesg[i].nunchuk_mesg.stick[0];
                    nunchukStickdata.y = mesg[i].nunchuk_mesg.stick[1];
                    emit dbusNunchukStick(sequence, nunchukStickdata);
                }

                if (nunchukStickdata.x > nunchukStickMaxX) WiimoteButtons |= NUNCHUK_BTN_STICK_RIGHT; else
                if (nunchukStickdata.x < nunchukStickMinX) WiimoteButtons |= NUNCHUK_BTN_STICK_LEFT;
                if (nunchukStickdata.y > nunchukStickMaxY) WiimoteButtons |= NUNCHUK_BTN_STICK_UP; else
                if (nunchukStickdata.y < nunchukStickMinY) WiimoteButtons |= NUNCHUK_BTN_STICK_DOWN;

            // Nunchuk params section *********************************************************************************************** /
                x =  static_cast< double>((mesg[i].nunchuk_mesg.acc[0] - nunchuk_calibration.zero[0])) / (nunchuk_calibration.one[0] - nunchuk_calibration.zero[0]);
                y =  static_cast< double>((mesg[i].nunchuk_mesg.acc[1] - nunchuk_calibration.zero[1])) / (nunchuk_calibration.one[1] - nunchuk_calibration.zero[1]);
                z =  static_cast< double>((mesg[i].nunchuk_mesg.acc[2] - nunchuk_calibration.zero[2])) / (nunchuk_calibration.one[2] - nunchuk_calibration.zero[2]);

                if (nunchukAccX != x || nunchukAccY != y || nunchukAccZ != z)
                {
                    nunchukAccdata.x = mesg[i].nunchuk_mesg.acc[0];
                    nunchukAccdata.y = mesg[i].nunchuk_mesg.acc[1];
                    nunchukAccdata.z = mesg[i].nunchuk_mesg.acc[2];
                    nunchukAccX = x;
                    nunchukAccY = y;
                    nunchukAccZ = z;
                    nunchukAccdata.roll = atan(x / z);
                    if (z <= 0.0) nunchukAccdata.roll += 3.14159265358979323 * ((x > 0.0) ? 1 : -1);
                    nunchukAccdata.roll = -nunchukAccdata.roll;
                    if (z) nunchukAccdata.pitch = atan(y / z * cos(nunchukAccdata.roll)); else nunchukAccdata.pitch = 0.0;

                    emit dbusNunchukAcc(sequence, nunchukAccdata);
                }

            // Nunchuk tilt section ************************************************************************************************* /
                WiimoteButtons &= NUNCHUK_TILT_NOTMASK;

                if (nunchukAccdata.pitch > 0.30) WiimoteButtons  |= NUNCHUK_BTN_TILT_FRONT; else
                if (nunchukAccdata.pitch < -0.30) WiimoteButtons |= NUNCHUK_BTN_TILT_BACK;
                if (nunchukAccdata.roll < -0.45) WiimoteButtons  |= NUNCHUK_BTN_TILT_RIGHT; else
                if (nunchukAccdata.roll > 0.45) WiimoteButtons   |= NUNCHUK_BTN_TILT_LEFT;

            // Nunchuk shift section ************************************************************************************************ /
                WiimoteButtons &= NUNCHUK_SHIFT_NOTMASK;

                vacc = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

                if (vacc > 2.2) WiimoteButtons |= NUNCHUK_BTN_SHIFT_SHAKE;

                nxpow = sqrt(pow(x, 2));
                nypow = sqrt(pow(y, 2));
                nzpow = sqrt(pow(z, 2));

                if (nxpow > 2.0 || nypow > 2.0 || nzpow > 2.0) {
                    ncounter = 10;
                    if (nxpow > 2.0) nfXmotion << x;
                    if (nypow > 2.0) nfYmotion << y;
                    if (nzpow > 2.0) nfZmotion << z;
                }

                if (ncounter == 0 && (nfXmotion.count() > 0 || nfYmotion.count() > 0 || nfZmotion.count() > 0)) {
                    nxvalue = (nfXmotion.count() > 0) ? nfXmotion.last() : 0.0;
                    nyvalue = (nfYmotion.count() > 0) ? nfYmotion.last() : 0.0;
                    nzvalue = (nfZmotion.count() > 0) ? nfZmotion.last() : 0.0;
                    if (nfXmotion.count() > 1)
                        for (int i = nfXmotion.count() - 1; i > 0; --i)
                            if (nxvalue > 0) nxvalue += (nfXmotion.at(i - 1) > 0) ? nfXmotion.at(i - 1) : 0; else
                                             nxvalue += (nfXmotion.at(i - 1) < 0) ? nfXmotion.at(i - 1) : 0;
                    if (nfYmotion.count() > 1)
                        for (int i = nfYmotion.count() - 1; i > 0; --i)
                            if (nyvalue > 0) nyvalue += (nfYmotion.at(i - 1) > 0) ? nfYmotion.at(i - 1) : 0; else
                                             nyvalue += (nfYmotion.at(i - 1) < 0) ? nfYmotion.at(i - 1) : 0;
                    if (nfZmotion.count() > 1)
                        for (int i = nfZmotion.count() - 1; i > 0; --i)
                            if (nzvalue > 0) nzvalue += (nfZmotion.at(i - 1) > 0) ? nfZmotion.at(i - 1) : 0; else
                                             nzvalue += (nfZmotion.at(i - 1) < 0) ? nfZmotion.at(i - 1) : 0;

                    nfXmotion.clear();
                    nfYmotion.clear();
                    nfZmotion.clear();

                    nxvalueabs = (nxvalue > 0) ? nxvalue : -nxvalue;
                    nyvalueabs = (nyvalue > 0) ? nyvalue : -nyvalue;
                    nzvalueabs = (nzvalue > 0) ? nzvalue : -nzvalue;


                    if (nxvalueabs > 20) nyvalueabs -= 80; else
                    if (nxvalueabs > 10) nyvalueabs -= 40; else
                    if (nxvalueabs > 5) nyvalueabs -= 20;

                    if (nyvalue < -4) nyvalueabs += 60; else
                    if (nyvalue < -3) nyvalueabs += 40; else
                    if (nyvalue < -2) nyvalueabs += 20; else
                    if (nyvalue < -1) nyvalueabs += 10;

                    if ((nxvalueabs > nyvalueabs) && (nxvalueabs > nzvalueabs))
                        WiimoteButtons |= ((nxvalue > 0) ? NUNCHUK_BTN_SHIFT_RIGHT : NUNCHUK_BTN_SHIFT_LEFT);
                            else if ((nyvalueabs > nxvalueabs) && (nyvalueabs > nzvalueabs))
                                WiimoteButtons |= ((nyvalue > 0) ? NUNCHUK_BTN_SHIFT_FORWARD : NUNCHUK_BTN_SHIFT_BACKWARD);
                } else ncounter--;

                if (WiimoteButtonsTmp != WiimoteButtons) {
                    ButtonRequest = true;
                    emit dbusNunchukButtons(sequence, WiimoteButtons);
                }

                break;

// Classic messages ***************************************************************************************************************** /
            case CWIID_MESG_CLASSIC:
                WiimoteButtonsTmp = WiimoteButtons;
            // Classic right stick section ****************************************************************************************** /
                WiimoteButtons &= CLASSIC_RSTICK_NOTMASK;
                if ((classicRStickdata.x != mesg[i].classic_mesg.r_stick[0]) || (classicRStickdata.y != mesg[i].classic_mesg.r_stick[1])){
                    classicRStickdata.x = mesg[i].classic_mesg.r_stick[0];
                    classicRStickdata.y = mesg[i].classic_mesg.r_stick[1];
                    emit dbusClassicControllerRStick(sequence, classicRStickdata);
                }

                if (classicRStickdata.x > classicRStickMaxX) WiimoteButtons |= CLASSIC_BTN_RSTICK_RIGHT; else
                if (classicRStickdata.x < classicRStickMinX) WiimoteButtons |= CLASSIC_BTN_RSTICK_LEFT;
                if (classicRStickdata.y > classicRStickMaxY) WiimoteButtons |= CLASSIC_BTN_RSTICK_UP; else
                if (classicRStickdata.y < classicRStickMinY) WiimoteButtons |= CLASSIC_BTN_RSTICK_DOWN;

            // Classic left stick section ******************************************************************************************* /
                WiimoteButtons &= CLASSIC_LSTICK_NOTMASK;
                if ((classicLStickdata.x != mesg[i].classic_mesg.l_stick[0]) || (classicLStickdata.y != mesg[i].classic_mesg.l_stick[1])){
                    classicLStickdata.x = mesg[i].classic_mesg.l_stick[0];
                    classicLStickdata.y = mesg[i].classic_mesg.l_stick[1];
                    emit dbusClassicControllerLStick(sequence, classicLStickdata);
                }

                if (classicLStickdata.x > classicLStickMaxX) WiimoteButtons |= CLASSIC_BTN_LSTICK_RIGHT; else
                if (classicLStickdata.x < classicLStickMinX) WiimoteButtons |= CLASSIC_BTN_LSTICK_LEFT;
                if (classicLStickdata.y > classicLStickMaxY) WiimoteButtons |= CLASSIC_BTN_LSTICK_UP; else
                if (classicLStickdata.y < classicLStickMinY) WiimoteButtons |= CLASSIC_BTN_LSTICK_DOWN;

            // Classic button section *********************************************************************************************** /
                if (LastClassicButtons != mesg[i].classic_mesg.buttons)
                {               
                    LastClassicButtons = mesg[i].classic_mesg.buttons;
                    WiimoteButtons &= CLASSIC_BUTTON_NOTMASK;

                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_A) WiimoteButtons |= CLASSIC_BTN_A;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_B) WiimoteButtons |= CLASSIC_BTN_B;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_X) WiimoteButtons |= CLASSIC_BTN_X;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_Y) WiimoteButtons |= CLASSIC_BTN_Y;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_MINUS) WiimoteButtons |= CLASSIC_BTN_MINUS;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_PLUS) WiimoteButtons |= CLASSIC_BTN_PLUS;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_HOME) WiimoteButtons |= CLASSIC_BTN_HOME;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_L) WiimoteButtons |= CLASSIC_BTN_L;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_R) WiimoteButtons |= CLASSIC_BTN_R;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_ZL) WiimoteButtons |= CLASSIC_BTN_ZL;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_ZR) WiimoteButtons |= CLASSIC_BTN_ZR;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_UP) WiimoteButtons |= CLASSIC_BTN_UP;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_DOWN) WiimoteButtons |= CLASSIC_BTN_DOWN;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_LEFT) WiimoteButtons |= CLASSIC_BTN_LEFT;
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_RIGHT) WiimoteButtons |= CLASSIC_BTN_RIGHT;
                }

                if (WiimoteButtonsTmp != WiimoteButtons) {
                    ButtonRequest = true;
                    emit dbusClassicControllerButtons(sequence, WiimoteButtons);
                }

                break;

            default:
                break;
        }


        if (ButtonRequest)
        {
            ButtonRequest = false;
            emit dbusWiimoteGeneralButtons(sequence, WiimoteButtons);
        }

        delete mesg;
        if(Device->isDisconnected()) break;
    }


    Device->disconnectFromDevice(true);

    emit dbusWiimoteDisconnected(sequence);
    emit unregisterConnection(static_cast< void*>( this));   
}

void WiimoteConnection::batteryStatusRequest()
{
    struct cwiid_state state;
    Device->getWiimoteState(state);
    int newlife = static_cast< unsigned char>(100.0 * (state.battery / static_cast< double>(CWIID_BATTERY_MAX)));
    if (life != newlife)
        emit dbusWiimoteBatteryLife(sequence, life);
    life = newlife;
}


// Classic cleanup method ************************************************************************ /
void WiimoteConnection::classicDeviceCleanup(struct stickdata &lstick, struct stickdata &rstick) {
    lstick.x = lstick.y = 0x3F >> 1;
    rstick.x = rstick.y = 0x1F >> 1;
    emit dbusClassicControllerLStick(sequence, lstick);
    emit dbusClassicControllerRStick(sequence, rstick);
    emit dbusClassicControllerButtons(sequence, 0);
    emit dbusClassicControllerUnplugged(sequence);
}

// Nunchuk cleanup method *********************************************************************** /
void WiimoteConnection::nunchukDeviceCleanup(struct stickdata &stick, struct accdata &acc) {
    acc.x = acc.y = acc.z = 0xFF >> 1;
    acc.pitch = acc.roll = 0.0;
    stick.x = stick.y = 0xFF >> 1;
    emit dbusNunchukStick(sequence, stick);
    emit dbusNunchukAcc(sequence, acc);
    emit dbusNunchukButtons(sequence, 0);
    emit dbusNunchukUnplugged(sequence);
}


// Nunchuk cleanup method ************************************************************************/
void WiimoteConnection::wiimoteDeviceCleanup(QList< struct irpoint> &points, struct accdata &acc) {
    struct irpoint point;
    points.clear();
    point.x = point.y = 0;
    point.size = -1;
    points << point << point << point << point;

    acc.x = acc.y = acc.z = 0xFF >> 1;
    acc.pitch = acc.roll = 0.0;

    emit dbusWiimoteAcc(sequence, acc);
    emit dbusWiimoteInfrared(sequence, points);
    emit dbusWiimoteButtons(sequence, 0);
}

void WiimoteConnection::quitThread()
{
    quitRequest = true;
}
