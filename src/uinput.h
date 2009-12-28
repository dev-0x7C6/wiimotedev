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

#ifndef UINPUT_H
#define UINPUT_H

#include <QMapIterator>
#include <QMap>
#include <QObject>

#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/uinput.h>

#include <QPoint>

const QString EVENT_UINPUT_DEVICE_NAME = "wiimotedev: event device";

const quint8 gamepadFixValue = 3;

const QString uinput_filename[3] = {"/dev/uinput", "/dev/input/uinput",
                                    "/dev/misc/uinput"};

const QString UNABLE_OPEN_UINPUT = "Unable to open uinput \"%1\" in module \"%2\"";
const QString UNABLE_CREATE_UINPUT = "Unable to create uinput device in module \"%1\"";

class UInputCore :public QObject
{
    Q_OBJECT
protected:
    int uinput_fd;
    struct uinput_user_dev uinp;
    struct input_event event;
    void uinput_send_event(__u16 type, __u16 code, __s32 value);

    quint32 setUIEventBit(quint8 event);
    quint32 getEventInterfaces(QMap< QString, quint32> map);

    QString uinputDevicePath;
    bool isOpen;

public:

    UInputCore();
   ~UInputCore(){};

    void uinput_set_path(QString path);
    bool check();
    void uinput_close();

    void sendKeyEvent(__u16 event, __u8 status);
    void sendSyncEvent();
};

inline void UInputCore::uinput_set_path(QString path) { uinputDevicePath = path; }
inline bool UInputCore::check() { return isOpen; }

inline void UInputCore::sendKeyEvent(__u16 event, __u8 status) { uinput_send_event(EV_KEY, event, status); }
inline void UInputCore::sendSyncEvent() { uinput_send_event(EV_SYN, SYN_REPORT, 0); }

class UInputEventDevice :public UInputCore
{
    Q_OBJECT
private:
    QPoint absmax;
    QPoint absmin;

public slots:
    void recvKeyEvents(QList <quint16> *scancodes, quint8 status);

public:
    bool uinput_open();
    void uinput_mouse_wheel(qint16 rel);
    void uinput_mouse_hwheel(qint16 rel);
    void uinput_mouse_move(quint32 rels, bool invertX = false, bool invertY = false);
};


#endif // UINPUT_H
