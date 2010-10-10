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

#include "core/settings.h"

const QString dbusSupportValue("wiimotedev/DBusInterface");
const QString tcpSupportValue("wiimotedev/TCPInterface");


WiimotedevSettings::WiimotedevSettings(QString file, QObject *parent):
  QObject(parent),
  config(file)
{
  settings = new QSettings(config, QSettings::IniFormat, this);
  reload();
}

void WiimotedevSettings::reload()
{
  settings->sync();

  ifaceDBusSupport = settings->value(dbusSupportValue, false).toBool();
  ifaceTcpSupport = settings->value(tcpSupportValue, false).toBool();
  tcpAllowed = settings->value("tcp/allowed", QStringList()).toStringList();
  tcpPort = settings->value("tcp/port", WIIMOTEDEV_TCP_PORT).toInt();
  powersave = settings->value("features/powersave", 0).toUInt();

  sequence.clear();

  settings->beginGroup("sequence");
  for (register int i = 0; i < settings->allKeys().count(); ++i)
    sequence[settings->allKeys().at(i)] = settings->value(settings->allKeys().at(i), 0).toUInt();

  settings->endGroup();
}

void WiimotedevSettings::setDBusInterfaceSupport(bool support)
{
  settings->setValue(dbusSupportValue, support);
  settings->sync();
}

void WiimotedevSettings::setTcpInterfaceSupport(bool support)
{
  settings->setValue(tcpSupportValue, support);
  settings->sync();
}
