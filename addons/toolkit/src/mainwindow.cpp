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
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(&infraredTimeout, SIGNAL(timeout()), this, SLOT(infraredCleanup()));
    infraredTimeout.setInterval(30);

    text_buttons_.insert(WIIMOTE_BTN_1, "Wiiremote 1");
    text_buttons_.insert(WIIMOTE_BTN_2, "Wiiremote 2");
    text_buttons_.insert(WIIMOTE_BTN_A, "Wiiremote A");
    text_buttons_.insert(WIIMOTE_BTN_B, "Wiiremote B");
    text_buttons_.insert(WIIMOTE_BTN_PLUS, "Wiiremote Plus");
    text_buttons_.insert(WIIMOTE_BTN_MINUS, "Wiiremote Minus");
    text_buttons_.insert(WIIMOTE_BTN_HOME, "Wiiremote Home");
    text_buttons_.insert(WIIMOTE_BTN_UP, "Wiiremote Up");
    text_buttons_.insert(WIIMOTE_BTN_DOWN, "Wiiremote Down");
    text_buttons_.insert(WIIMOTE_BTN_LEFT, "Wiiremote Left");
    text_buttons_.insert(WIIMOTE_BTN_RIGHT, "Wiiremote Right");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_UP, "Wiiremote Shift Up");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_DOWN, "Wiiremote Shift Down");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_LEFT, "Wiiremote Shift Left");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_RIGHT, "Wiiremote Shift Right");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_FORWARD, "Wiiremote Shift Forward");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_BACKWARD, "Wiiremote Shift Backward");
    text_buttons_.insert(WIIMOTE_BTN_TILT_FRONT, "Wiiremote Tilt Front");
    text_buttons_.insert(WIIMOTE_BTN_TILT_BACK, "Wiiremote Tilt Back");
    text_buttons_.insert(WIIMOTE_BTN_TILT_LEFT, "Wiiremote Tilt Left");
    text_buttons_.insert(WIIMOTE_BTN_TILT_RIGHT, "Wiiremote Tilt Right");
    text_buttons_.insert(NUNCHUK_BTN_Z, "Nunchuk Z");
    text_buttons_.insert(NUNCHUK_BTN_C, "Nunchuk B");
    text_buttons_.insert(NUNCHUK_BTN_STICK_UP, "Nunchuk Stick Up");
    text_buttons_.insert(NUNCHUK_BTN_STICK_DOWN, "Nunchuk Stick Down");
    text_buttons_.insert(NUNCHUK_BTN_STICK_LEFT, "Nunchuk Stick Left");
    text_buttons_.insert(NUNCHUK_BTN_STICK_RIGHT, "Nunchuk Stick Right");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_UP, "Nunchuk Shift Up");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_DOWN, "Nunchuk Shift Down");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_LEFT, "Nunchuk Shift Left");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_RIGHT, "Nunchuk Shift Right");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_FORWARD, "Nunchuk Shift Forward");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_BACKWARD, "Nunchuk Shift Backward");
    text_buttons_.insert(NUNCHUK_BTN_TILT_FRONT, "Nunchuk Tilt Front");
    text_buttons_.insert(NUNCHUK_BTN_TILT_BACK, "Nunchuk Tilt Back");
    text_buttons_.insert(NUNCHUK_BTN_TILT_LEFT, "Nunchuk Tilt Left");
    text_buttons_.insert(NUNCHUK_BTN_TILT_RIGHT, "Nunchuk Tilt Right");
    text_buttons_.insert(CLASSIC_BTN_X, "Classic X");
    text_buttons_.insert(CLASSIC_BTN_Y, "Classic Y");
    text_buttons_.insert(CLASSIC_BTN_A, "Classic A");
    text_buttons_.insert(CLASSIC_BTN_B, "Classic B");
    text_buttons_.insert(CLASSIC_BTN_L, "Classic L");
    text_buttons_.insert(CLASSIC_BTN_R, "Classic R");
    text_buttons_.insert(CLASSIC_BTN_ZL, "Classic ZL");
    text_buttons_.insert(CLASSIC_BTN_ZR, "Classic ZR");
    text_buttons_.insert(CLASSIC_BTN_MINUS, "Classic Minus");
    text_buttons_.insert(CLASSIC_BTN_PLUS, "Classic Plus");
    text_buttons_.insert(CLASSIC_BTN_HOME, "Classic Home");
    text_buttons_.insert(CLASSIC_BTN_UP, "Classic Up");
    text_buttons_.insert(CLASSIC_BTN_DOWN, "Classic Down");
    text_buttons_.insert(CLASSIC_BTN_LEFT, "Classic Left");
    text_buttons_.insert(CLASSIC_BTN_RIGHT, "Classic Right");
    text_buttons_.insert(CLASSIC_BTN_LSTICK_UP, "Classic Left-Stick Up");
    text_buttons_.insert(CLASSIC_BTN_LSTICK_DOWN, "Classic Left-Stick Down");
    text_buttons_.insert(CLASSIC_BTN_LSTICK_LEFT, "Classic Left-Stick Left");
    text_buttons_.insert(CLASSIC_BTN_LSTICK_RIGHT, "Classic Left-Stick Right");
    text_buttons_.insert(CLASSIC_BTN_RSTICK_UP, "Classic Right-Stick Up");
    text_buttons_.insert(CLASSIC_BTN_RSTICK_DOWN, "Classic Right-Stick Down");
    text_buttons_.insert(CLASSIC_BTN_RSTICK_LEFT, "Classic Right-Stick Left");
    text_buttons_.insert(CLASSIC_BTN_RSTICK_RIGHT, "Classic Right-Stick Right");
    text_buttons_.insert(WIIMOTE_BTN_SHIFT_SHAKE, "Wiiremote Shift Shake");
    text_buttons_.insert(NUNCHUK_BTN_SHIFT_SHAKE, "Nunchuk Shift Shake");

    wiimoteId = 1;
    widthMultiplier = 0.5;
    heightMultiplier = 0.5;
    dotSizeMultiplier = 4;

    qRegisterMetaType< irpoint>("irpoint");
    qRegisterMetaType< QList< irpoint> >("QList< irpoint>");

    ui->setupUi(this);
    ui->toolBox->hide();

    scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(QColor(0x0A, 0x0A, 0x0A, 0xFF), Qt::SolidPattern));

    scene->setSceneRect(0, 0, 512, 384);

    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);


    infraredTitle.setHtml("<font color=\"#aaaaaa\">Infrared:</font>");
    infraredTitle.setPos(0, 0);

    infraredPointsText[0] = new QGraphicsTextItem();
    infraredPointsText[1] = new QGraphicsTextItem();
    infraredPointsText[2] = new QGraphicsTextItem();
    infraredPointsText[3] = new QGraphicsTextItem();
    infraredPointsText[0]->setY(25);
    infraredPointsText[1]->setY(40);
    infraredPointsText[2]->setY(55);
    infraredPointsText[3]->setY(70);

    infraredGroup.addToGroup(&infraredTitle);
    infraredGroup.addToGroup(infraredPointsText[0]);
    infraredGroup.addToGroup(infraredPointsText[1]);
    infraredGroup.addToGroup(infraredPointsText[2]);
    infraredGroup.addToGroup(infraredPointsText[3]);

    updateInfraredInfo(QList < irpoint>() );

    wiimoteAccTitle.setHtml("<font color=\"#aaaaaa\">Accelerometer:</font>");
    wiimoteAccTitle.setPos(0, 0);

    wiimoteAccPointsText[0] = new QGraphicsTextItem();
    wiimoteAccPointsText[1] = new QGraphicsTextItem();
    wiimoteAccPointsText[2] = new QGraphicsTextItem();
    wiimoteAccPointsText[3] = new QGraphicsTextItem();
    wiimoteAccPointsText[4] = new QGraphicsTextItem();
    wiimoteAccPointsText[0]->setY(25);
    wiimoteAccPointsText[1]->setY(40);
    wiimoteAccPointsText[2]->setY(55);
    wiimoteAccPointsText[3]->setY(70);
    wiimoteAccPointsText[4]->setY(85);

    wiimoteAccGroup.addToGroup(&wiimoteAccTitle);
    wiimoteAccGroup.addToGroup(wiimoteAccPointsText[0]);
    wiimoteAccGroup.addToGroup(wiimoteAccPointsText[1]);
    wiimoteAccGroup.addToGroup(wiimoteAccPointsText[2]);
    wiimoteAccGroup.addToGroup(wiimoteAccPointsText[3]);
    wiimoteAccGroup.addToGroup(wiimoteAccPointsText[4]);

    wiimoteAccGroup.setY(95);

    updateWiimoteAccInfo(0, 0, 0, 0.0, 0.0);

    wiimoteStdButtonText.setY(512);
    wiimoteExtButtonText.setY(527);

    updateButtonInfo(0);


    scene->addItem(dotItems[0] = new DotItem);
    scene->addItem(dotItems[1] = new DotItem);
    scene->addItem(dotItems[2] = new DotItem);
    scene->addItem(dotItems[3] = new DotItem);

    scene->addItem(&wiimoteStdButtonText);
    scene->addItem(&wiimoteExtButtonText);
    scene->addItem(&infraredGroup);
    scene->addItem(&wiimoteAccGroup);

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

    connect(iface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));

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

