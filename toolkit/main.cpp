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

#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "headers/consts.h"
#include "interfaces/deviceevents.h"

#include "src/mainwindow.h"

int main(int argc, char *argv[])
{  
  QApplication application(argc, argv);
  qRegisterMetaType< uint8> ("uint8");
  qRegisterMetaType< uint16> ("uint16");
  qRegisterMetaType< uint32> ("uint32");
  qRegisterMetaType< uint64> ("uint64");
  qRegisterMetaType< int8> ("int8");
  qRegisterMetaType< int16> ("int16");
  qRegisterMetaType< int32> ("int32");
  qRegisterMetaType< int64> ("int64");

  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Utf-8"));

  DBusDeviceEventsInterface interface(WIIMOTEDEV_DBUS_SERVICE_NAME, WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                                      QDBusConnection::systemBus(), 0);

  if (!interface.isValid()) {
    QMessageBox::critical(0, "Critical", "Unable to connect with wiimotedev-daemon service", QMessageBox::Ok);
    return EXIT_FAILURE;
  }

  QList < uint> list = interface.dbusGetWiimoteList();

  int id = 1;

  if (!list.count()) {
    QMessageBox::warning(0, "Warning", "No wiiremotes connected", QMessageBox::Ok);
  }

  if (list.count() == 1)
    id = list.first();

  if (list.count() > 1) {
    SelectWiimote select;
    select.setWiimoteList(list);
    select.exec();
    id = select.getSelectedWiimote();
  }

  if (application.arguments().count() > 1)
    id = application.arguments().at(1).toInt();

  MainWindow window(&interface, id);
  QObject::connect(&interface, SIGNAL(dbusClassicControllerPlugged(uint)), &window, SLOT(dbusClassicPlugged(uint)));
  QObject::connect(&interface, SIGNAL(dbusClassicControllerUnplugged(uint)), &window, SLOT(dbusClassicUnplugged(uint)));
  QObject::connect(&interface, SIGNAL(dbusClassicControllerRStick(uint,const stickdata&)), &window, SLOT(dbusClassicControllerRStick(uint,const stickdata&)));
  QObject::connect(&interface, SIGNAL(dbusClassicControllerLStick(uint,const stickdata&)), &window, SLOT(dbusClassicControllerLStick(uint,const stickdata&)));

  QObject::connect(&interface, SIGNAL(dbusNunchukAcc(uint,const accdata&)), &window, SLOT(dbusNunchukAcc(uint,const accdata&)));
  QObject::connect(&interface, SIGNAL(dbusNunchukPlugged(uint)), &window, SLOT(dbusNunchukPlugged(uint)));
  QObject::connect(&interface, SIGNAL(dbusNunchukUnplugged(uint)), &window, SLOT(dbusNunchukUnplugged(uint)));
  QObject::connect(&interface, SIGNAL(dbusNunchukStick(uint, const stickdata&)), &window, SLOT(dbusNunchukStick(uint,const stickdata&)));

  QObject::connect(&interface, SIGNAL(dbusVirtualCursorPosition(uint,double,double,double,double)), &window, SLOT(dbusVirtualCursorPosition(uint,double,double,double,double)));
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorFound(uint)), &window, SLOT(dbusVirtualCursorFound(uint)));
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorLost(uint)), &window, SLOT(dbusVirtualCursorLost(uint)));

  QObject::connect(&interface, SIGNAL(dbusWiimoteConnected(uint)), &window, SLOT(dbusWiimoteConnected(uint)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteDisconnected(uint)), &window, SLOT(dbusWiimoteDisconnected(uint)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteAcc(uint,const accdata&)), &window, SLOT(dbusWiimoteAcc(uint,const accdata&)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteBatteryLife(uint,uint8)), &window, SLOT(dbusWiimoteBatteryLife(uint,uint8)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteGeneralButtons(uint,uint64)), &window, SLOT(dbusWiimoteGeneralButtons(uint,uint64)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteInfrared(uint, const QList< irpoint>&)), &window, SLOT(dbusWiimoteInfrared(uint, const QList<struct irpoint>&)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteLedStatusChanged(uint,uint8)), &window, SLOT(dbusWiimoteLedStatusChanged(uint,uint8)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteRumbleStatusChanged(uint,uint8)), &window, SLOT(dbusWiimoteRumbleStatusChanged(uint,uint8)));

  window.show();

  return application.exec();
}
