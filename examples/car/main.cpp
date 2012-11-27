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
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDBusConnection>
#include <QMessageBox>

#include <QScopedPointer>
#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include "dbus/interfaces/deviceevents.h"
#include "car.h"

const quint32 count = 4;


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  DBusDeviceEventsInterface *interface = new DBusDeviceEventsInterface(WIIMOTEDEV_DBUS_SERVICE_NAME,
      WIIMOTEDEV_DBUS_OBJECT_EVENTS, QDBusConnection::systemBus(), 0);

  if(!interface->isValid()) {
    QMessageBox::critical(0, "Critical", "Unable to connect with wiimotedev-daemon service", QMessageBox::Ok);
    return EXIT_FAILURE;
  }

  QGraphicsScene *scene = new QGraphicsScene();

  scene->setSceneRect(-500, -500, 1000, 1000);
  scene->setItemIndexMethod(QGraphicsScene::NoIndex);


  for (register int i = 1; i <= count; ++i) {
    Car *item = new Car();
    item->setWiiremote(i);
    item->color = Qt::white;
    item->setX(100 * (i - 1));
    item->setY(100 * (i - 1));
    scene->addItem(item);
    QObject::connect(interface, SIGNAL(dbusWiimoteAcc(quint32,accdata)), item, SLOT(dbusWiimoteAcc(quint32,accdata)), Qt::QueuedConnection);
    QObject::connect(interface, SIGNAL(dbusWiimoteButtons(quint32,quint64)), item, SLOT(dbusWiimoteButtons(quint32,quint64)), Qt::QueuedConnection);
  }




  QGraphicsView view(scene);
  view.setRenderHints(QPainter::Antialiasing);
  view.setBackgroundBrush(Qt::darkGray);
  view.setCacheMode(QGraphicsView::CacheBackground);
  view.setWindowTitle("Wiimotedev Remote-Car example");
  view.resize(400, 300);
  view.show();

  return app.exec();


}