void MainWindow::updateButtonInfo(quint64 value) {
  quint64 std = 0;
  std |= value & WIIMOTE_BUTTON_MASK;
  std |= value & NUNCHUK_BUTTON_MASK;
  std |= value & CLASSIC_BUTTON_MASK;

  quint64 ext = 0;
  ext |= value & WIIMOTE_TILT_MASK;
  ext |= value & WIIMOTE_SHIFT_MASK;
  ext |= value & NUNCHUK_TILT_MASK;
  ext |= value & NUNCHUK_SHIFT_MASK;
  ext |= value & NUNCHUK_STICK_MASK;
  ext |= value & CLASSIC_LSTICK_MASK;
  ext |= value & CLASSIC_RSTICK_MASK;

  wiimoteStdButtonText.setY(ui->graphicsView->geometry().height()-50);
  wiimoteExtButtonText.setY(ui->graphicsView->geometry().height()-35);
  wiimoteStdButtonText.setHtml(QString("<font color=#999999>std: </font>%1").arg(getReadableWiiremoteSequence(std)));
  wiimoteExtButtonText.setHtml(QString("<font color=#999999>ext: </font>%1").arg(getReadableWiiremoteSequence(ext)));
}

void MainWindow::updateInfraredInfo(QList < irpoint> list)
{
  int i = 0;
  for (; i < list.count(); ++i)
    infraredPointsText[i]->setHtml(QString("<font color=#555555>%1: </font>" \
      "<font color=#ffffff>%2</font><font color=#999999>x</font>" \
      "<font color=#ffffff>%3</font><font color=#555555> size: </font>" \
      "<font color=#ffffff>%4</font>").arg(QString::number(i), QString::number(list.at(i).x)
                                              , QString::number(list.at(i).y), QString::number(list.at(i).size)));
  for (; i < 4; ++i)
    infraredPointsText[i]->setHtml(QString("<font color=#555555>%1: </font>" \
      "<font color=#ffffff>%2</font><font color=#999999>x</font>" \
      "<font color=#ffffff>%3</font><font color=#555555> size: </font>" \
      "<font color=#ffffff>%4</font>").arg(QString::number(i), "0", "0", "-1"));

}

