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

#include "profilemanager.h"
#include "wiimoteconnection.h"
#include <QStringList>
#include <QSettings>
#include <QDebug>
#include <QMapIterator>
#include <QFile>
#include <QDBusConnection>

QMap < QString, quint64> devicebuttons;
QMap < QString, quint16> scancodes;

// wiimote filters
quint64 wiimoteButtonFilter = 0;
quint64 wiimoteShiftFilter = 0;
quint64 wiimoteTiltFilter = 0;
// nunchuk filters
quint64 nunchukButtonFilter = 0;
quint64 nunchukShiftFilter = 0;
quint64 nunchukStickFilter = 0;
quint64 nunchukTiltFilter = 0;
// classic filters
quint64 classicButtonFilter = 0;
quint64 classicShiftFilter = 0;
quint64 classicLStickFilter = 0;
quint64 classicRStickFilter = 0;
quint64 classicTiltFilter = 0;

ProfileManagerAdaptor::ProfileManagerAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

void ProfileManagerAdaptor::loadProfile(QString file)
{
    QMetaObject::invokeMethod(parent(), "loadProfile", Q_ARG(QString, file));
}

void ProfileManagerAdaptor::unloadProfile()
{
    QMetaObject::invokeMethod(parent(), "unloadProfile");
}

QString ProfileManagerAdaptor::currentProfile()
{
    QString value;
    QMetaObject::invokeMethod(parent(), "currentProfile", Q_RETURN_ARG(QString, value));
    return value;
}

ProfileManager::ProfileManager()
{
    uInputEventDevice = new UInputEventDevice();
    connect(this, SIGNAL(sendKeyEvents(QList<quint16>*,quint8)), uInputEventDevice, SLOT(recvKeyEvents(QList<quint16>*,quint8)));
    uInputEventDevice->uinput_open();

    profileName = "";

  // DBUS adaptor
    new ProfileManagerAdaptor(this);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject("/ProfileManager", this);
    connection.registerService("org.wiimotedev.ProfileManager");

}

ProfileManager::~ProfileManager()
{
    disconnect(uInputEventDevice, 0, 0, 0);
    uInputEventDevice->uinput_close();
    delete uInputEventDevice;
}

void ProfileManager::Initialization()
{
    extern QString filePathScancode;

    QSettings settings(filePathScancode, QSettings::IniFormat);
    settings.beginGroup("scancode");

    QStringList list = settings.allKeys();
    for (register qint32 i = 0; i < list.count(); ++i)
        scancodes[QString(list.at(i)).toLower()] = settings.value(list.at(i), 0).toInt();

    settings.endGroup();

    devicebuttons.insert("wiimote.1", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.2", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.a", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.b", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.plus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.minus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.home", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.forward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.backward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.shake", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.front", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.back", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.z", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.c", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.forward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.backward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.shake", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.front", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.back", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.x", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.y", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.a", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.b", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.l", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.r", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.zl", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.zr", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.minus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.plus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.home", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.right", static_cast< quint64>( true) << devicebuttons.count());


    wiimoteButtonFilter |= devicebuttons.value("wiimote.1");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.2");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.a");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.b");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.minus");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.plus");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.home");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.up");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.down");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.left");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.right");

    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.up");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.down");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.left");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.right");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.forward");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.backward");

    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.front");
    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.back");
    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.left");
    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.right");

    wiimoteButtonFilter = ~wiimoteButtonFilter;
    wiimoteShiftFilter = ~wiimoteShiftFilter;
    wiimoteTiltFilter = ~wiimoteTiltFilter;

    nunchukButtonFilter |= devicebuttons.value("nunchuk.c");
    nunchukButtonFilter |= devicebuttons.value("nunchuk.z");

    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.up");
    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.down");
    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.left");
    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.right");

    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.up");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.down");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.left");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.right");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.forward");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.backward");

    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.front");
    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.back");
    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.left");
    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.right");

    nunchukButtonFilter = ~nunchukButtonFilter;
    nunchukShiftFilter = ~nunchukShiftFilter;
    nunchukStickFilter = ~nunchukStickFilter;
    nunchukTiltFilter = ~nunchukTiltFilter;

    classicButtonFilter |= devicebuttons.value("classic.a");
    classicButtonFilter |= devicebuttons.value("classic.b");
    classicButtonFilter |= devicebuttons.value("classic.x");
    classicButtonFilter |= devicebuttons.value("classic.y");
    classicButtonFilter |= devicebuttons.value("classic.l");
    classicButtonFilter |= devicebuttons.value("classic.r");
    classicButtonFilter |= devicebuttons.value("classic.zl");
    classicButtonFilter |= devicebuttons.value("classic.zr");
    classicButtonFilter |= devicebuttons.value("classic.minus");
    classicButtonFilter |= devicebuttons.value("classic.plus");
    classicButtonFilter |= devicebuttons.value("classic.home");
    classicButtonFilter |= devicebuttons.value("classic.up");
    classicButtonFilter |= devicebuttons.value("classic.down");
    classicButtonFilter |= devicebuttons.value("classic.left");
    classicButtonFilter |= devicebuttons.value("classic.right");

    classicLStickFilter |= devicebuttons.value("classic.lstick.up");
    classicLStickFilter |= devicebuttons.value("classic.lstick.down");
    classicLStickFilter |= devicebuttons.value("classic.lstick.left");
    classicLStickFilter |= devicebuttons.value("classic.lstick.right");

    classicRStickFilter |= devicebuttons.value("classic.rstick.up");
    classicRStickFilter |= devicebuttons.value("classic.rstick.down");
    classicRStickFilter |= devicebuttons.value("classic.rstick.left");
    classicRStickFilter |= devicebuttons.value("classic.rstick.right");

    classicButtonFilter = ~classicButtonFilter;
    classicLStickFilter = ~classicLStickFilter;
    classicRStickFilter = ~classicRStickFilter;
}

