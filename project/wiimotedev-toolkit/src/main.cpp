/**********************************************************************************
 * Wiimotedev toolkit                                                             *
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
#include <wiimotedev/consts.h>

#include "mainwindow.h"
#include "interface.h"

int main(int argc, char *argv[])
{  
    QApplication application(argc, argv);

    DBusDeviceEventsInterface *iface = new DBusDeviceEventsInterface(WIIMOTEDEV_DBUS_SERVICE_NAME,
                                                                     WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                                                                     QDBusConnection::systemBus(),
                                                                     &application);
    if (!iface->isValid()) {
        QMessageBox::critical(application.activeWindow(), QString("Critical"), QString("Wiimotedev-daemon: service is not available"));
        return 0;
    }

    delete iface;

    MainWindow window;
    window.show();

    return application.exec();
}
