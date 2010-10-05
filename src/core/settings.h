/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
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

#ifndef WIIMOTEDEV_SETTINGS_H
#define WIIMOTEDEV_SETTINGS_H

#include <QSettings>
#include <QStringList>

#include "headers/consts.h"

class WiimotedevSettings : public QObject
{
private:
  QSettings *settings;
  QString config;

  bool ifaceDBusSupport;
  bool ifaceTcpSupport;

  QMap < QString, quint32> sequence;
  QStringList tcpAllowed;

  quint16 tcpPort;

  quint32 powersave;

public:
  WiimotedevSettings(QString file = WIIMOTEDEV_CONFIG_FILE, QObject *parent = 0);

public:
  void reload();

  inline bool dbusInterfaceSupport() { return ifaceDBusSupport; }
  inline bool tcpInterfaceSupport() { return ifaceTcpSupport; }

  inline QMap < QString, quint32> getWiiremoteSequence() { return sequence; }

  inline QStringList tcpGetAllowedHostList() { return tcpAllowed; }
  inline quint16 tcpGetPort() { return tcpPort; }

  inline quint32 getPowerSaveValue() { return powersave; }

  void setDBusInterfaceSupport(bool support);
  void setTcpInterfaceSupport(bool support);
};

#endif // WIIMOTEDEV_SETTINGS_H
