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

#include "wiimoteconnection.h"

#include <QtDebug>
#include <math.h>

WiimoteConnection::WiimoteConnection(QObject *parent) :QThread(parent)
{
    connected = false;
    id = 0;
    setTerminationEnabled(true);
}

WiimoteConnection::~WiimoteConnection()
{
}

void WiimoteConnection::run()
{
    if (!device) return;
    emit dbusWiimoteConnected(sequence);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(getBatteryStatus()), Qt::DirectConnection);
    timer->setInterval(60000);
    timer->start();

    int count;
    union cwiid_mesg *mesg;
    struct timespec time;

    double x, y, z, roll, pitch, vacc;
      

    struct cwiid_ir_mesg ir_mesg;
    memset(&ir_mesg, 0, sizeof(struct cwiid_ir_mesg));



    bool ButtonRequest = false;
    unsigned char BatteryLife, NewBatteryLife;

    unsigned int WiimoteStatus = STATUS_WIIMOTE_CONNECTED;
    unsigned long long WiimoteButtons = 0x00;
    unsigned long long WiimoteButtonsTmp = 0x00;

    unsigned char ClassicLStickX = 0x00;
    unsigned char ClassicLStickY = 0x00;
    unsigned char ClassicRStickX = 0x00;
    unsigned char ClassicRStickY = 0x00;
    unsigned char NunchukStickX = 0x00;
    unsigned char NunchukStickY = 0x00;

    unsigned short int LastClassicButtons = 0x00;

    struct acc_cal wiimote_calibration;
    struct acc_cal nunchuk_calibration;
    memset(&wiimote_calibration, 0, sizeof(struct acc_cal));
    memset(&nunchuk_calibration, 0, sizeof(struct acc_cal));

    double LastWiimoteAccX = 0.0;
    double LastWiimoteAccY = 0.0;
    double LastWiimoteAccZ = 0.0;

    double LastNunchukAccX = 0.0;
    double LastNunchukAccY = 0.0;
    double LastNunchukAccZ = 0.0;

    struct accdata acc;
    struct stickdata stick;

    cwiid_get_acc_cal(device, CWIID_EXT_NONE, &wiimote_calibration);

    QList< irpoint> table;
    struct irpoint point;

    double wxvalueabs, wxvalue, nxvalueabs, nxvalue, wxpow, nxpow = 0.0;
    double wyvalueabs, wyvalue, nyvalueabs, nyvalue, wypow, nypow = 0.0;
    double wzvalueabs, wzvalue, nzvalueabs, nzvalue, wzpow, nzpow = 0.0;

    qint32 wcounter, ncounter = 0;

    while (!cwiid_get_mesg(device, &count, &mesg, &time))
    {
        for (register int i = 0; i < count; ++i) switch (mesg[i].type)
        {
            case CWIID_MESG_ERROR:
                connected = false;
                WiimoteButtons = 0;

                acc.x = 0xFF / 2;
                acc.y = 0xFF / 2;
                acc.z = 0xFF / 2;

                acc.pitch = 0;
                acc.roll = 0;

                if (WiimoteStatus & STATUS_WIIMOTE_CLASSIC_CONNECTED)
                {
                    stick.x = CLASSIC_LSTICK_X_MAX / 2;
                    stick.y = CLASSIC_LSTICK_Y_MAX / 2;

                    emit dbusClassicControllerLStick(sequence, stick);

                    stick.x = CLASSIC_RSTICK_X_MAX / 2;
                    stick.y = CLASSIC_RSTICK_Y_MAX / 2;

                    emit dbusClassicControllerRStick(sequence, stick);
                    emit dbusClassicControllerButtons(sequence, WiimoteButtons);
                    emit dbusClassicControllerUnplugged(sequence);
                }

                if (WiimoteStatus & STATUS_WIIMOTE_NUNCHUK_CONNECTED)
                {
                    stick.x = NUNCHUK_STICK_X_MAX / 2;
                    stick.y = NUNCHUK_STICK_Y_MAX / 2;

                    emit dbusNunchukStick(sequence, stick);
                    emit dbusNunchukAcc(sequence, acc);
                    emit dbusNunchukButtons(sequence, WiimoteButtons);
                    emit dbusNunchukUnplugged(sequence);
                }

                point.size = -1;
                point.x = 0;
                point.y = 0;
                table << point << point << point << point;

                // emit dbusWiimoteBatteryLife(sequence, 0); don't send this sig
                // emit dbusWiimoteStatus(sequence, 0); same here

                emit dbusWiimoteAcc(sequence, acc);
                emit dbusWiimoteInfrared(sequence, table);
                emit dbusWiimoteButtons(sequence, WiimoteButtons);

                ButtonRequest = true;
                break;

            case CWIID_MESG_STATUS:
                NewBatteryLife = static_cast< unsigned char>(100.0 * (mesg[i].status_mesg.battery / static_cast< double>(CWIID_BATTERY_MAX)));
                if (BatteryLife != NewBatteryLife) {
                    BatteryLife = NewBatteryLife;
                    emit dbusWiimoteBatteryLife(sequence, BatteryLife);
                }

                switch (mesg[i].status_mesg.ext_type)
                {
                    case CWIID_EXT_NONE:    
                        if (WiimoteStatus & STATUS_WIIMOTE_CLASSIC_CONNECTED)
                        {
                            WiimoteButtons &= CLASSIC_BUTTON_NOTMASK;
                            WiimoteButtons &= CLASSIC_LSTICK_NOTMASK;
                            WiimoteButtons &= CLASSIC_RSTICK_NOTMASK;

                            stick.x = CLASSIC_LSTICK_X_MAX / 2;
                            stick.y = CLASSIC_LSTICK_Y_MAX / 2;

                            emit dbusClassicControllerLStick(sequence, stick);

                            stick.x = CLASSIC_RSTICK_X_MAX / 2;
                            stick.y = CLASSIC_RSTICK_Y_MAX / 2;

                            emit dbusClassicControllerRStick(sequence, stick);
                            emit dbusClassicControllerButtons(sequence, WiimoteButtons);
                            emit dbusClassicControllerUnplugged(sequence);
                            ButtonRequest = true;
                        }

                        if (WiimoteStatus & STATUS_WIIMOTE_NUNCHUK_CONNECTED)
                        {
                            WiimoteButtons &= NUNCHUK_BUTTON_NOTMASK;
                            WiimoteButtons &= NUNCHUK_STICK_NOTMASK;
                            WiimoteButtons &= NUNCHUK_SHIFT_NOTMASK;
                            WiimoteButtons &= NUNCHUK_TILT_NOTMASK;

                            stick.x = NUNCHUK_STICK_X_MAX / 2;
                            stick.y = NUNCHUK_STICK_Y_MAX / 2;

                            emit dbusNunchukStick(sequence, stick);

                            acc.x = 0xFF / 2;
                            acc.y = 0xFF / 2;
                            acc.z = 0xFF / 2;

                            acc.pitch = 0;
                            acc.roll = 0;

                            emit dbusNunchukAcc(sequence, acc);

                            emit dbusNunchukButtons(sequence, WiimoteButtons);
                            emit dbusNunchukUnplugged(sequence);
                            ButtonRequest = true;
                        }
                        WiimoteStatus = STATUS_WIIMOTE_CONNECTED;
                        emit dbusWiimoteStatus(sequence, WiimoteStatus);
                        break;
                        
                    case CWIID_EXT_NUNCHUK:
                        if (!(WiimoteStatus & STATUS_WIIMOTE_NUNCHUK_CONNECTED))
                            emit dbusNunchukPlugged(sequence);
                        cwiid_get_acc_cal(device, CWIID_EXT_NUNCHUK, &nunchuk_calibration);
                        WiimoteStatus |= STATUS_WIIMOTE_NUNCHUK_CONNECTED;
                        emit dbusWiimoteStatus(sequence, WiimoteStatus);
                        break;
                        
                    case CWIID_EXT_CLASSIC:
                        if (!(WiimoteStatus & STATUS_WIIMOTE_CLASSIC_CONNECTED))
                            emit dbusClassicControllerPlugged(sequence);
                        WiimoteStatus |= STATUS_WIIMOTE_CLASSIC_CONNECTED;
                        emit dbusWiimoteStatus(sequence, WiimoteStatus);
                        break;

                    default:
                        break;
                }
                break;

            case CWIID_MESG_IR:
                {
                    QList< irpoint> table;
                    irpoint point;
                    for (register int j = 0; j < 4; ++j)
                    {
                        point.size = mesg[i].ir_mesg.src[j].valid ? mesg[i].ir_mesg.src[j].size : -1;
                        point.x = mesg[i].ir_mesg.src[j].pos[0];
                        point.y = mesg[i].ir_mesg.src[j].pos[1];
                        table << point;
                    }
                    emit dbusWiimoteInfrared(sequence, table);
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

            case CWIID_MESG_NUNCHUK:             
                WiimoteButtonsTmp = WiimoteButtons;
                WiimoteButtons &= NUNCHUK_BUTTON_NOTMASK;
                WiimoteButtons &= NUNCHUK_STICK_NOTMASK;

                x =  static_cast< double>((mesg[i].nunchuk_mesg.acc[0] - nunchuk_calibration.zero[0])) /
                     (nunchuk_calibration.one[0] - nunchuk_calibration.zero[0]);
                y =  static_cast< double>((mesg[i].nunchuk_mesg.acc[1] - nunchuk_calibration.zero[1])) /
                     (nunchuk_calibration.one[1] - nunchuk_calibration.zero[1]);
                z =  static_cast< double>((mesg[i].nunchuk_mesg.acc[2] - nunchuk_calibration.zero[2])) /
                     (nunchuk_calibration.one[2] - nunchuk_calibration.zero[2]);

                if ((LastNunchukAccX != x) || (LastNunchukAccY != y) || (LastNunchukAccZ != z))
                {
                    LastNunchukAccX = x;
                    LastNunchukAccY = y;
                    LastNunchukAccZ = z;

                    roll = atan(x / z);
                    if (z <= 0.0)
                       roll += 3.14159265358979323 * ((x > 0.0) ? 1 : -1);
                    roll *= -1;
                    if (z)
                       pitch = atan(y / z * cos(roll)); else pitch = 0.0;

                    acc.x = mesg[i].nunchuk_mesg.acc[0];
                    acc.y = mesg[i].nunchuk_mesg.acc[1];
                    acc.z = mesg[i].nunchuk_mesg.acc[2];
                    acc.pitch = pitch;
                    acc.roll = roll;

                    emit dbusNunchukAcc(sequence, acc);

                    WiimoteButtons &= NUNCHUK_TILT_NOTMASK;

                    if (pitch > 0.30) WiimoteButtons  |= NUNCHUK_BTN_TILT_FRONT; else
                    if (pitch < -0.30) WiimoteButtons |= NUNCHUK_BTN_TILT_BACK;
                    if (roll < -0.45) WiimoteButtons  |= NUNCHUK_BTN_TILT_RIGHT; else
                    if (roll > 0.45) WiimoteButtons   |= NUNCHUK_BTN_TILT_LEFT;

                    vacc = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

                    WiimoteButtons &= NUNCHUK_SHIFT_NOTMASK;

                    nxpow = sqrt(pow(x, 2));
                    nypow = sqrt(pow(y, 2));
                    nzpow = sqrt(pow(z, 2));

                    if ((nxpow > 2.0) || (nypow > 2.0) || (nzpow > 2.0))
                    {
                        ncounter = 10;
                        if (nxpow > 2.0) nfXmotion << x;
                        if (nypow > 2.0) nfYmotion << y;
                        if (nzpow > 2.0) nfZmotion << z;
                    }

                    if ((ncounter == 0) && ((nfXmotion.count() > 0) || (nfYmotion.count() > 0) || (nfZmotion.count() > 0)))
                    {
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
                        {
                            WiimoteButtons |= ((nxvalue > 0) ? NUNCHUK_BTN_SHIFT_RIGHT : NUNCHUK_BTN_SHIFT_LEFT);
                        } else
                        if ((nyvalueabs > nxvalueabs) && (nyvalueabs > nzvalueabs))
                        {
                            WiimoteButtons |= ((nyvalue > 0) ? NUNCHUK_BTN_SHIFT_FORWARD : NUNCHUK_BTN_SHIFT_BACKWARD);
                        }

                    } else ncounter--;

                }

                if (mesg[i].nunchuk_mesg.buttons & CWIID_NUNCHUK_BTN_C) WiimoteButtons |= NUNCHUK_BTN_C;
                if (mesg[i].nunchuk_mesg.buttons & CWIID_NUNCHUK_BTN_Z) WiimoteButtons |= NUNCHUK_BTN_Z;


                if ((NunchukStickX != mesg[i].nunchuk_mesg.stick[0]) ||
                    (NunchukStickY != mesg[i].nunchuk_mesg.stick[1]))
                {
                    NunchukStickX = mesg[i].nunchuk_mesg.stick[0];
                    NunchukStickY = mesg[i].nunchuk_mesg.stick[1];
                    stick.x = NunchukStickX;
                    stick.y = NunchukStickY;
                    emit dbusNunchukStick(sequence, stick);
                }

                if (NunchukStickX > 178) WiimoteButtons |= NUNCHUK_BTN_STICK_RIGHT; else
                if (NunchukStickX < 78)  WiimoteButtons |= NUNCHUK_BTN_STICK_LEFT;
                if (NunchukStickY > 178) WiimoteButtons |= NUNCHUK_BTN_STICK_UP; else
                if (NunchukStickY < 78)  WiimoteButtons |= NUNCHUK_BTN_STICK_DOWN;

                if (WiimoteButtonsTmp != WiimoteButtons) {
                    ButtonRequest = true;
                    emit dbusNunchukButtons(sequence, WiimoteButtons);
                }

                break;

            case CWIID_MESG_CLASSIC:
                WiimoteButtonsTmp = WiimoteButtons;
                WiimoteButtons &= CLASSIC_LSTICK_NOTMASK;
                WiimoteButtons &= CLASSIC_RSTICK_NOTMASK;

                if ((ClassicLStickX != mesg[i].classic_mesg.l_stick[0]) ||
                    (ClassicLStickY != mesg[i].classic_mesg.l_stick[1]))
                {
                    ClassicLStickX = mesg[i].classic_mesg.l_stick[0];
                    ClassicLStickY = mesg[i].classic_mesg.l_stick[1];
                    stick.x = ClassicLStickX;
                    stick.y = ClassicLStickY;
                    emit dbusClassicControllerLStick(sequence, stick);
                }

                if ((ClassicRStickX != mesg[i].classic_mesg.r_stick[0]) ||
                    (ClassicRStickY != mesg[i].classic_mesg.r_stick[1]))
                {
                    ClassicRStickX = mesg[i].classic_mesg.r_stick[0];
                    ClassicRStickY = mesg[i].classic_mesg.r_stick[1];
                    stick.x = ClassicRStickX;
                    stick.y = ClassicRStickY;
                    emit dbusClassicControllerRStick(sequence, stick);
                }

                if (ClassicLStickX > 42) WiimoteButtons |= CLASSIC_BTN_LSTICK_RIGHT; else
                if (ClassicLStickX < 22) WiimoteButtons |= CLASSIC_BTN_LSTICK_LEFT;
                if (ClassicLStickY > 42) WiimoteButtons |= CLASSIC_BTN_LSTICK_UP; else
                if (ClassicLStickY < 22) WiimoteButtons |= CLASSIC_BTN_LSTICK_DOWN;

                if (ClassicRStickX > 21) WiimoteButtons |= CLASSIC_BTN_RSTICK_RIGHT; else
                if (ClassicRStickX < 11) WiimoteButtons |= CLASSIC_BTN_RSTICK_LEFT;
                if (ClassicRStickY > 21) WiimoteButtons |= CLASSIC_BTN_RSTICK_UP; else
                if (ClassicRStickY < 11) WiimoteButtons |= CLASSIC_BTN_LSTICK_DOWN;

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
        if(!connected) break;
    }

    cwiid_close(device);

    emit dbusWiimoteDisconnected(sequence);

    timer->stop();
    disconnect(timer, 0, 0, 0);
    delete timer;

    emit unregisterConnection(static_cast< void*>( this));   
    return;
}

bool WiimoteConnection::connectAny()
{
    if (connected) return false;

    bdaddr_t bdaddr;
    memset(&bdaddr, 0, sizeof(bdaddr_t));

    if ((device = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC)))
    {
        memcpy(&wiimotebdaddr, &bdaddr, sizeof(bdaddr_t));
        id = cwiid_get_id(device);

        cwiid_set_rpt_mode(device, CWIID_RPT_STATUS | CWIID_RPT_BTN | CWIID_RPT_IR | CWIID_RPT_NUNCHUK | CWIID_RPT_CLASSIC | CWIID_RPT_ACC);

        connected = true;
        emit registerConnection(static_cast< void*>( this));
        return true;
    }

    connected = false;
    return false;
}

void WiimoteConnection::_disconnect()
{
    if (!connected) return;
    cwiid_set_rpt_mode(device, 0);
    cwiid_close(device);
    connected = false;
    emit unregisterConnection(static_cast< void*>(this));
}

QString WiimoteConnection::getWiimoteSAddr()
{
    char addr[17];
    ba2str(&wiimotebdaddr, addr);
    return QString::fromAscii(addr, 17);
}

void WiimoteConnection::getBatteryStatus()
{
    cwiid_request_status(device);
}


quint8 WiimoteConnection::getLedStatus()
{
    return ledStatus;
}

bool WiimoteConnection::getRumbleStatus()
{
    return rumbleStatus;
}

void WiimoteConnection::setLedStatus(quint8 status)
{
    cwiid_set_led(device, status);
    ledStatus = status;
}

void WiimoteConnection::setRumbleStatus(bool status)
{
    cwiid_set_rumble(device, status);
    rumbleStatus = status;
}
