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

#include <QtSingleApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "config.h"
#include "headers/consts.h"
#include "src/interfaces/deviceevents.h"

#include "src/mainwindow.h"

int main(int argc, char *argv[])
{
  QtSingleApplication application(argc, argv);

  if (application.isRunning())
   return 0;

  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Utf-8"));

  DBusDeviceEventsInterface interface(WIIMOTEDEV_DBUS_SERVICE_NAME, WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                                      QDBusConnection::systemBus(), 0);


  MainWindow window(&interface);
//  QObject::connect(&interface, SIGNAL(dbusClassicControllerRStick(quint32,const stickdata&)), &window, SLOT(dbusClassicControllerRStick(quint32,const stickdata&)));
//  QObject::connect(&interface, SIGNAL(dbusClassicControllerLStick(quint32,const stickdata&)), &window, SLOT(dbusClassicControllerLStick(quint32,const stickdata&)));

//  QObject::connect(&interface, SIGNAL(dbusNunchukAcc(quint32,const accdata&)), &window, SLOT(dbusNunchukAcc(quint32,const accdata&)));
//  QObject::connect(&interface, SIGNAL(dbusNunchukPlugged(quint32)), &window, SLOT(dbusNunchukPlugged(quint32)));
//  QObject::connect(&interface, SIGNAL(dbusNunchukUnplugged(quint32)), &window, SLOT(dbusNunchukUnplugged(quint32)));
//  QObject::connect(&interface, SIGNAL(dbusNunchukStick(quint32, const stickdata&)), &window, SLOT(dbusNunchukStick(quint32,const stickdata&)));

    QObject::connect(&interface, SIGNAL(dbusWiimoteConnected(quint32)), &window, SLOT(dbusWiimoteConnected(quint32)));
    QObject::connect(&interface, SIGNAL(dbusWiimoteDisconnected(quint32)), &window, SLOT(dbusWiimoteDisconnected(quint32)));
//  QObject::connect(&interface, SIGNAL(dbusWiimoteAcc(quint32,const accdata&)), &window, SLOT(dbusWiimoteAcc(quint32,const accdata&)));
//  QObject::connect(&interface, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), &window, SLOT(dbusWiimoteBatteryLife(quint32,quint8)));
    QObject::connect(&interface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), &window, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));
//  QObject::connect(&interface, SIGNAL(dbusWiimoteInfrared(quint32, const QList< irpoint>&)), &window, SLOT(dbusWiimoteInfrared(quint32, const QList<struct irpoint>&)));
//  QObject::connect(&interface, SIGNAL(dbusWiimoteLedStatusChanged(quint32,quint8)), &window, SLOT(dbusWiimoteLedStatusChanged(quint32,quint8)));
//  QObject::connect(&interface, SIGNAL(dbusWiimoteRumbleStatusChanged(quint32,quint8)), &window, SLOT(dbusWiimoteRumbleStatusChanged(quint32,quint8)));


  return application.exec();
}