void ProfileManager::Finalization()
{
    devicebuttons.clear();
}

bool ProfileManager::loadProfile(QString file)
{
    if (!QFile::exists(file))
        return false;

    profileName = file;

    QList < QStringList*> lists;
    events.clear();

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup("events");
    lists << new QStringList();
    lists.at(0)->clear();
    lists.at(0)->append(settings.allKeys());

    QRegExp reg(".*(\\[.*(\\d+)\\])");
    lists << new QStringList();
    lists << new QStringList();

    qint32 index;
    for (register qint32 i = 0; i < lists.at(0)->count(); ++i)
    {
        EventClass *event = new EventClass();
        lists.at(1)->clear();
        lists.at(1)->append(QString(lists.at(0)->at(i)).remove(QRegExp("[ ]")).toLower().split('+'));
        event->event.clear();

        for (register qint32 j = 0; j < lists.at(1)->count(); ++j)
        {
            reg.exactMatch(lists.at(1)->at(j));
            index = reg.cap(2).toInt();
            event->event.insert(index, event->event.value(index, 0) | devicebuttons.value(reg.cap(0).remove(reg.cap(1)), 0));
        }

        event->setStatus(false);

        lists.at(2)->clear();
        lists.at(2)->append(settings.value(lists.at(0)->at(i), QStringList()).toStringList());
        event->kcode.clear();

        for (register qint32 j = 0; j < lists.at(2)->count(); ++j)
            event->kcode << scancodes.value(QString(lists.at(2)->at(j)).toLower(), 0);

        events << event;
    }

    delete lists.at(2);
    delete lists.at(1);
    lists.removeAt(2);
    lists.removeAt(1);

    lists.at(0)->clear();

    settings.endGroup();

    delete lists.at(0);
    lists.removeAt(0);

    return true;
}

bool ProfileManager::unloadProfile()
{
    for (register qint32 i = 0; i < events.count(); ++i)
        delete events.at(i);
    profileName = "";
}


void ProfileManager::buttonStatusChanged(void *object, quint64 value)
{
    quint32 id = static_cast< WiimoteConnection*>( object)->getWiimoteSequence();

#ifdef deviceButtonStatus
    quint64 x = ((quint64)1 << 63) | value;
    qDebug(QString("device[%1] flags = %2").arg(QString::number(id), QString::number(x, 2).mid(1, 63)).toAscii());
#endif

    buttons.insert(id, value);

    bool matched;
    for (register qint32 i = 0; i < events.count(); ++i)
    {
        QMapIterator < quint32, quint64> event(events.at(i)->event);
        matched = true;
        while (event.hasNext())
        {
            event.next();
            qDebug() << buttons.value(event.key()) << event.value();
            matched = matched && (buttons.value(event.key()) & event.value());
        }
        if (!events.at(i)->getStatus() && matched) {
            events.at(i)->setStatus(true);
            emit sendKeyEvents(&events.at(i)->kcode, 1);
        }
        if (events.at(i)->getStatus() && !matched)
        {
            events.at(i)->setStatus(false);
            emit sendKeyEvents(&events.at(i)->kcode, 0);
        }
    }
}

void ProfileManager::wiimoteStatusChanged(void *object, quint8 status)
{
}

void ProfileManager::recvIRMesg(struct cwiid_ir_mesg ir)
{
    //qDebug() << ir.src[0].pos[0] << "x" << ir.src[0].pos[1];
}
