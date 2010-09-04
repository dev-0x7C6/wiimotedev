/**********************************************************************************
 * Wiimotedev toolkit                                                             *
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
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>

#include "ui/selectwiimote.h"

#include "interface.h"
#include "wiimotedev.h"

const double defaultPointMultiplier = 2.5;

namespace Ui {
    class MainWindow;
}


class DotItem : public QGraphicsItem
{
public:

    bool visible;
    double size;

    DotItem()
    {
        visible = false;
        size = 0;
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsFocusable, true);
    }

    QRectF boundingRect() const
    {
        qreal point = size*defaultPointMultiplier*1.5;
        return QRectF(-point,-point,point,point);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        if (visible) {
            painter->setBrush(Qt::white);
            painter->setPen(Qt::white);
            qreal point = size*defaultPointMultiplier;
            painter->drawEllipse(-point,-point,point,point);
        }
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    quint32 wiimoteId;
    quint8 leds;

    SelectWiimote *selectWiimote;

    struct accdata wiimoteAccTable;
    struct accdata nunchukAccTable;

    DotItem *classicLStickDot;
    DotItem *classicRStickDot;
    DotItem *nunchukStickDot;

    Ui::MainWindow *ui;
    DBusDeviceEventsInterface  *iface;
    QGraphicsScene *scene;
    DotItem *dotItems[4];

    QTimer infraredTimeout;

    double widthMultiplier;
    double heightMultiplier;
    double dotSizeMultiplier;

private slots:
    void changeDevicePushed();
    void getWiimoteStats();

    void toggleRumble(bool rumble);
    void toggleLed1(bool toggled);
    void toggleLed2(bool toggled);
    void toggleLed3(bool toggled);
    void toggleLed4(bool toggled);

    void dbusNunchukPlugged(quint32 id);
    void dbusNunchukUnplugged(quint32 id);
  //  void dbusWiimoteGeneralButtons(quint32 id, quint64 value);

  //  void dbusWiimoteBatteryLife(quint32 id, quint8 life);
    void dbusWiimoteButtons(quint32 id, quint64 value);
  //  void dbusWiimoteStatus(quint32 id, quint8 status);
    void dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points);
    void dbusWiimoteAcc(quint32 id, struct accdata acc);

    void dbusNunchukButtons(quint32 id, quint64 value);
    void dbusNunchukStick(quint32 id, struct stickdata stick);
    void dbusNunchukAcc(quint32 id, struct accdata acc);

   // void dbusClassicControllerButtons(quint32 id, quint64 value);
    void dbusClassicControllerLStick(quint32 id, struct stickdata stick);
    void dbusClassicControllerRStick(quint32 id, struct stickdata stick);

    void infraredCleanup();
};

#endif // MAINWINDOW_H
