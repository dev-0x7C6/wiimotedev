/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2015  Bartłomiej Burdukiewicz                               *
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

#include <QMessageBox>
#include <QDBusReply>

#include "infraredcameraview.h"
#include "linux/usr/include/wiimotedev/deviceevents.h"

#include <QtCore/qmath.h>

#define PI M_PI

InfraredCameraView::InfraredCameraView(WiimotedevDeviceEvents *iface, uint id, QWidget *parent) :
  QGraphicsView(parent),
  cursor(new QGraphicsPixmapItem(QPixmap(":/cursor.png"))),
  iface(iface),
  wiimoteId(id) {
  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  widthMultiplier = 0.5;
  heightMultiplier = 0.5;
  dotSizeMultiplier = 4;
  connect(&infraredTimeout, SIGNAL(timeout()), this, SLOT(infraredCleanup()));
  infraredTimeout.setInterval(30);
  scene.setBackgroundBrush(QBrush(QColor(0x00, 0x00, 0x00, 0xFF), Qt::SolidPattern));
  scene.setSceneRect(0, 0, geometry().width(), geometry().height());
  setScene(&scene);
  cursor->setZValue(100);
  cursor->setTransformOriginPoint(7, 2);
  cursor->setTransformationMode(Qt::SmoothTransformation);
  cursor->setVisible(false);
  scene.addItem(cursor);
  line = new QGraphicsLineItem();
  line->setZValue(-1);

  for (int i = 0; i < 4; ++i) {
    infraredPoints[i] = new QGraphicsEllipseItem();
    infraredPoints[i]->setPen(QPen(Qt::white));
    infraredPoints[i]->setBrush(QBrush(Qt::black));
    infraredPoints[i]->setRect(0, 0, 3, 3);
    infraredPoints[i]->hide();
    infraredPoints[i]->setZValue(10.0);
    infraredLine[i] = new QGraphicsLineItem();
    infraredLine[i]->setPen(QPen(Qt::yellow));
    infraredLine[i]->hide();
    infraredLine[i]->setZValue(5.0);
    scene.addItem(infraredPoints[i]);
    scene.addItem(infraredLine[i]);
  }

  line->setPen(QPen(QColor::fromRgb(0x40, 0x40, 0x40)));
  scene.addItem(line);
}


void InfraredCameraView::dbusWiimoteConnected(uint id) {
  if (wiimoteId != id)
    return;
}

void InfraredCameraView::dbusWiimoteDisconnected(uint id) {
  if (wiimoteId != id)
    return;

  dbusVirtualCursorLost(id);
  dbusWiimoteInfrared(id, QList <irpoint>());
}

void InfraredCameraView::setWiimoteId(uint id) {
  wiimoteId = id;
}

void InfraredCameraView::resizeEvent(QResizeEvent *event) {
  QRect rect = geometry();
  setSceneRect(0, 0, rect.width(), rect.height());
  line->setLine(-(geometry().width() / 10), geometry().height() / 2, geometry().width() + (geometry().width() / 10), geometry().height() / 2);
  line->setTransformOriginPoint(geometry().width() / 2, geometry().height() / 2);
}

InfraredCameraView::~InfraredCameraView() {
}

void InfraredCameraView::infraredCleanup() {
  infraredTimeout.stop();

  for (int i = 0; i < 4; ++i)
    infraredPoints[i]->hide();
}

