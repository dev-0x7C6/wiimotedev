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

#include <QMessageBox>
#include <QDBusReply>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(&infraredTimeout, SIGNAL(timeout()), this, SLOT(infraredCleanup()));
    infraredTimeout.setInterval(30);

    wiimoteId = 1;
    widthMultiplier = 0.5;
    heightMultiplier = 0.5;
    dotSizeMultiplier = 4;

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");

    ui->setupUi(this);

    scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(QColor(0x0A, 0x0A, 0x0A, 0xFF), Qt::SolidPattern));

    scene->setSceneRect(0, 0, 512, 384);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);


    infraredTitle = new QGraphicsTextItem();
    infraredTitle->setHtml("<font color=\"#aaaaaa\"> Infrared Informations</font>");
    infraredTitle->setPos(0, 0);

    infraredPointsText[0] = new QGraphicsTextItem();
    infraredPointsText[1] = new QGraphicsTextItem();
    infraredPointsText[2] = new QGraphicsTextItem();
    infraredPointsText[3] = new QGraphicsTextItem();
    infraredPointsText[0]->setY(25);
    infraredPointsText[1]->setY(40);
    infraredPointsText[2]->setY(55);
    infraredPointsText[3]->setY(70);

    updateInfraredInfo(QList < irpoint>() );


    scene->addItem(infraredTitle);
    scene->addItem(infraredPointsText[0]);
    scene->addItem(infraredPointsText[1]);
    scene->addItem(infraredPointsText[2]);
    scene->addItem(infraredPointsText[3]);

//    irInfo = new InfraredInfo;
//    scene->addItem(irInfo);
//    irInfo->setPos(100, 100);

    scene->addItem(dotItems[0] = new DotItem);
    scene->addItem(dotItems[1] = new DotItem);
    scene->addItem(dotItems[2] = new DotItem);
    scene->addItem(dotItems[3] = new DotItem);


    scene = new QGraphicsScene();
    scene->setSceneRect(-41, -41, 82, 82);
    scene->addItem(nunchukStickDot = new DotItem);
    nunchukStickDot->setX(0);
    nunchukStickDot->setY(0);
    nunchukStickDot->size = 1;
    nunchukStickDot->visible = true;
    ui->nunchukStickView->setScene(scene);

    scene = new QGraphicsScene();
    scene->setSceneRect(-41, -41, 82, 82);
    scene->addItem(classicLStickDot = new DotItem);
    classicLStickDot->setX(0);
    classicLStickDot->setY(0);
    classicLStickDot->size = 1;
    classicLStickDot->visible = true;
    ui->classicLStickView->setScene(scene);

    scene = new QGraphicsScene();
    scene->setSceneRect(-41, -41, 82, 82);
    scene->addItem(classicRStickDot = new DotItem);
    classicRStickDot->setX(0);
    classicRStickDot->setY(0);
    classicRStickDot->size = 1;
    classicRStickDot->visible = true;
    ui->classicRStickView->setScene(scene);


    iface = new DBusDeviceEventsInterface(WIIMOTEDEV_DBUS_SERVICE_NAME,
                                          WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                                          QDBusConnection::systemBus(),
                                          this);

    // this->setWindowState(Qt::WindowFullScreen);


    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));


    connect(iface, SIGNAL(dbusNunchukPlugged(quint32)), this, SLOT(dbusNunchukPlugged(quint32)));
    connect(iface, SIGNAL(dbusNunchukUnplugged(quint32)), this, SLOT(dbusNunchukUnplugged(quint32)));

    connect(iface, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SLOT(dbusWiimoteButtons(quint32,quint64)));
    connect(iface, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SLOT(dbusNunchukButtons(quint32,quint64)));

    connect(iface, SIGNAL(dbusNunchukStick(quint32,stickdata)), this, SLOT(dbusNunchukStick(quint32,stickdata)));
    connect(iface, SIGNAL(dbusClassicControllerRStick(quint32,stickdata)), this, SLOT(dbusClassicControllerRStick(quint32,stickdata)));
    connect(iface, SIGNAL(dbusClassicControllerLStick(quint32,stickdata)), this, SLOT(dbusClassicControllerLStick(quint32,stickdata)));

    connect(iface, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList<struct irpoint>)));
    connect(iface, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), this, SLOT(dbusWiimoteAcc(quint32,struct accdata)));
    connect(iface, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), this, SLOT(dbusNunchukAcc(quint32,accdata)));

    connect(ui->checkboxRumble, SIGNAL(toggled(bool)), this, SLOT(toggleRumble(bool)));
    connect(ui->checkboxWiimoteLed1, SIGNAL(toggled(bool)), this, SLOT(toggleLed1(bool)));
    connect(ui->checkboxWiimoteLed2, SIGNAL(toggled(bool)), this, SLOT(toggleLed2(bool)));
    connect(ui->checkboxWiimoteLed3, SIGNAL(toggled(bool)), this, SLOT(toggleLed3(bool)));
    connect(ui->checkboxWiimoteLed4, SIGNAL(toggled(bool)), this, SLOT(toggleLed4(bool)));


    connect(ui->actionChange_device, SIGNAL(triggered()), this, SLOT(changeDevicePushed()));

    QList <uint> list = iface->dbusGetWiimoteList();

    if (list.count() == 1)
        wiimoteId = list.at(0);

    if (list.count() > 1) {
        selectWiimote = new SelectWiimote(this);
        selectWiimote->setWiimoteList(list);
        selectWiimote->show();
        selectWiimote->exec();
        wiimoteId = selectWiimote->getSelectedWiimote();
        if (!wiimoteId)
            wiimoteId = list.at(0);
        delete selectWiimote;
    }

    getWiimoteStats() ;
}