void MainWindow::updateWiimoteAccInfo(int x, int y, int z, double p, double r) {
  wiimoteAccPointsText[0]->setHtml(QString("<font color=#555555>x: " \
    "</font><font color=#777777>%1</font>").arg(QString::number(x)));
  wiimoteAccPointsText[1]->setHtml(QString("<font color=#555555>y: " \
    "</font><font color=#777777>%1</font>").arg(QString::number(y)));
  wiimoteAccPointsText[2]->setHtml(QString("<font color=#555555>z: " \
    "</font><font color=#777777>%1</font>").arg(QString::number(z)));
  wiimoteAccPointsText[3]->setHtml(QString("<font color=#555555>pitch: " \
    "</font><font color=#777777>%1</font>").arg(QString::number(p, 'g', 4)));
  wiimoteAccPointsText[4]->setHtml(QString("<font color=#555555>roll: " \
    "</font><font color=#777777>%1</font>").arg(QString::number(r, 'g', 4)));
}

QString MainWindow::getReadableWiiremoteSequence(quint64 value) {
  QMapIterator<quint64, QString> text(text_buttons_);

  QStringList list;
  while (text.hasNext()) {
    text.next();
    if ((text.key() & value) == text.key())
      list << QString("<font color=#dddddd>%1</font>").arg(text.value());
  }

  list.sort();

  QString output;
  if (!list.isEmpty()) {
    for (int i = 0; i < (list.count() - 1); ++i)
      output += QString("%1<font color=#444444> + </font>").arg(list.at(i) );
    output += list.last();
  } else
    output = "<font color=#444444>none</font>";

  return output;
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
}

void MainWindow::dbusNunchukUnplugged(quint32 id)
{
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

void MainWindow::dbusWiimoteGeneralButtons(quint32 id, quint64 value) {
  if (id != wiimoteId)
      return;

  updateButtonInfo(value);
}

void MainWindow::dbusWiimoteAcc(quint32 id, struct accdata table)
{
    if (id != wiimoteId)
        return;

    updateWiimoteAccInfo(table.x, table.y, table.z, table.pitch*-90, table.roll*-90);
}

void MainWindow::dbusNunchukAcc(quint32 id, struct accdata table)
{
    if (id != wiimoteId)
        return;

}

