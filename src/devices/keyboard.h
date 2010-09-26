/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
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

#ifndef UINPUT_H
#define UINPUT_H

#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QStringList>
#include <QtCore/QFile>

#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <wiimotedev.h>

#define linux_register_evbit(x) ioctl(uinput_fd, UI_SET_EVBIT, x);
#define linux_register_keybit(x) ioctl(uinput_fd, UI_SET_KEYBIT, x);
#define linux_register_relbit(x) ioctl(uinput_fd, UI_SET_RELBIT, x);
#define linux_register_absbit(x) ioctl(uinput_fd, UI_SET_ABSBIT, x);

#define linux_abs_set_range(abs, max, min) dev.absmax[abs] = max; \
                                           dev.absmin[abs] = min; \
                                           dev.absflat[abs] = 0; \
                                           dev.absfuzz[abs] = 0;

#define UINPUT_PRODUCT_ID 0x01
#define UINPUT_VENDOR_ID 0x01
#define UINPUT_VERSION_ID 0x01
#define UINPUT_BUSTYPE_ID BUS_USB

const QStringList UInputLocation(QStringList() << "/dev/uinput" << "/dev/input/uinput" << "/dev/misc/uinput");

namespace defaults
{
/* General object ----------------------------------------------------------------*/
    namespace UInputObject {
        const QString deviceSignature("wiimotedev-uinput: default event device");
    }

/* Gamepads objects --------------------------------------------------------------*/
    namespace UInputClassicGamepad {
        const QString deviceSignature("wiimotedev-uinput: virtual classic gamepad");
        const quint16 lstick_max = (0x3F >> 1) + (0x3F >> 2) + 4;
        const quint16 lstick_min = (0x3F >> 1) - (0x3F >> 2) - 4;
        const quint16 rstick_max = (0x1F >> 1) + (0x1F >> 2) + 2;
        const quint16 rstick_min = (0x1F >> 1) - (0x1F >> 2) - 2;
        const qint8 dpad_max =  1;
        const qint8 dpad_min = -1;
    }

    namespace UInputWiimoteGamepad {
        const QString deviceSignature("wiimotedev-uinput: virtual wiiremote+nunchuk gamepad");
        const quint16 stick_max = (0xFF >> 1) + (0xFF >> 2);
        const quint16 stick_min = (0xFF >> 1) - (0xFF >> 2);
        const qint8 dpad_max =  1;
        const qint8 dpad_min = -1;
        const qint16 pitch_max =  180;
        const qint16 pitch_min = -180;
        const qint16 roll_max =  180;
        const qint16 roll_min = -180;
    }

/* IO Devices objects ------------------------------------------------------------*/
    namespace UInputEvent
    {
        const QString deviceSignature("wiimotedev-uinput: virtual keyboard/mouse");
    }
    namespace UInputMouse
    {
        const QString deviceSignature("wiimotedev-uinput: virtual mouse");
    }

    namespace classic
    {

    }
}

class UInputObject :public QObject
{
    Q_OBJECT
protected:
    QString uinputFile;
    bool alreadyOpened;

    int uinput_fd;
    struct uinput_user_dev dev;

public:
    UInputObject(QObject *parent = 0);

    static inline const char *staticInterfaceName() { return "UInputObject"; }

    virtual bool uinput_open();
    virtual void uinput_close(bool force = true)
    {
        if (!force) if (!alreadyOpened) return;
        ioctl(uinput_fd, UI_DEV_DESTROY);
        close(uinput_fd);
        uinput_fd = 0;
        alreadyOpened = false;
    }

    QString path(){ return uinputFile; }

    void sendEvent(quint16 type, quint16 code, qint32 value);
    inline void sendEventSync() { sendEvent(EV_SYN, SYN_REPORT, 0); };

};

class UInputClassicGamepad: public UInputObject
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName() { return "UInputClassicGamepad"; }

    UInputClassicGamepad() :UInputObject() {};
    bool uinput_open();

public slots:
    void classicButtons(quint32 id, quint64 buttons);
    void classicLStick(quint32 id, quint16 x, quint16 y);
    void classicRStick(quint32 id, quint16 x, quint16 y);
};

class UInputWiimoteGamepad: public UInputObject
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName() { return "UInputWiimoteGamepad"; }

    UInputWiimoteGamepad() :UInputObject() {};
    bool uinput_open();

public slots:
    void wiimoteButtons(quint32 id, quint64 buttons);
    void wiimoteAcc(quint32 id, struct accdata);

    void nunchukButtons(quint32 id , quint64 buttons);
    void nunchukStick(quint32 id, qint16 x, qint16 y);
    void nunchukAcc(quint32 id, struct accdata acc);
};

class UInputEvent: public UInputObject
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName() { return "UInputKeyboard"; }
    UInputEvent() :UInputObject() {}
    bool uinput_open(bool replay = true);

public slots:
    void pressKeyboardButton(quint16 button);
    void releaseKeyboardButton(quint16 button);
    void pressKeyboardButtonOnce(quint16 button);

    void moveMousePointerRel(qint32 x, qint32 y);

};


class UInputMouse: public UInputObject
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName() { return "UInputMouse"; }
    UInputMouse() :UInputObject() {}
    bool uinput_open(QRect absRect = QRect(-512, -384, 1024, 768), bool abs = false);

public slots:
    void moveMousePointerRel(qint32 x, qint32 y);
    void moveMousePointerAbs(qint32 x, qint32 y);
    void moveMouseVWheel(qint32 direction);
    void moveMouseHWheel(qint32 direction);
    void pressMouseButton(quint16 button);
    void releaseMouseButton(quint16 button);
};

#endif // UINPUT_H
