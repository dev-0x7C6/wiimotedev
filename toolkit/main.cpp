/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
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

#include "linux/usr/include/wiimotedev/consts.h"
#include "linux/usr/include/wiimotedev/deviceevents.h"

#include "src/mainwindow.h"
#include "src/toolkitmainwindow.h"
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

  DBusDeviceEventsInterface interface;

  if (!interface.isValid()) {
    QMessageBox::critical(0, "Critical", "Unable to connect with wiimotedev-daemon service", QMessageBox::Ok);
    return EXIT_FAILURE;
  }

  MainWindow *window = new MainWindow(&interface, 0);
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorPosition(uint,double,double,double,double)), window, SLOT(dbusVirtualCursorPosition(uint,double,double,double,double)));
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorFound(uint)), window, SLOT(dbusVirtualCursorFound(uint)));
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorLost(uint)), window, SLOT(dbusVirtualCursorLost(uint)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteAcc(uint,const accdata&)), window, SLOT(dbusWiimoteAcc(uint,const accdata&)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteInfrared(uint, const QList< irpoint>&)), window, SLOT(dbusWiimoteInfrared(uint, const QList<struct irpoint>&)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteDisconnected(uint)), window, SLOT(dbusWiimoteDisconnected(uint)));

  ToolkitMainWindow toolkit(&interface, window);
  toolkit.show();

  return application.exec();
}
