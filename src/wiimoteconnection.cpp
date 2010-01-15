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

extern QMap< QString, quint64> devicebuttons;
extern quint64 wiimoteButtonFilter;
extern quint64 wiimoteShiftFilter;
extern quint64 wiimoteTiltFilter;

extern quint64 nunchukButtonFilter;
extern quint64 nunchukShiftFilter;
extern quint64 nunchukStickFilter;
extern quint64 nunchukTiltFilter;

extern quint64 classicButtonFilter;
extern quint64 classicLStickFilter;
extern quint64 classicRStickFilter;

WiimoteConnection::WiimoteConnection(QObject *parent) :QThread(parent)
{
    deviceStatus = device_wiimote_connected;
    memset(&wiimote_calibration, 0, sizeof(struct acc_cal));
    memset(&nunchuk_calibration, 0, sizeof(struct acc_cal));
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

    if (verboseLevel >= 4)
        qDebug(QString("Wiimotedev: starting thread 0x%1").arg(QString::number(currentThreadId(), 0x10)).toAscii());


    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(getBatteryStatus()), Qt::DirectConnection);
    timer->setInterval(60000);
    timer->start();

    quint64 buttons = 0;
    quint64 lastbtn = 0;
    int count;
    union cwiid_mesg *mesg;
    struct timespec time;

    quint16 wiimotebtn = 0;
    quint16 nunchukbtn = 0;
    quint16 classicbtn = 0;

    double x, y, z, acc, roll, pitch;
      
    struct cwiid_nunchuk_mesg nunchuk_mesg;
    struct cwiid_classic_mesg classic_mesg;
    struct cwiid_ir_mesg ir_mesg;

    memset(&nunchuk_mesg, 0, sizeof(struct cwiid_nunchuk_mesg));
    memset(&classic_mesg, 0, sizeof(struct cwiid_classic_mesg));
    memset(&ir_mesg, 0, sizeof(struct cwiid_ir_mesg));

    batteryLife = 0;

    while (!cwiid_get_mesg(device, &count, &mesg, &time))
    {
        for (register int i = 0; i < count; ++i) switch (mesg[i].type)
        {
            case CWIID_MESG_ERROR:
                connected = false;
                buttons = 0;
                break;
            case CWIID_MESG_STATUS:
                newBatteryLife = static_cast< quint8>(100.0 * (mesg[i].status_mesg.battery / double(CWIID_BATTERY_MAX)));
                if (batteryLife != newBatteryLife) {
                    batteryLife = newBatteryLife;
                    emit batteryLifeChanged(static_cast< void*>( this), batteryLife);
                    emit dbusBatteryLifeChanged(getWiimoteSequence(), batteryLife);
                }

                switch (mesg[i].status_mesg.ext_type)
                {
                    case CWIID_EXT_NONE:
                        if ((deviceStatus & device_nunchuk_connected) ||
                            (deviceStatus & device_classic_connected))
                            buttons &= ~(wiimoteButtonFilter & wiimoteShiftFilter & wiimoteTiltFilter);
                            deviceStatus = device_wiimote_connected;
                            emit wiimoteStatusChanged(static_cast< void*>( this), deviceStatus);
                            emit dbusWiimoteStatusChanged(getWiimoteSequence(), deviceStatus);
                        break;
                    case CWIID_EXT_NUNCHUK:
                        if (!(deviceStatus & device_nunchuk_connected))
                        {
                            cwiid_get_acc_cal(device, CWIID_EXT_NUNCHUK, &nunchuk_calibration);
                            deviceStatus |= device_nunchuk_connected;
                            emit wiimoteStatusChanged(static_cast< void*>( this), deviceStatus);
                            emit dbusWiimoteStatusChanged(getWiimoteSequence(), deviceStatus);
                        }
                        break;
                    case CWIID_EXT_CLASSIC:
                        if (!(deviceStatus & device_classic_connected))
                        {
                            deviceStatus |= device_classic_connected;
                            emit wiimoteStatusChanged(static_cast< void*>( this), deviceStatus);
                            emit dbusWiimoteStatusChanged(getWiimoteSequence(), deviceStatus);
                        }
                        break;
                }
                break;

            case CWIID_MESG_IR:
                emit infraredTableChanged(static_cast< void*>( this), mesg[i].ir_mesg);
                {
                    QStringList table;
                    for (register int j = 0; j < 4; ++j)
                    {
                        table << QString::number(mesg[i].ir_mesg.src[j].valid ? mesg[i].ir_mesg.src[j].size : -1);
                        table << QString::number(mesg[i].ir_mesg.src[j].pos[0]);
                        table << QString::number(mesg[i].ir_mesg.src[j].pos[1]);
                    }
                    emit dbusInfraredTableChanged(getWiimoteSequence(), table);
                }

                break;

            case CWIID_MESG_BTN:
                if (wiimotebtn != mesg[i].btn_mesg.buttons)
                    wiimotebtn = mesg[i].btn_mesg.buttons; else break;

                buttons &= wiimoteButtonFilter;
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_1) buttons |= devicebuttons.value("wiimote.1");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_2) buttons |= devicebuttons.value("wiimote.2");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_A) buttons |= devicebuttons.value("wiimote.a");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_B) buttons |= devicebuttons.value("wiimote.b");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_MINUS) buttons |= devicebuttons.value("wiimote.minus");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_PLUS) buttons |= devicebuttons.value("wiimote.plus");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_HOME) buttons |= devicebuttons.value("wiimote.home");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_UP) buttons |= devicebuttons.value("wiimote.up");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_DOWN) buttons |= devicebuttons.value("wiimote.down");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_LEFT) buttons |= devicebuttons.value("wiimote.left");
                if (mesg[i].btn_mesg.buttons & CWIID_BTN_RIGHT) buttons |= devicebuttons.value("wiimote.right");
                break;

            case CWIID_MESG_ACC:
                x =  static_cast< double>((mesg[i].acc_mesg.acc[0] - wiimote_calibration.zero[0])) /
                     (wiimote_calibration.one[0] - wiimote_calibration.zero[0]);
                y =  static_cast< double>((mesg[i].acc_mesg.acc[1] - wiimote_calibration.zero[1])) /
                     (wiimote_calibration.one[1] - wiimote_calibration.zero[1]);
                z =  static_cast< double>((mesg[i].acc_mesg.acc[2] - wiimote_calibration.zero[2])) /
                     (wiimote_calibration.one[2] - wiimote_calibration.zero[2]);

                roll = atan(x / z);
                if (z <= 0.0)
                   roll += 3.14159265358979323 * ((x > 0.0) ? 1 : -1);
                roll *= -1;
                if (z)
                    pitch = atan(y / z * cos(roll)); else pitch = 0.0;

                emit dbusWiimoteAccTableChanged(getWiimoteSequence(),
                                                mesg[i].acc_mesg.acc[0],
                                                mesg[i].acc_mesg.acc[1],
                                                mesg[i].acc_mesg.acc[2],
                                                pitch, roll);

                buttons = buttons & wiimoteTiltFilter;

                if (pitch < -0.30) buttons |= devicebuttons.value("wiimote.tilt.back"); else
                if (pitch > 0.30)  buttons |= devicebuttons.value("wiimote.tilt.front");
                if (roll < -0.45)  buttons |= devicebuttons.value("wiimote.tilt.right"); else
                if (roll > 0.45)   buttons |= devicebuttons.value("wiimote.tilt.left");

                break;

            case CWIID_MESG_NUNCHUK:

                x =  static_cast< double>((mesg[i].nunchuk_mesg.acc[0] - nunchuk_calibration.zero[0])) /
                     (nunchuk_calibration.one[0] - nunchuk_calibration.zero[0]);
                y =  static_cast< double>((mesg[i].nunchuk_mesg.acc[1] - nunchuk_calibration.zero[1])) /
                     (nunchuk_calibration.one[1] - nunchuk_calibration.zero[1]);
                z =  static_cast< double>((mesg[i].nunchuk_mesg.acc[2] - nunchuk_calibration.zero[2])) /
                     (nunchuk_calibration.one[2] - nunchuk_calibration.zero[2]);

                roll = atan(x / z);
                if (z <= 0.0)
                   roll += 3.14159265358979323 * ((x > 0.0) ? 1 : -1);
                roll *= -1;
                if (z)
                   pitch = atan(y / z * cos(roll)); else pitch = 0.0;

                emit dbusNunchukAccTableChanged(getWiimoteSequence(),
                                                mesg[i].nunchuk_mesg.acc[0],
                                                mesg[i].nunchuk_mesg.acc[1],
                                                mesg[i].nunchuk_mesg.acc[2],
                                                pitch, roll);

                buttons = buttons & nunchukTiltFilter;

                if (pitch < -0.30) buttons |= devicebuttons.value("nunchuk.tilt.back"); else
                if (pitch > 0.30)  buttons |= devicebuttons.value("nunchuk.tilt.front");
                if (roll < -0.45)  buttons |= devicebuttons.value("nunchuk.tilt.right"); else
                if (roll > 0.45)   buttons |= devicebuttons.value("nunchuk.tilt.left");


                if ((nunchuk_mesg.stick[0] != mesg[i].nunchuk_mesg.stick[0]) ||
                    (nunchuk_mesg.stick[1] != mesg[i].nunchuk_mesg.stick[1]))
                {
                    nunchuk_mesg.stick[0] = mesg[i].nunchuk_mesg.stick[0];
                    nunchuk_mesg.stick[1] = mesg[i].nunchuk_mesg.stick[1];
                    buttons &= nunchukStickFilter;

                    if (nunchuk_mesg.stick[0] > 178) buttons |= devicebuttons.value("nunchuk.stick.right"); else
                    if (nunchuk_mesg.stick[0] < 78) buttons |= devicebuttons.value("nunchuk.stick.left");
                    if (nunchuk_mesg.stick[1] > 178) buttons |= devicebuttons.value("nunchuk.stick.up"); else
                    if (nunchuk_mesg.stick[1] < 78) buttons |= devicebuttons.value("nunchuk.stick.down");
                }
                
                if (nunchuk_mesg.buttons != mesg[i].nunchuk_mesg.buttons){
                    nunchuk_mesg.buttons = mesg[i].nunchuk_mesg.buttons;
                    buttons &= nunchukButtonFilter;
                    if (mesg[i].nunchuk_mesg.buttons & CWIID_NUNCHUK_BTN_C) buttons |= devicebuttons.value("nunchuk.c");
                    if (mesg[i].nunchuk_mesg.buttons & CWIID_NUNCHUK_BTN_Z) buttons |= devicebuttons.value("nunchuk.z");
                }
                break;

            case CWIID_MESG_CLASSIC:
                if ((classic_mesg.l_stick[0] != mesg[i].classic_mesg.l_stick[0]) ||
                    (classic_mesg.l_stick[0] != mesg[i].classic_mesg.l_stick[1]))
                {
                    classic_mesg.l_stick[0] = mesg[i].classic_mesg.l_stick[0];
                    classic_mesg.l_stick[1] = mesg[i].classic_mesg.l_stick[1];
                    buttons &= classicLStickFilter;

                    if (classic_mesg.l_stick[0] > 42) buttons |= devicebuttons.value("classic.lstick.right"); else
                    if (classic_mesg.l_stick[0] < 22) buttons |= devicebuttons.value("classic.lstick.left");
                    if (classic_mesg.l_stick[1] > 42) buttons |= devicebuttons.value("classic.lstick.up"); else
                    if (classic_mesg.l_stick[1] < 22) buttons |= devicebuttons.value("classic.lstick.down");
                }

                if ((classic_mesg.r_stick[0] != mesg[i].classic_mesg.r_stick[0]) ||
                    (classic_mesg.r_stick[0] != mesg[i].classic_mesg.r_stick[1]))
                {
                    classic_mesg.r_stick[0] = mesg[i].classic_mesg.r_stick[0];
                    classic_mesg.r_stick[1] = mesg[i].classic_mesg.r_stick[1];
                    buttons &= classicRStickFilter;

                    if (classic_mesg.r_stick[0] > 21) buttons |= devicebuttons.value("classic.rstick.right"); else
                    if (classic_mesg.r_stick[0] < 11) buttons |= devicebuttons.value("classic.rstick.left");
                    if (classic_mesg.r_stick[1] > 21) buttons |= devicebuttons.value("classic.rstick.up"); else
                    if (classic_mesg.r_stick[1] < 11) buttons |= devicebuttons.value("classic.rstick.down");
                }


                if (classic_mesg.buttons != mesg[i].classic_mesg.buttons)
                {
                    classic_mesg.buttons = mesg[i].classic_mesg.buttons;
                    buttons &= classicButtonFilter;

                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_A) buttons |= devicebuttons.value("classic.a");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_B) buttons |= devicebuttons.value("classic.b");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_X) buttons |= devicebuttons.value("classic.x");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_Y) buttons |= devicebuttons.value("classic.y");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_MINUS) buttons |= devicebuttons.value("classic.minus");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_PLUS) buttons |= devicebuttons.value("classic.plus");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_HOME) buttons |= devicebuttons.value("classic.home");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_L) buttons |= devicebuttons.value("classic.l");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_R) buttons |= devicebuttons.value("classic.r");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_ZL) buttons |= devicebuttons.value("classic.zl");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_ZR) buttons |= devicebuttons.value("classic.zr");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_UP) buttons |= devicebuttons.value("classic.up");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_DOWN) buttons |= devicebuttons.value("classic.down");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_LEFT) buttons |= devicebuttons.value("classic.left");
                    if (mesg[i].classic_mesg.buttons & CWIID_CLASSIC_BTN_RIGHT) buttons |= devicebuttons.value("classic.right");
                }
                break;
        }


        if (buttons != lastbtn)
        {
            lastbtn = buttons;
            emit buttonStatusChanged(static_cast< void*>( this), buttons);
            emit dbusButtonStatusChanged(getWiimoteSequence(), buttons);
        }
        delete mesg;
        if(!connected) break;
    }

    cwiid_close(device);

    if (verboseLevel >= 4)
        qDebug(QString("Wiimotedev: is going down 0x%1").arg(QString::number(currentThreadId(), 0x10)).toAscii());

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

        cwiid_get_acc_cal(device, CWIID_EXT_NONE, &wiimote_calibration);

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
