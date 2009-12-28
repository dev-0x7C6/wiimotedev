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

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H


#include <QDBusAbstractAdaptor>
#include <QtDBus>

#include <QObject>
#include <QSettings>
#include <QMap>
#include <cwiid.h>
#include "uinput.h"

class ProfileManagerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.ProfileManager")
    Q_CLASSINFO("D-Bus Introspection", ""
     "  <interface name=\"org.wiimotedev.ProfileManager\">\n"
     "    <method name=\"loadProfile\">\n"
     "      <arg name=\"file\" type=\"s\" direction=\"in\"/>\n"
     "    </method>\n"
     "    <method name=\"unloadProfile\" />\n"
     "    <method name=\"currentProfile\">\n"
     "      <arg type=\"s\" direction=\"out\"/>\n"
     "    </method>\n"
     "    <signal name=\"dbusButtonStatusChanged\">\n"
     "      <arg type=\"s\" direction=\"in\"/>\n"
     "      <arg type=\"s\" direction=\"in\"/>\n"
     "    </signal>\n"
     "    <signal name=\"dbusWiimoteStatusChanged\">\n"
     "      <arg type=\"s\" direction=\"in\"/>\n"
     "      <arg type=\"s\" direction=\"in\"/>\n"
     "    </signal>\n"
     "  </interface>\n"
     "")
public:
    ProfileManagerAdaptor(QObject *parent);
    virtual ~ProfileManagerAdaptor(){};

public Q_SLOTS:
    void loadProfile(QString file);
    void unloadProfile();
    QString currentProfile();

signals:
    void dbusButtonStatusChanged(quint32, quint64);
    void dbusWiimoteStatusChanged(quint32, quint64);

};

class EventClass
{
private:
    bool work;

public:
    QMap < quint32, quint64> event;
    QList < quint16> kcode;
    void setStatus(bool status){ work = status; }
    bool getStatus() { return work; }
};

class ProfileManager : public QObject
{
    Q_OBJECT
public:
    ProfileManager();
   ~ProfileManager();
    static void Initialization();
    static void Finalization();

private:
    QString profileName;
    QList < EventClass*> events;
    QMap < quint32, quint64> buttons;

    UInputEventDevice *uInputEventDevice;

public slots:
    bool loadProfile(QString file);
    bool unloadProfile();
    QString currentProfile(){ return profileName; }

    void buttonStatusChanged(void *object, quint64 value);
    void wiimoteStatusChanged(void *object, quint8 status);

    void recvIRMesg(struct cwiid_ir_mesg ir);

signals:
    void sendKeyEvents(QList <quint16>*, quint8);

    void dbusButtonStatusChanged(quint32, quint64);
    void dbusWiimoteStatusChanged(quint32, quint64);
};

#endif // PROFILEMANAGER_H
