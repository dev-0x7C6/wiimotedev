/**********************************************************************************
 * Wiimotedev client, wiiremote system service                                    *
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

#include <QMapIterator>
#include <QSettings>
#include <QHostAddress>

#include "network/manager.h"

extern bool additional_debug;

ConnectionManager::ConnectionManager() : terminateReq(false)
{
/* Register Meta Types ---------------------------------------------- */

    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
    qRegisterMetaType< QList< accdata> >("QList< accdata>");
    qRegisterMetaType< QList< stickdata> >("QList< stickdata>");

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< accdata>("accdata");
    qRegisterMetaType< stickdata>("stickdata");

/* Setup ------------------------------------------------------------ */

  //  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

/* Load rules ------------------------------------------------------- */
    QSettings settings(confFile, QSettings::IniFormat);

    tcpHostname = settings.value(clientSection + deftcpHostname, QString()).toString();
    tcpTimeout = settings.value(clientSection + deftcpTimeout, 30).toInt();
    tcpPort =  settings.value(clientSection + deftcpPort, 50091).toInt();

/* DBus interface --------------------------------------------------- */
    QDBusConnection connection = QDBusConnection::systemBus();

    dbusDeviceEventsAdaptor = new DBusDeviceEventsAdaptorWrapper(this, connection);
    dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
    bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

    connect(this, SIGNAL(dbusReportUnregistredWiiremote(QString)), dbusDeviceEventsAdaptor, SIGNAL(dbusReportUnregistredWiiremote(QString)));

#ifdef USE_SYSLOG
    if (additional_debug) {
      systemlog::debug(QString("register %1 object %2").arg(WIIMOTEDEV_DBUS_OBJECT_EVENTS, dbusDeviceEventsAdaptor->isRegistred() ? "done" : "failed"));
      systemlog::debug(QString("register %1 object %2").arg(WIIMOTEDEV_DBUS_OBJECT_SERVICE, dbusServiceAdaptor->isRegistred() ? "done" : "failed"));
      systemlog::debug(QString("register %1 service %2").arg(WIIMOTEDEV_DBUS_SERVICE_NAME, registred ? "done" : "failed"));
    }

    if (!(dbusDeviceEventsAdaptor->isRegistred() &&
          dbusServiceAdaptor->isRegistred() && registred))
      systemlog::error(QString("can not register dbus service"));
