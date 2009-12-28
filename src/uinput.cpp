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

#include <QFile>
#include "uinput.h"


UInputCore::UInputCore()
{
    isOpen = false;
    uinputDevicePath = "";
    if (QFile::exists(QString::fromUtf8("/dev/uinput"))) uinputDevicePath = QString::fromUtf8("/dev/uinput");
    if (QFile::exists(QString::fromUtf8("/dev/input/uinput"))) uinputDevicePath = QString::fromUtf8("/dev/input/uinput");
    if (QFile::exists(QString::fromUtf8("/dev/misc/uinput"))) uinputDevicePath = QString::fromUtf8("/dev/misc/uinput");
}

void UInputCore::uinput_send_event(__u16 type, __u16 code, __s32 value)
{
    struct input_event event;
    memset(&event, 0, sizeof(event));
    event.type = type;
    event.code = code;
    event.value = value;
    write(uinput_fd, &event, sizeof(event));
}

void UInputCore::uinput_close()
{
    isOpen = false;
    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);
}


quint32 UInputCore::setUIEventBit(quint8 event)
{
    register quint32 result = 0x00;
    switch (event)
    {
        case EV_KEY: result = UI_SET_KEYBIT; break;
        case EV_REL: result = UI_SET_RELBIT; break;
        case EV_ABS: result = UI_SET_ABSBIT; break;
        case EV_MSC: result = UI_SET_MSCBIT; break;
        case EV_LED: result = UI_SET_LEDBIT; break;
        case EV_SND: result = UI_SET_SNDBIT; break;
        case EV_FF: result = UI_SET_FFBIT; break;
        case EV_SW: result = UI_SET_SWBIT; break;
    }
    return result;
}

quint32 UInputCore::getEventInterfaces(QMap< QString, quint32> map)
{
    register quint32 interfaces = 0;
    QMapIterator< QString, quint32> iter(map);
    while (iter.hasNext())
    {
        iter.next();
        switch (quint8(iter.value()))
        {
            case EV_KEY: interfaces |= true << EV_KEY; break;
            case EV_REL: interfaces |= true << EV_REL; break;
            case EV_ABS: interfaces |= true << EV_ABS; break;
            case EV_MSC: interfaces |= true << EV_MSC; break;
            case EV_SW: interfaces |= true << EV_SW; break;
            case EV_LED: interfaces |= true << EV_LED; break;
            case EV_SND: interfaces |= true << EV_SND; break;
            case EV_REP: interfaces |= true << EV_REP; break;
            case EV_FF: interfaces |= true << EV_FF; break;
            case EV_PWR: interfaces |= true << EV_PWR; break;
            case EV_FF_STATUS: interfaces |= true << EV_FF_STATUS; break;
            case EV_MAX: interfaces |= true << EV_MAX; break;
            case EV_CNT: interfaces |= true << EV_CNT; break;
        }
    }
    return interfaces;
}

bool UInputEventDevice::uinput_open()
{
    isOpen = false;
    uinput_fd = open(uinputDevicePath.toAscii(), O_WRONLY | O_NDELAY);
    if (uinput_fd == 0)
    {
        qDebug("Unable to open " + uinputDevicePath.toAscii());
        return false;
    }

    memset(&uinp,0,sizeof(uinp));
    strncpy(uinp.name, EVENT_UINPUT_DEVICE_NAME.toAscii(), EVENT_UINPUT_DEVICE_NAME.length());
    uinp.id.vendor = 0x0001;
    uinp.id.product = 0x0001;
    uinp.id.version = 0x0001;
    uinp.id.bustype = BUS_USB;

    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_EVBIT, EV_REP);

    for (quint16 i = 0; i < 256; ++i)
        ioctl(uinput_fd, UI_SET_KEYBIT, i);

    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_FORWARD);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_BACK);

    ioctl(uinput_fd, UI_SET_EVBIT, EV_REL);
    ioctl(uinput_fd, UI_SET_RELBIT, REL_X);
    ioctl(uinput_fd, UI_SET_RELBIT, REL_Y);
    ioctl(uinput_fd, UI_SET_RELBIT, REL_HWHEEL);
    ioctl(uinput_fd, UI_SET_RELBIT, REL_WHEEL);

    write(uinput_fd, &uinp, sizeof(uinp));
    if (ioctl(uinput_fd, UI_DEV_CREATE))
    {
        qDebug("Unable to create UINPUT device.");
        return false;
    }
    isOpen = true;
    return true;
}

void UInputEventDevice::uinput_mouse_wheel(qint16 rel)
{
    uinput_send_event(EV_REL, REL_WHEEL, rel);
    uinput_send_event(EV_SYN, SYN_REPORT, 0);
}

void UInputEventDevice::uinput_mouse_hwheel(qint16 rel)
{
    uinput_send_event(EV_REL, REL_HWHEEL, rel);
    uinput_send_event(EV_SYN, SYN_REPORT, 0);
}
void UInputEventDevice::uinput_mouse_move(quint32 rels, bool invertX, bool invertY)
{
    uinput_send_event(EV_REL, REL_X, invertX ? -((qint16)(rels)) : (qint16)(rels));
    uinput_send_event(EV_REL, REL_Y, invertY ? -((qint16)(rels >> 16)) : (qint16)(rels >> 16));
    uinput_send_event(EV_SYN, SYN_REPORT, 0);
}

void UInputEventDevice::recvKeyEvents(QList <quint16> *scancodes, quint8 status)
{
    for (register qint32 i = 0; i < scancodes->count(); ++i)
        sendKeyEvent(scancodes->at(i), status);
    sendSyncEvent();
}

