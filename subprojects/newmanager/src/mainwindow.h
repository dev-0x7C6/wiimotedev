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

#include "widgets/graphicsbutton.h"
#include "widgets/graphicscheckbox.h"
#include "widgets/graphicsprofilecover.h"
#include "widgets/graphicsmanagermenu.h"

#include "headers/consts.h"
#include "src/interfaces/customjobs.h"
#include "src/interfaces/deviceevents.h"
#include "src/interfaces/profilemanager.h"

#include <QTimeLine>
#include <QObject>
#include <QGraphicsItem>

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

  void loadProfiles();


  void confineCursor();
  void moveCursor();
  void scaleCursor(double scale);
  void rotateCursor(double angle);


  void setCoverIndex(int index);
  void nextCover();
  void prevCover();

private slots:
  void profileButtonClicked();
  void coverButtonClicked();
  void preferenceButtonClicked();
  void connectionButtonClicked();

  void showCovers();
  void showProfiles();

  void showProfilesPage();
  void hideProfilesPage();
  void showCoversPage();
  void hideCoversPage();
  void showPreferencesPage();
  void hidePreferencesPage();
  void showConnectionsPage();
  void hideConnectionsPage();

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
// General
  QFont *font64;
  QFont *font48;
  QFont *font32;
  QFont *font24;
  QFont *font16;
  QFont *font8;

// Menu componets
  GraphicsButton *profileButton;
  GraphicsButton *coverButton;
  GraphicsButton *preferencesButton;
  GraphicsButton *connectionButton;
  GraphicsManagerMenu *menuBackground;

  QList < QGraphicsItem*> menuComponents;

// Profiles page
  QList < GraphicsButton*> profiles;

// Preferences page
  GraphicsCheckbox *prefAutoHideMenu;
  GraphicsCheckbox *prefDisplayBatteryStatus;
  GraphicsCheckbox *prefDisplayServiceStatus;

// Covers page
  QList < GraphicsProfileCover*> covers;

// Connections page
  QList < GraphicsButton*> connections;


  DBusDeviceEventsInterface *device;
  double x, y;
  double mouseX;
  double mouseY;
  double mouseAccX;
  double mouseAccY;


  bool visibleMenu;

  QPixmap *cursorPixmap;

  double cursorAngle;
  double cursorSize;


  QGraphicsPixmapItem *cursorHandle;





  GraphicsButton *lastFocusedProfile;
  GraphicsButton *lastActivedProfile;
  GraphicsButton *lastFocusedMenu;
  GraphicsButton *lastActivedMenu;

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