void MainWindow::updateInfraredInfo(QList < irpoint> list)
{
  int i = 0;
  for (; i < list.count(); ++i)
    infraredPointsText[i]->setHtml(QString("<font color=\"#555555\">%1: </font>" \
      "<font color=\"#ffffff\">%2</font><font color=\"#999999\">x</font>" \
      "<font color=\"#ffffff\">%3</font><font color=\"#555555\"> size: </font>" \
      "<font color=\"#ffffff\">%4</font>").arg(QString::number(i), QString::number(list.at(i).x)
                                              , QString::number(list.at(i).y), QString::number(list.at(i).size)));
  for (; i < 4; ++i)
    infraredPointsText[i]->setHtml(QString("<font color=\"#555555\">%1: </font>" \
      "<font color=\"#ffffff\">%2</font><font color=\"#999999\">x</font>" \
      "<font color=\"#ffffff\">%3</font><font color=\"#555555\"> size: </font>" \
      "<font color=\"#ffffff\">%4</font>").arg(QString::number(i), "0", "0", "-1"));

}


MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::toggleRumble(bool rumble)
{
    iface->dbusWiimoteSetRumbleStatus(wiimoteId, rumble);
}

void MainWindow::changeDevicePushed()
{
    QList <uint> list = iface->dbusGetWiimoteList();

    selectWiimote = new SelectWiimote(this);
    selectWiimote->setWiimoteList(list);
    selectWiimote->show();
    selectWiimote->exec();
    if (selectWiimote->getSelectedWiimote())
        wiimoteId = selectWiimote->getSelectedWiimote();
    delete selectWiimote;

    getWiimoteStats();
}

void MainWindow::getWiimoteStats()
{
    leds = iface->dbusWiimoteGetLedStatus(wiimoteId);
    ui->checkboxRumble->setChecked(iface->dbusWiimoteGetRumbleStatus(wiimoteId));
    ui->progressWiimoteBattery->setValue(iface->dbusWiimoteGetBatteryLife(wiimoteId));
    ui->checkboxNunchukConnected->setChecked(iface->dbusIsNunchukConnected(wiimoteId));

    ui->checkboxWiimoteLed1->setChecked(leds & 1);
    ui->checkboxWiimoteLed2->setChecked(leds & 2);
    ui->checkboxWiimoteLed3->setChecked(leds & 4);
    ui->checkboxWiimoteLed4->setChecked(leds & 8);
}

void MainWindow::toggleLed1(bool toggled)
{
    leds &= 0xE;
    leds |= (toggled << 0);
    iface->dbusWiimoteSetLedStatus(wiimoteId, leds);
}

void MainWindow::toggleLed2(bool toggled)
{
    leds &= 0xD;
    leds |= (toggled << 1);
    iface->dbusWiimoteSetLedStatus(wiimoteId, leds);
}

void MainWindow::toggleLed3(bool toggled)
{
    leds &= 0xB;
    leds |= (toggled << 2);
    iface->dbusWiimoteSetLedStatus(wiimoteId, leds);
}

