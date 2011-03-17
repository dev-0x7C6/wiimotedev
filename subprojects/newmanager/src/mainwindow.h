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


class ProfileItem :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)

  quint32 width;
  quint32 height;
  QFont font;
  QString text;
  QPixmap icon;
  quint32 state;

  QColor focusColor;
  QColor activeColor;
  QColor inactiveColor;

  bool focused;
  bool actived;

public:
  ProfileItem (QObject *parent = 0);
  QRectF boundingRect() const { return QRectF(0, 0, width, height); }

  enum States {
    itemActive = 0,
    itemInactive,
    itemFocus
  };

  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }

  void setText(QString t) { text = t; }
  void setIcon(QPixmap i) { icon = i; apos = (height/2)-(icon.height()/2);}
  void setFont(QFont f) { font = f; }

  void setFocusColor(QColor c) { focusColor = c; }
  void setActiveColor(QColor c) { inactiveColor = c; }
  void setInactiveColor(QColor c) { inactiveColor = c; }

  void setActiveState(bool f) { actived = f; }
  void setFocusState(bool a) { focused = a; }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  int apos;
};

class ProfileCoverItem :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)

  quint32 width;
  quint32 height;
  QFont font;
  QString text;
  QPixmap cover;
  quint32 state;

  QColor focusColor;
  QColor activeColor;
  QColor inactiveColor;

  bool focused;
  bool actived;

public:
  ProfileCoverItem (QObject *parent = 0);
  QRectF boundingRect() const { return QRectF(0, 0, width, height); }

  enum States {
    itemActive = 0,
    itemInactive,
    itemFocus
  };

  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }

  void setText(QString t) { text = t; }
  void setFont(QFont f) { font = f; }
  void setCover(QPixmap c);

  void setFocusColor(QColor c) { focusColor = c; }
  void setActiveColor(QColor c) { inactiveColor = c; }
  void setInactiveColor(QColor c) { inactiveColor = c; }

  void setActiveState(bool f) { actived = f; }
  void setFocusState(bool a) { focused = a; }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  int apos;
};

class QGraphicsItemGroupPlus :public QObject, public QGraphicsItemGroup
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
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

  QFont *font8;
  QFont *font16;
  QFont *font24;
  QPixmap *cursorPixmap;

  double cursorAngle;
  double cursorSize;


  QGraphicsPixmapItem *cursorHandle;

  QGraphicsItemGroupPlus coverGroup;
  QGraphicsItemGroupPlus profileGroup;

  QList < ProfileItem*> profiles;

  ProfileItem *ProfilesMenuItem;
  ProfileItem *ConnectionsMenuItem;
  ProfileItem *PreferencesMenuItem;

  ProfileItem *lastFocusedProfile;
  ProfileItem *lastActivedProfile;

  QPixmap *enabledPixmap;
  QPixmap *disabledPixmap;

  quint32 profileGroupHeight;

  QPixmap *styledBackground;

  quint64 lastButtons;


};

#endif // MAINWINDOW_H
