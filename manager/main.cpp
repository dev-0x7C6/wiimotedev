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

#include "src/infraredcameraview.h"
#include "src/wiimoterawstream.h"
#include "src/mainwindow.h"

int main(int argc, char *argv[])
{  
  QApplication application(argc, argv);
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Utf-8"));

  WiimotedevDeviceEvents interface;

  if (!interface.isValid()) {
    QMessageBox::critical(0, "Critical", "Unable to connect with wiimotedev-daemon service", QMessageBox::Ok);
    return EXIT_FAILURE;
  }

  InfraredCameraView *infrared = new InfraredCameraView(&interface, 0);
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorPosition(uint,double,double,double,double)), infrared, SLOT(dbusVirtualCursorPosition(uint,double,double,double,double)));
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorFound(uint)), infrared, SLOT(dbusVirtualCursorFound(uint)));
  QObject::connect(&interface, SIGNAL(dbusVirtualCursorLost(uint)), infrared, SLOT(dbusVirtualCursorLost(uint)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteAcc(uint,const accdata&)), infrared, SLOT(dbusWiimoteAcc(uint,const accdata&)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteInfrared(uint, const QList< irpoint>&)), infrared, SLOT(dbusWiimoteInfrared(uint, const QList<struct irpoint>&)));
  QObject::connect(&interface, SIGNAL(dbusWiimoteDisconnected(uint)), infrared, SLOT(dbusWiimoteDisconnected(uint)));

  WiimoteRawStream *toolkit = new WiimoteRawStream(&interface, infrared);


  MainWindow window(toolkit);
  window.show();

  return application.exec();
}
