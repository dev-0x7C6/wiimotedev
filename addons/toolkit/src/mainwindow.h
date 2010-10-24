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
#include <QPainter>

#include "widgets/wiimotebatteryitem.h"
#include "widgets/wiimoteleditem.h"
#include "src/selectwiimote.h"

#include "src/interfaces/deviceevents.h"
#include "headers/consts.h"

const double defaultPointMultiplier = 2.5;


class MainWindow : public QGraphicsView {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

  enum Points {
    ir0source = 0,
    ir1source,
    ir2source,
    ir3source,
    ir4source
  };

protected:
  virtual void resizeEvent(QResizeEvent*);

private:
  QGraphicsItemGroup *infraredGroup;
  QGraphicsTextItem *infraredTitle;
  QGraphicsTextItem *infraredPointsText[4];

  QGraphicsTextItem *accelerometrTitle;
  QGraphicsItemGroup *accelerometrGroup;
  QGraphicsTextItem *accelerometrPointsText[10];

  QGraphicsLineItem *line;

  QGraphicsTextItem *wiimoteStdButtonText;
  QGraphicsTextItem *wiimoteExtButtonText;

  QGraphicsEllipseItem *infraredPoints[4];
  QGraphicsLineItem *infraredLine[4];
  WiimoteLedItem *ledPixmaps[4];
  WiimoteBatteryItem *batteryItem;

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

  QString getReadableWiiremoteSequence(quint64);

private slots:
  void updateAccelerometrInfo(int, int, int, double, double,
                              int, int, int, double, double);
  void updateButtonInfo(quint64);
  void updateInfraredInfo(QList < struct irpoint>);


private slots:
  void changeDevicePushed();
  void infraredCleanup();
  void getWiimoteStats();

  void toggleRumble(bool);
  void toggleLed1(bool);
  void toggleLed2(bool);
  void toggleLed3(bool);
  void toggleLed4(bool);

  void dbusWiimoteAcc(quint32 id, struct accdata acc);
  void dbusWiimoteBatteryLife(quint32 id, quint8 life);
  void dbusWiimoteGeneralButtons(quint32 id, quint64 value);
  void dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points);

  void dbusNunchukPlugged(quint32 id);
  void dbusNunchukUnplugged(quint32 id);

  void dbusNunchukAcc(quint32 id, struct accdata acc);
  void dbusNunchukStick(quint32 id, struct stickdata stick);

  void dbusClassicControllerLStick(quint32 id, struct stickdata stick);
  void dbusClassicControllerRStick(quint32 id, struct stickdata stick);
};

#endif // MAINWINDOW_H
