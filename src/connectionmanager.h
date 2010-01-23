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

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#ifdef __syslog
    #include <syslog.h>
#endif

#include <QDBusAbstractAdaptor>
#include <QtDBus>

#include <QObject>
#include <QThread>
#include <QList>
#include <QTimer>
#include <QTime>

#include "wiimoteconnection.h"

class ConnectionManagerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.daemon.ConnectionManager")
    Q_CLASSINFO("D-Bus Introspection", ""
     "  <interface name=\"org.wiimotedev.daemon.ConnectionManager\">\n"
     "    <method name=\"dbusGetWiimoteList\">\n"
     "      <arg type=\"s\" direction=\"out\"/>\n"
     "    </method>\n"
     "    <signal name=\"dbusBatteryLifeChanged\">\n"
     "      <arg type=\"u\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "    </signal>\n"
     "    <signal name=\"dbusButtonStatusChanged\">\n"
     "      <arg type=\"u\" direction=\"in\"/>\n"
     "      <arg type=\"t\" direction=\"in\"/>\n"
     "    </signal>\n"
     "    <signal name=\"dbusInfraredTableChanged\">\n"
     "      <arg type=\"u\" direction=\"in\"/>\n"
     "      <arg type=\"s\" direction=\"in\"/>\n"
     "    </signal>\n"
     "    <signal name=\"dbusNunchukAccTableChanged\">\n"
     "      <arg type=\"u\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "      <arg type=\"d\" direction=\"in\"/>\n"
     "      <arg type=\"d\" direction=\"in\"/>\n"
     "    </signal>\n"
     "    <signal name=\"dbusWiimoteAccTableChanged\">\n"
     "      <arg type=\"u\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "      <arg type=\"d\" direction=\"in\"/>\n"
     "      <arg type=\"d\" direction=\"in\"/>\n"
     "    </signal>\n"
     "    <signal name=\"dbusWiimoteStatusChanged\">\n"
     "      <arg type=\"u\" direction=\"in\"/>\n"
     "      <arg type=\"y\" direction=\"in\"/>\n"
     "    </signal>\n"
     "  </interface>\n"
     "")
public:
    ConnectionManagerAdaptor(QObject *parent);
    virtual ~ConnectionManagerAdaptor(){};

    QStringList dbusGetWiimoteList();

signals:
    void dbusBatteryLifeChanged(quint32, quint8);
    void dbusButtonStatusChanged(quint32, quint64);
    void dbusInfraredTableChanged(quint32, QStringList);
    void dbusNunchukAccTableChanged(quint32, quint8, quint8, quint8, qreal, qreal);
    void dbusWiimoteAccTableChanged(quint32, quint8, quint8, quint8, qreal, qreal);
    void dbusWiimoteConnected(quint32);
    void dbusWiimoteDisconnected(quint32);
    void dbusWiimoteStatusChanged(quint32, quint8);

};


class ConnectionManager : public QThread
{
    Q_OBJECT

private:
    QMap< QString, quint16> wiiremoteSequence;
    QList< void*> objectList;

    bdaddr_t bdaddr_any;
    bool terminateReq;

    ConnectionManagerAdaptor *adaptor;
    WiimoteConnection *connectionObject;

public:
    ConnectionManager();
   ~ConnectionManager();

protected:
    void run();

private slots:
    void registerConnection(void *object);
    void unregisterConnection(void *object);

signals:
    void sendIRMesg(struct cwiid_ir_mesg ir);

signals:
    void dbusBatteryLifeChanged(quint32, quint8);
    void dbusButtonStatusChanged(quint32, quint64);
    void dbusInfraredTableChanged(quint32, QStringList);
    void dbusNunchukAccTableChanged(quint32, quint8, quint8, quint8, qreal, qreal);
    void dbusWiimoteAccTableChanged(quint32, quint8, quint8, quint8, qreal, qreal);
    void dbusWiimoteConnected(quint32);
    void dbusWiimoteDisconnected(quint32);
    void dbusWiimoteStatusChanged(quint32, quint8);

};

#endif // CONNECTIONMANAGER_H
