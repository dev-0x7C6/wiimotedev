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

#include "profilemanager.h"

QDBusReply< void> DBusProfileManagerInterface::loadProfile(QString file)
{
  QList<QVariant> argumentList;
  argumentList << file;
  return callWithArgumentList(QDBus::Block, "loadProfile", argumentList);
}

QDBusReply< void> DBusProfileManagerInterface::unloadProfile()
{
  QList<QVariant> argumentList;
  return callWithArgumentList(QDBus::Block, "unloadProfile", argumentList);
}

QDBusReply< QString> DBusProfileManagerInterface::currentProfile()
{
  QList<QVariant> argumentList;
  return callWithArgumentList(QDBus::Block, "currentProfile", argumentList);
}

