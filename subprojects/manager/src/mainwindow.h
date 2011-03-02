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

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QMenu>
#include <QPainter>
#include <QTimer>

#include "headers/consts.h"
#include "src/interfaces/customjobs.h"
#include "src/interfaces/deviceevents.h"
#include "src/interfaces/profilemanager.h"

#include "widgets/devicewidget.h"
#include "widgets/profilewidget.h"
#include "ui_devicewidget.h"
#include <QVBoxLayout>
#include <QSpacerItem>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSettings>
#include <QListWidgetItem>

namespace Ui {
    class MainWindow;
}

class MainWindow :public QMainWindow {
  Q_OBJECT
private:
  QSystemTrayIcon *tray;
  QMenu *menu;
  QAction *menuExitAction;
  QPixmap *logo;


  DBusDeviceEventsInterface *daemon;

  qreal logoOpacity;

  double x, y;
  double mouseX;
  double mouseY;
  double mouseAccX;
  double mouseAccY;

  QTimer scrollTimer;

  QVBoxLayout *deviceVerticalLayout;
  QVBoxLayout *profileVerticalLayout;
  QHash < quint32, DeviceWidget*> deviceWidgets;
  QHash < quint32, QString> storeMacAddresses;

// Window
  Ui::MainWindow *ui;
  QSettings *globalSettings;
  QString profileFile;

  ProfileWidget *currentProfile;

  QMap <QString, int> pageMap;

public:
  MainWindow(DBusDeviceEventsInterface *daemon, QWidget *parent = 0);
  ~MainWindow();

protected:
  virtual void paintEvent(QPaintEvent*);
  void timerEvent(QTimerEvent *event);

public slots:
  void dbusVirtualCursorPosition(quint32, double, double, double, double);
  void dbusWiimoteGeneralButtons(quint32 id, quint64 value);
  void dbusWiimoteConnected(quint32 id);
  void dbusWiimoteDisconnected(quint32 id);
  void slotMousePosition();
  void scroll();
  void loadProfile(ProfileWidget*);

  void selectPage(QListWidgetItem*);

  void executeRequest(QStringList list);

  void setTrayTooltip();

private:
  bool windowAtMousePosition;

  QTimer checkMousePos;

  QRect calcGeometry();

private:

  QRect defaultGeometry;

  QMap< QString, quint64> devicebuttons;

  bool connectedWithService;

  quint64 buttons;
  quint64 lastButtons;
  quint32 index;

  DBusCustomJobsInterface *customJobsInterface;
  DBusProfileManagerInterface *profileInterface;

  void showProfile(struct ProfileItem item);
  void moveToCenter();

};

#endif // MAINWINDOW_H
