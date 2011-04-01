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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsScene>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtGui>

#include "widgets/graphicsprofileitem.h"
#include "widgets/graphicsprofilecover.h"
#include "widgets/graphicsmanagermenu.h"

#include "headers/consts.h"
#include "src/interfaces/customjobs.h"
#include "src/interfaces/deviceevents.h"
#include "src/interfaces/profilemanager.h"

#include <QTimeLine>
#include <QObject>
#include <QGraphicsItem>


class QGraphicsItemGroupPlus :public QObject, public QGraphicsItemGroup
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (qreal x READ x WRITE setX)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)

private:
  GraphicsProfileItem* lastItem;

public:
  QGraphicsItemGroupPlus() {
    lastItem = 0;
  }

protected:

  void hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    QGraphicsItem::hoverEnterEvent(event);
    QGraphicsItem* item = scene()->itemAt(event->scenePos().x(), event->scenePos().y());

    if (!item)
      return;

    if (item->parentItem() != dynamic_cast< QGraphicsItem *>(this))
      return;

    GraphicsProfileItem* obj = dynamic_cast< GraphicsProfileItem*>(item);

    if (obj) {
      if (lastItem)
        lastItem->hoverLeave();
      lastItem = obj;
      lastItem->hoverEnter();
    }

  }

  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    QGraphicsItem::hoverLeaveEvent(event);
      if (lastItem) {
        lastItem->hoverLeave();
        lastItem = 0;
      }

    }

};

class QGraphicsPixmapItemPlus :public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
  Q_PROPERTY (qreal x READ x WRITE setX)
  Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)

};


class MainWindow : public QGraphicsView {
  Q_OBJECT
public:
  MainWindow(DBusDeviceEventsInterface *device);
  ~MainWindow();

private slots:
  void showMenu(int duration);
  void hideMenu(int duration);


private:
  QRect calculateWindowSize();


  void confineCursor();
  void moveCursor();
  void scaleCursor(double scale);
  void rotateCursor(double angle);


  void setCoverIndex(int index);
  void nextCover();
  void prevCover();

private slots:
  void showCovers();
  void showProfiles();


protected:
  virtual void drawForeground(QPainter *painter, const QRectF &rect);
  virtual void drawBackground(QPainter *painter, const QRectF &rect);
  virtual void resizeEvent(QResizeEvent*);
  virtual void mouseMoveEvent (QMouseEvent*);
  virtual void mousePressEvent (QMouseEvent*);
  virtual void mouseReleaseEvent (QMouseEvent*);
  void timerEvent(QTimerEvent *event);

public slots:
  void dbusVirtualCursorPosition(quint32,double, double, double, double);
  void dbusWiimoteGeneralButtons(quint32,quint64);

  void setRumbleStatus(int status);

private:
  DBusDeviceEventsInterface *device;
  double x, y;
  double mouseX;
  double mouseY;
  double mouseAccX;
  double mouseAccY;

  QFont *font8;
  QFont *font16;
  QFont *font24;
  QFont *font32;
  QFont *font48;
  QFont *font64;

  bool visibleMenu;

  QPixmap *cursorPixmap;

  double cursorAngle;
  double cursorSize;


  QGraphicsPixmapItem *cursorHandle;

  QGraphicsItemGroupPlus menuGroup;
  QGraphicsItemGroupPlus coverGroup;
  QGraphicsItemGroupPlus profileGroup;

  QList < GraphicsProfileItem*> profiles;
  QList < GraphicsProfileCover*> covers;

  GraphicsProfileItem *ProfilesMenuItem;
  GraphicsProfileItem *ConnectionsMenuItem;
  GraphicsProfileItem *CoversMenuItem;
  GraphicsProfileItem *PreferencesMenuItem;

  GraphicsProfileItem *lastFocusedProfile;
  GraphicsProfileItem *lastActivedProfile;
  GraphicsProfileItem *lastFocusedMenu;
  GraphicsProfileItem *lastActivedMenu;
  GraphicsManagerMenu *menu;

  QGraphicsPixmapItemPlus *profileRunning;

  QPixmap *enabledPixmap;
  QPixmap *disabledPixmap;

  quint32 profileGroupHeight;

  QPixmap *styledBackground;

  quint64 lastButtons;
  qint32 currentCoverIndex;
  QTimeLine rumble;


};

#endif // MAINWINDOW_H