void InfraredCameraView::dbusWiimoteInfrared(uint id, const QList<irpoint> &points) {
  if (id != wiimoteId)
    return;

  if (sceneRect().width() != width() ||
      sceneRect().height() != height())
    setSceneRect(0, 0, width(), height());

  widthMultiplier = static_cast<double>(width()) / 1024.0;
  heightMultiplier = static_cast<double>(height()) / 768.0;
  dotSizeMultiplier = (widthMultiplier + heightMultiplier) / 2.0;
  infraredTimeout.start();

  for (int i = 0; i < 4; ++i) {
    if (i < points.count()) {
      register int size = points.at(i).size * 2;
      infraredPoints[i]->setRect(-size / 2, -size / 2, size, size);

      if (!infraredPoints[i]->isVisible())
        infraredPoints[i]->show();

      infraredPoints[i]->setPos((static_cast<double>(points.at(i).x) * widthMultiplier) - (size / 2),
                                (static_cast<double>(points.at(i).y) * heightMultiplier) - (size / 2));
    } else {
      if (infraredPoints[i]->isVisible())
        infraredPoints[i]->hide();
    }
  }

  switch (points.count()) {
    case ir0source:
    case ir1source:
      for (int i = 0; i < 4; ++i)
        if (infraredLine[i]->isVisible()) infraredLine[i]->hide();

      break;

    case ir2source:
      for (int i = 1; i < 4; ++i)
        if (infraredLine[i]->isVisible()) infraredLine[i]->hide();

      infraredLine[0]->setLine(infraredPoints[0]->x(), infraredPoints[0]->y(), infraredPoints[1]->x(), infraredPoints[1]->y());

      if (!infraredLine[0]->isVisible()) infraredLine[0]->show();

      break;

    case ir3source:
      if (infraredLine[3]->isVisible()) infraredLine[3]->hide();

      infraredLine[0]->setLine(infraredPoints[0]->x(), infraredPoints[0]->y(), infraredPoints[1]->x(), infraredPoints[1]->y());
      infraredLine[1]->setLine(infraredPoints[1]->x(), infraredPoints[1]->y(), infraredPoints[2]->x(), infraredPoints[2]->y());
      infraredLine[2]->setLine(infraredPoints[2]->x(), infraredPoints[2]->y(), infraredPoints[0]->x(), infraredPoints[0]->y());

      if (!infraredLine[0]->isVisible()) infraredLine[0]->show();

      if (!infraredLine[1]->isVisible()) infraredLine[1]->show();

      if (!infraredLine[2]->isVisible()) infraredLine[2]->show();

      break;

    case ir4source:
      infraredLine[0]->setLine(infraredPoints[0]->x(), infraredPoints[0]->y(), infraredPoints[1]->x(), infraredPoints[1]->y());
      infraredLine[1]->setLine(infraredPoints[1]->x(), infraredPoints[1]->y(), infraredPoints[2]->x(), infraredPoints[2]->y());
      infraredLine[2]->setLine(infraredPoints[2]->x(), infraredPoints[2]->y(), infraredPoints[3]->x(), infraredPoints[3]->y());
      infraredLine[3]->setLine(infraredPoints[3]->x(), infraredPoints[3]->y(), infraredPoints[0]->x(), infraredPoints[0]->y());

      if (!infraredLine[0]->isVisible()) infraredLine[0]->show();

      if (!infraredLine[1]->isVisible()) infraredLine[1]->show();

      if (!infraredLine[2]->isVisible()) infraredLine[2]->show();

      if (!infraredLine[3]->isVisible()) infraredLine[3]->show();
  }
}

void InfraredCameraView::dbusVirtualCursorFound(uint id) {
  if (id != wiimoteId)
    return;

  cursor->setVisible(true);
}

void InfraredCameraView::dbusVirtualCursorLost(uint id) {
  if (id != wiimoteId)
    return;

  cursor->setVisible(false);
}

void InfraredCameraView::dbusVirtualCursorPosition(uint id, double x, double y, double size, double angle) {
  if (id != wiimoteId)
    return;

  if (!cursor->isVisible())
    cursor->setVisible(true);

  cursor->setX((512.0 - x) * (geometry().width() / 1024.0));
  cursor->setY((384.0 - y) * (geometry().height() / 768.0));
  cursor->setScale((1024 - size) / 500);
  cursor->setRotation(-angle * 180 / M_PI);
}

void InfraredCameraView::dbusWiimoteAcc(uint id, const accdata &table) {
  if (id != wiimoteId)
    return;

  double roll = -table.roll;

  if (roll < 0)
    roll = 360 - table.roll;

  line->setRotation(-roll);
}