void MainWindow::toggleLed4(bool toggled)
{
    leds &= 0x7;
    leds |= (toggled << 3);
    iface->dbusWiimoteSetLedStatus(wiimoteId, leds);
}

void MainWindow::dbusNunchukStick(quint32 id, struct stickdata stick)
{
    if (id != wiimoteId)
        return;

    register int x = ((0xFF >> 1) - stick.x) * 0.25;
    register int y = ((0xFF >> 1) - stick.y) * 0.25;
    nunchukStickDot->setPos(-x, y);
}

void MainWindow::dbusClassicControllerLStick(quint32 id, struct stickdata stick)
{
    if (id != wiimoteId)
        return;

    register int x = ((0x3F >> 1) - stick.x) * 1.2;
    register int y = ((0x3F >> 1) - stick.y) * 1.2;
    classicLStickDot->setPos(-x, y);
}

void MainWindow::dbusClassicControllerRStick(quint32 id, struct stickdata stick)
{
    if (id != wiimoteId)
        return;

    register int x = ((0x1F >> 1) - stick.x) * 2.3;
    register int y = ((0x1F >> 1) - stick.y) * 2.3;
    classicRStickDot->setPos(-x, y);
}

void MainWindow::infraredCleanup()
{
    infraredTimeout.stop();

    for (register int i = 0; i < 4; ++i) {
        dotItems[i]->visible = false;
        dotItems[i]->size = 0;
        dotItems[i]->setPos(-100, -100);
    }
}

void MainWindow::dbusNunchukPlugged(quint32 id)
{
    if (id == wiimoteId)
        ui->checkboxNunchukConnected->setChecked(true);
}

void MainWindow::dbusNunchukUnplugged(quint32 id)
{
    if (id == wiimoteId)
        ui->checkboxNunchukConnected->setChecked(false);
}

void MainWindow::dbusWiimoteInfrared(quint32 id, QList<irpoint> points)
{
    if (id != wiimoteId)
        return;

    updateInfraredInfo(points);

    if (ui->graphicsView->sceneRect().width() != ui->graphicsView->width() ||
        ui->graphicsView->sceneRect().height() != ui->graphicsView->height())
        ui->graphicsView->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    widthMultiplier = static_cast< double> (ui->graphicsView->width()) / 1024.0;
    heightMultiplier = static_cast< double> (ui->graphicsView->height()) / 768.0;
    dotSizeMultiplier = (widthMultiplier + heightMultiplier) / 2.0;

    infraredTimeout.start();
    for (register int i = 0; i < 4; ++i) {
        if (i < points.count()) {
            dotItems[i]->visible = (points.at(i).size != -1);
            dotItems[i]->size = dotSizeMultiplier * points.at(i).size;
            dotItems[i]->setPos(static_cast< double>( points.at(i).x) * widthMultiplier, static_cast< double>( points.at(i).y) * heightMultiplier);
        } else {
            dotItems[i]->visible = false;
            dotItems[i]->size = 0;
            dotItems[i]->setPos(-100, -100);
        }
    }
}

void MainWindow::dbusWiimoteButtons(quint32 id, quint64 value)
{
    ui->checkboxWiimoteDigitalUp->setChecked(value & WIIMOTE_BTN_UP);
    ui->checkboxWiimoteDigitalDown->setChecked(value & WIIMOTE_BTN_DOWN);
    ui->checkboxWiimoteDigitalLeft->setChecked(value & WIIMOTE_BTN_LEFT);
    ui->checkboxWiimoteDigitalRight->setChecked(value & WIIMOTE_BTN_RIGHT);

    ui->checkboxWiimote1->setChecked(value & WIIMOTE_BTN_1);
    ui->checkboxWiimote2->setChecked(value & WIIMOTE_BTN_2);
    ui->checkboxWiimoteA->setChecked(value & WIIMOTE_BTN_A);
    ui->checkboxWiimoteB->setChecked(value & WIIMOTE_BTN_B);
    ui->checkboxWiimoteMinus->setChecked(value & WIIMOTE_BTN_MINUS);
    ui->checkboxWiimoteHome->setChecked(value & WIIMOTE_BTN_HOME);
    ui->checkboxWiimotePlus->setChecked(value & WIIMOTE_BTN_PLUS);

    ui->checkboxWiimoteTiltFront->setChecked(value & WIIMOTE_BTN_TILT_FRONT);
    ui->checkboxWiimoteTiltBack->setChecked(value & WIIMOTE_BTN_TILT_BACK);
    ui->checkboxWiimoteTiltLeft->setChecked(value & WIIMOTE_BTN_TILT_LEFT);
    ui->checkboxWiimoteTiltRight->setChecked(value & WIIMOTE_BTN_TILT_RIGHT);

    ui->checkboxWiimoteShiftForward->setChecked(value & WIIMOTE_BTN_SHIFT_FORWARD);
    ui->checkboxWiimoteShiftBackward->setChecked(value & WIIMOTE_BTN_SHIFT_BACKWARD);
    ui->checkboxWiimoteShiftLeft->setChecked(value & WIIMOTE_BTN_SHIFT_LEFT);
    ui->checkboxWiimoteShiftRight->setChecked(value & WIIMOTE_BTN_SHIFT_RIGHT);
    ui->checkboxWiimoteShiftUp->setChecked(value & WIIMOTE_BTN_SHIFT_UP);
    ui->checkboxWiimoteShiftDown->setChecked(value & WIIMOTE_BTN_SHIFT_DOWN);
}