#endif

    connect(this, SIGNAL(dbusWiimoteGeneralButtons(quint32, quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)));

    connect(this, SIGNAL(dbusWiimoteConnected(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteConnected(quint32)));
    connect(this, SIGNAL(dbusWiimoteDisconnected(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteDisconnected(quint32)));
    connect(this, SIGNAL(dbusWiimoteBatteryLife(quint32, quint8)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteBatteryLife(quint32, quint8)));
    connect(this, SIGNAL(dbusWiimoteButtons(quint32, quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteButtons(quint32, quint64)));
    connect(this, SIGNAL(dbusWiimoteStatus(quint32, quint8)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteStatus(quint32, quint8)));
    connect(this, SIGNAL(dbusWiimoteInfrared(quint32, QList< struct irpoint>)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteInfrared(quint32, QList< struct irpoint>)));
    connect(this, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusWiimoteAcc(quint32, struct accdata)));

    connect(this, SIGNAL(dbusNunchukPlugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukPlugged(quint32)));
    connect(this, SIGNAL(dbusNunchukUnplugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukUnplugged(quint32)));
    connect(this, SIGNAL(dbusNunchukButtons(quint32, quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukButtons(quint32, quint64)));
    connect(this, SIGNAL(dbusNunchukStick(quint32, struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukStick(quint32, struct stickdata)));
    connect(this, SIGNAL(dbusNunchukAcc(quint32, struct accdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusNunchukAcc(quint32, struct accdata)));

    connect(this, SIGNAL(dbusClassicControllerPlugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerPlugged(quint32)));
    connect(this, SIGNAL(dbusClassicControllerUnplugged(quint32)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerUnplugged(quint32)));
    connect(this, SIGNAL(dbusClassicControllerButtons(quint32, quint64)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerButtons(quint32, quint64)));
    connect(this, SIGNAL(dbusClassicControllerLStick(quint32, struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerLStick(quint32, struct stickdata)));
    connect(this, SIGNAL(dbusClassicControllerRStick(quint32, struct stickdata)), dbusDeviceEventsAdaptor, SIGNAL(dbusClassicControllerRStick(quint32, struct stickdata)));

}

ConnectionManager::~ConnectionManager()
{
}

void ConnectionManager::run()
{
    socket = new QTcpSocket();
    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(quit()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::QueuedConnection);

    timer->setInterval(tcpTimeout * 1000);

    do {
       if (!socket->isValid())
           socket->connectToHost(tcpHostname, tcpPort);
       if (!terminateReq) {
           timer->start();
           exec();
           timer->stop();
       }
    } while (!terminateReq);

    delete socket;
}

void ConnectionManager::terminateRequest()
{
    terminateReq = true;
    if (socket->isValid())
        socket->disconnectFromHost();
    quit();
}

void ConnectionManager::readyRead()
{
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_4_0);

    quint16 signal;
    quint32 id;
    quint32 count;
    quint64 value;
    quint8 status;

    struct stickdata stick;
    struct accdata acc;
    QList < struct irpoint> irpoints;

    do { stream >> signal >> id; switch (signal)
    {
        case iddbusWiimoteGeneralButtons:
            stream >> value;
            emit dbusWiimoteGeneralButtons(id, value);
            break;

        case iddbusWiimoteConnected:
            emit dbusWiimoteConnected(id);
            break;

        case iddbusWiimoteDisconnected:
            emit dbusWiimoteDisconnected(id);
            break;

        case iddbusWiimoteBatteryLife:
            stream >> status;
            emit dbusWiimoteBatteryLife(id, status);
            break;

        case iddbusWiimoteButtons:
            stream >> value;
            emit dbusWiimoteButtons(id, value);
            break;

        case iddbusWiimoteStatus:
            stream >> status;
            emit dbusWiimoteStatus(id, status);
            break;

        case iddbusWiimoteInfrared:
            stream >> count;

            for (register int i = 0; i < count; ++i) {
                struct irpoint ir;
                stream >> ir.size;
                stream >> ir.x;
                stream >> ir.y;
                irpoints << ir;
            }

            emit dbusWiimoteInfrared(id, irpoints);
            break;

        case iddbusWiimoteAcc:
            stream >> acc.pitch;
            stream >> acc.roll;
            stream >> acc.x;
            stream >> acc.y;
            stream >> acc.z;

            emit dbusWiimoteAcc(id, acc);
            break;

        case iddbusNunchukPlugged:
            emit dbusNunchukPlugged(id);
            break;

        case iddbusNunchukUnplugged:
            emit dbusNunchukUnplugged(id);
            break;

        case iddbusNunchukButtons:
            stream >> value;
            emit dbusNunchukButtons(id, value);
            break;

        case iddbusNunchukStick:
            stream >> stick.x;
            stream >> stick.y;
            emit dbusNunchukStick(id, stick);
            break;

        case iddbusNunchukAcc:
            stream >> acc.pitch;
            stream >> acc.roll;
            stream >> acc.x;
            stream >> acc.y;
            stream >> acc.z;

            emit dbusNunchukAcc(id, acc);
            break;

        case iddbusClassicControllerPlugged:
            emit dbusClassicControllerPlugged(id);
            break;

        case iddbusClassicControllerUnplugged:
            emit dbusClassicControllerUnplugged(id);
            break;

        case iddbusClassicControllerButtons:
            stream >> value;
            emit dbusClassicControllerButtons(id, value);
            break;

        case iddbusClassicControllerLStick:
            stream >> stick.x;
            stream >> stick.y;
            emit dbusClassicControllerLStick(id, stick);
            break;

        case iddbusClassicControllerRStick:
            stream >> stick.x;
            stream >> stick.y;
            emit dbusClassicControllerRStick(id, stick);
            break;
        }
    } while (!stream.atEnd());
}

quint8 ConnectionManager::dbusWiimoteGetLedStatus(quint32 id)
{
}


bool ConnectionManager::dbusWiimoteGetRumbleStatus(quint32 id)
{
}

bool ConnectionManager::dbusWiimoteSetLedStatus(quint32 id, quint8 status)
{
}

bool ConnectionManager::dbusWiimoteSetRumbleStatus(quint32 id, bool status)
{
}
