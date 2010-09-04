/**********************************************************************************
 * Wiimotedev car example                                                         *
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

#include <QtGui/QApplication>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtDBus/QDBusConnection>

#include "car.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGraphicsScene scene;

    scene.setSceneRect(-500, -500, 1000, 1000);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    Car *car;

    car = new Car(1); // wiimote 1 car
    car->color = Qt::white; // white car for wiimote 1
    scene.addItem(car);

    car = new Car(2); // wiimote 2 car
    car->color = Qt::white; // gray car for wiimote 2
    car->setX(150);
    scene.addItem(car);

    car = new Car(3); // wiimote 3 car
    car->color = Qt::white; // gray car for wiimote 2
    car->setY(150);
    scene.addItem(car);

    car = new Car(4); // wiimote 4 car
    car->color = Qt::white; // gray car for wiimote 2
    car->setX(150);
    car->setY(150);
    scene.addItem(car);

    QGraphicsView view(&scene);
    view.setBackgroundBrush(Qt::darkGray);
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Wiimotedev Car Example"));
    view.resize(400, 300);
    view.show();

    return app.exec();
}