void MainWindow::dbusNunchukButtons(quint32 id, quint64 value)
{
    ui->checkboxNunchukAnalogUp->setChecked(value & NUNCHUK_BTN_STICK_UP);
    ui->checkboxNunchukAnalogDown->setChecked(value & NUNCHUK_BTN_STICK_DOWN);
    ui->checkboxNunchukAnalogLeft->setChecked(value & NUNCHUK_BTN_STICK_LEFT);
    ui->checkboxNunchukAnalogRight->setChecked(value & NUNCHUK_BTN_STICK_RIGHT);

    ui->checkboxNunchukC->setChecked(value & NUNCHUK_BTN_C);
    ui->checkboxNunchukZ->setChecked(value & NUNCHUK_BTN_Z);

    ui->checkboxNunchukTiltFront->setChecked(value & NUNCHUK_BTN_TILT_FRONT);
    ui->checkboxNunchukTiltBack->setChecked(value & NUNCHUK_BTN_TILT_BACK);
    ui->checkboxNunchukTiltLeft->setChecked(value & NUNCHUK_BTN_TILT_LEFT);
    ui->checkboxNunchukTiltRight->setChecked(value & NUNCHUK_BTN_TILT_RIGHT);

    ui->checkboxNunchukShiftForward->setChecked(value & NUNCHUK_BTN_SHIFT_FORWARD);
    ui->checkboxNunchukShiftBackward->setChecked(value & NUNCHUK_BTN_SHIFT_BACKWARD);
    ui->checkboxNunchukShiftLeft->setChecked(value & NUNCHUK_BTN_SHIFT_LEFT);
    ui->checkboxNunchukShiftRight->setChecked(value & NUNCHUK_BTN_SHIFT_RIGHT);
    ui->checkboxNunchukShiftUp->setChecked(value & NUNCHUK_BTN_SHIFT_UP);
    ui->checkboxNunchukShiftDown->setChecked(value & NUNCHUK_BTN_SHIFT_DOWN);
}

void MainWindow::dbusWiimoteAcc(quint32 id, struct accdata table)
{
    if (id != wiimoteId)
        return;

    wiimoteAccTable = table;
    ui->progressWiimoteXAxis->setValue(wiimoteAccTable.x);
    ui->progressWiimoteYAxis->setValue(wiimoteAccTable.y);
    ui->progressWiimoteZAxis->setValue(wiimoteAccTable.z);
    ui->sliderWiimotePitchAxis->setValue(wiimoteAccTable.pitch*-90);
    ui->sliderWiimoteRollAxis->setValue(wiimoteAccTable.roll*-90);
}

void MainWindow::dbusNunchukAcc(quint32 id, struct accdata table)
{
    if (id != wiimoteId)
        return;

    nunchukAccTable = table;

    ui->progressNunchukXAxis->setValue(nunchukAccTable.x);
    ui->progressNunchukYAxis->setValue(nunchukAccTable.y);
    ui->progressNunchukZAxis->setValue(nunchukAccTable.z);
    ui->sliderNunchukPitchAxis->setValue(nunchukAccTable.pitch*-90);
    ui->sliderNunchukRollAxis->setValue(nunchukAccTable.roll*-90);
}

