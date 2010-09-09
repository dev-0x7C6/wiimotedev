/**********************************************************************************
 * Wiimotedev daemon, wiimotedev-register tool                                    *
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

#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusReply>
#include <QSettings>
#include "include/wiimotedev/consts.h"

#define APP_NAME "wiimotedev-register"
#define APP_VERSION "1.3"

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  application.setApplicationName(APP_NAME);
  application.setApplicationVersion(APP_VERSION);


  QDBusInterface devices(WIIMOTEDEV_DBUS_SERVICE_NAME,
                         WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                         WIIMOTEDEV_DBUS_IFACE_EVENTS,
                         QDBusConnection::systemBus(), 0);

  if (!devices.isValid())
    qFatal("Cannot connect to %s %s iterface:%s", WIIMOTEDEV_DBUS_SERVICE_NAME, WIIMOTEDEV_DBUS_OBJECT_EVENTS, WIIMOTEDEV_DBUS_IFACE_EVENTS);


  QDBusReply< QStringList> reply = devices.call(QDBus::Block, "dbusGetUnregistredWiimoteList");

  if (!reply.isValid())
    qFatal("Invalid reply, dbusGetUnregistredWiimoteList");

  QStringList list = QStringList(reply.value());

  QSettings settings(WIIMOTEDEV_CONFIG_FILE, QSettings::IniFormat);
  settings.beginGroup("sequence");

  QHash < quint32, bool> field;
  foreach (const QString &name, settings.allKeys())
    field[settings.value(name, 0).toUInt()] = true;

  for (int i = list.count() - 1; i >= 0; --i)
    if (field[settings.value(list.at(i), 0).toUInt()]) list.removeAt(i);

  if (!list.count()) {
    qDebug("Nothing to register...");
    return 0;
  }

  qDebug("Registered devices:");
  int id = 0;
  foreach (const QString &name, list) {
    while (field[++id]);
    settings.setValue(name, id);
    qDebug("%s", QString("%1 as %2").arg(name, QString::number(id)).toAscii().constData());
  }

  settings.endGroup();
  settings.sync();

  QDBusInterface service(WIIMOTEDEV_DBUS_SERVICE_NAME,
                         WIIMOTEDEV_DBUS_OBJECT_SERVICE,
                         WIIMOTEDEV_DBUS_IFACE_SERVICE,
                         QDBusConnection::systemBus(), 0);


  if (!devices.isValid())
    qFatal("Cannot connect to %s %s iterface:%s", WIIMOTEDEV_DBUS_SERVICE_NAME, WIIMOTEDEV_DBUS_OBJECT_SERVICE, WIIMOTEDEV_DBUS_IFACE_SERVICE);

  QDBusReply< bool> reply2 = service.call("dbusReloadSequenceList");

  if (static_cast< bool>( reply2.value()))
    qDebug("Registration complete..."); else
    qDebug("Registration fail...");
}
