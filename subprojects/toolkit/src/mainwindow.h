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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QMenu>

#include "src/interfaces/deviceevents.h"
#include "widgets/wiimoteanalogitem.h"
#include "widgets/wiimotebatteryitem.h"
#include "widgets/wiimoteleditem.h"
#include "widgets/wiimoterumbleitem.h"
#include "src/selectwiimote.h"

#include "headers/consts.h"

const double defaultPointMultiplier = 2.5;


class MainWindow : public QGraphicsView {
  Q_OBJECT
private:
  QString macAddress;

public:
  MainWindow(DBusDeviceEventsInterface *iface, quint32 id = 1, QWidget *parent = 0);
  ~MainWindow();

  enum Points {
    ir0source = 0,
    ir1source,
    ir2source,
    ir3source,
    ir4source
  };

  enum IROrder {
    LeftToRight,
    RightToLeft
  };

  enum AnalogMode {
    modeNone,
    modeClassic,
    modeNunchuk
  };

protected:
  virtual void resizeEvent(QResizeEvent*);

private:
  QGraphicsTextItem *accelerometrInfo;
  QGraphicsTextItem *analogInfo;
  QGraphicsTextItem *infraredInfo;
  QGraphicsTextItem *statusInfo;
  QGraphicsLineItem *line;
  QGraphicsPixmapItem *cursor;

  QGraphicsTextItem *wiimoteStdButtonText;
  QGraphicsTextItem *wiimoteExtButtonText;

  QGraphicsEllipseItem *infraredPoints[4];
  QGraphicsLineItem *infraredLine[4];
  WiimoteLedItem *ledPixmaps[4];
  WiimoteBatteryItem *batteryItem;
  WiimoteRumbleItem *rumbleItem;
  QTime timer;

  QMenu menu;

  quint32 order;

  double p;
  double stableAccRoll;

  double lineLength;
  double angleDiff;

  struct accdata nunchuk_acc;
  struct accdata wiimote_acc;

  QMap <quint64, QString> text_buttons_;

  quint32 wiimoteId;
  quint8 leds;

  SelectWiimote *selectWiimote;

  DBusDeviceEventsInterface *iface;
  QGraphicsScene scene;

  QTimer infraredTimeout;

  double widthMultiplier;
  double heightMultiplier;
  double dotSizeMultiplier;

  stickdata sticks[2];

  QString getReadableWiiremoteSequence(quint64);

  int analogMode;

private slots:
  void updateAccelerometrInfo(int, int, int, double, double,
                              int, int, int, double, double);
  void updateAnalogInfo();
  void updateButtonInfo(quint64);
  void updateInfraredInfo(QList < struct irpoint>);
  void updateStatusInfo();


private slots:
  void changeDevicePushed();
  void infraredCleanup();

  void toggleRumble(bool);
  void toggleLed1(bool);
  void toggleLed2(bool);
  void toggleLed3(bool);
  void toggleLed4(bool);

  void dbusVirtualCursorPosition(quint32 id, double x, double y, double size, double angle);
  void dbusVirtualCursorLost(quint32 id);
  void dbusVirtualCursorFound(quint32 id);

  void dbusWiimoteAcc(quint32 id, const accdata &acc);
  void dbusWiimoteBatteryLife(quint32 id, quint8 life);
  void dbusWiimoteConnected(quint32 id);
  void dbusWiimoteDisconnected(quint32 id);
  void dbusWiimoteGeneralButtons(quint32 id, quint64 value);
  void dbusWiimoteInfrared(quint32 id, const QList< struct irpoint> &points);
  void dbusWiimoteLedStatusChanged(quint32, quint8);
  void dbusWiimoteRumbleStatusChanged(quint32, quint8);

  void dbusNunchukPlugged(quint32 id);
  void dbusNunchukUnplugged(quint32 id);
  void dbusClassicPlugged(quint32 id);
  void dbusClassicUnplugged(quint32 id);

  void dbusNunchukAcc(quint32 id, const accdata &acc);
  void dbusNunchukStick(quint32 id, const stickdata &stick);

  void dbusClassicControllerLStick(quint32 id, const stickdata &stick);
  void dbusClassicControllerRStick(quint32 id, const stickdata &stick);
};

#endif // MAINWINDOW_H
