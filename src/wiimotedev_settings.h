/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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


#ifndef WIIMOTEDEV_SETTINGS_H
#define WIIMOTEDEV_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QStringList>

#include "wiimotedev.h"

class WiimotedevSettings : public QObject
{
    Q_OBJECT
private:
    QSettings *settings;
    QString config;

    bool ifaceDBusSupport;
    bool ifaceTcpSupport;

    QMap < QString, quint32> sequence;
    QStringList tcpAllowed;

    quint16 tcpPort;

public:
    WiimotedevSettings(QObject *parent = 0, QString file = WIIMOTEDEV_CONFIG_FILE);
    ~WiimotedevSettings();

public:
    void reload();

    inline bool dbusInterfaceSupport() { return ifaceDBusSupport; }
    inline bool tcpInterfaceSupport() { return ifaceTcpSupport; }

    inline QMap < QString, quint32> getWiiremoteSequence() { return sequence; }

    inline QStringList tcpGetAllowedHostList() { return tcpAllowed; }
    inline quint16 tcpGetPort() { return tcpPort; }

    void setDBusInterfaceSupport(bool support);
    void setTcpInterfaceSupport(bool support);
};

#endif // WIIMOTEDEV_SETTINGS_H
