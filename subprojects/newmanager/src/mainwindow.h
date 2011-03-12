/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsScene>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtGui>

#include "headers/consts.h"
#include "src/interfaces/customjobs.h"
#include "src/interfaces/deviceevents.h"
#include "src/interfaces/profilemanager.h"


class ProfileGraphicsItem :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

  Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)

public:
  ProfileGraphicsItem (quint32 width, quint32 height, QObject *parent = 0);
  QRectF boundingRect() const;

  void setFocus(bool);
  void setActive(bool);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  quint32 width;
  quint32 height;

  bool focused;
  bool actived;

  QPixmap *pixmap;
  int apos;
};

class GraphicsBarItem :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
public:
  GraphicsBarItem (quint32 width, quint32 height, quint32 location, QObject *parent = 0);
  QRectF boundingRect() const;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  quint32 width;
  quint32 height;
  quint32 location;
};

class MainWindow : public QGraphicsView {
  Q_OBJECT
public:
  MainWindow(DBusDeviceEventsInterface *device);
  ~MainWindow();

private:
  QRect calculateWindowSize();
  void confineCursor();

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

private:
  DBusDeviceEventsInterface *device;
  double x, y;
  double mouseX;
  double mouseY;
  double mouseAccX;
  double mouseAccY;

  QGraphicsPixmapItem *cursorHandle;
  QGraphicsItemGroup profileGroup;

  QList < ProfileGraphicsItem*> profiles;

  ProfileGraphicsItem *lastFocusedProfile;
  ProfileGraphicsItem *lastActivedProfile;

  quint32 profileGroupHeight;

  QPixmap *styledBackground;

  quint64 lastButtons;


};

#endif // MAINWINDOW_H
