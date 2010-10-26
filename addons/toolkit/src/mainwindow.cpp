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

MainWindow::MainWindow(QWidget *parent) :
  QGraphicsView(parent)
{
  QPalette windowColor;
  QBrush brush(QColor(255, 255, 255, 255));
  brush.setStyle(Qt::SolidPattern);
  windowColor.setBrush(QPalette::Active, QPalette::WindowText, brush);
  QBrush brush1(QColor(0, 0, 0, 255));
  brush1.setStyle(Qt::SolidPattern);
  windowColor.setBrush(QPalette::Active, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Text, brush);
  windowColor.setBrush(QPalette::Active, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Active, QPalette::ButtonText, brush);
  windowColor.setBrush(QPalette::Active, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Window, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
  QBrush brush2(QColor(255, 255, 220, 255));
  brush2.setStyle(Qt::SolidPattern);
  windowColor.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Text, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Window, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Text, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Window, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
  setPalette(windowColor);
//

  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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


  connect(&infraredTimeout, SIGNAL(timeout()), this, SLOT(infraredCleanup()));
  infraredTimeout.setInterval(30);

  scene.setBackgroundBrush(QBrush(QColor(0x0A, 0x0A, 0x0A, 0xFF), Qt::SolidPattern));

  scene.setSceneRect(0, 0, 1024/2, 768/2);
  this->setGeometry(QRect(0, 0, 1024/2, 768/2));

  setScene(&scene);

  quint32 x = 0;

  infraredTitle = new QGraphicsTextItem();
  infraredTitle->setHtml("<font color=\"#ffffff\">Infrared:</font>");
  infraredTitle->setPos(0, 0);

  infraredGroup = new QGraphicsItemGroup();
  infraredGroup->addToGroup(infraredTitle);

  x = 10;
  for (register int i = 0; i < 4; ++i) {
    infraredPointsText[i] = new QGraphicsTextItem();
    infraredPointsText[i]->setY(x += 15);
    infraredPointsText[i]->setX(5);
    infraredGroup->addToGroup(infraredPointsText[i]);
  }

  infraredGroup->setX(160);

  updateInfraredInfo(QList < irpoint>() );

  accelerometrTitle = new QGraphicsTextItem();
  accelerometrTitle->setHtml("<font color=#ffffff>Accelerometer:</font>");
  accelerometrTitle->setPos(0, 0);
  accelerometrGroup = new QGraphicsItemGroup();
  accelerometrGroup->addToGroup(accelerometrTitle);

  x = 10;
  for (register int i = 0; i < 10; ++i) {
    accelerometrPointsText[i] = new QGraphicsTextItem();
    accelerometrPointsText[i]->setY(x += 15);
    accelerometrPointsText[i]->setX(5);
    if (i == 4) x += 15;
    accelerometrGroup->addToGroup(accelerometrPointsText[i]);
  }
  accelerometrGroup->setX(0);

  memset(&nunchuk_acc, 0, sizeof(struct accdata));
  memset(&wiimote_acc, 0, sizeof(struct accdata));

  updateAccelerometrInfo(wiimote_acc.x, wiimote_acc.y, wiimote_acc.z, wiimote_acc.pitch, wiimote_acc.roll,
                         nunchuk_acc.x, nunchuk_acc.y, nunchuk_acc.z, nunchuk_acc.pitch, nunchuk_acc.roll);

  wiimoteStdButtonText = new QGraphicsTextItem();
  wiimoteExtButtonText = new QGraphicsTextItem();
  batteryItem = new WiimoteBatteryItem();

  for (register int i = 0; i < 4; ++i)
    ledPixmaps[i] = new WiimoteLedItem();

  line = new QGraphicsLineItem();

  updateButtonInfo(0);

  for (register int i = 0; i < 4; ++i) {
    infraredPoints[i] = new QGraphicsEllipseItem();
    infraredPoints[i]->setPen(QPen(Qt::white));
    infraredPoints[i]->setRect(0, 0, 3, 3);
    infraredPoints[i]->hide();
    infraredPoints[i]->setZValue(10.0);
    infraredLine[i] = new QGraphicsLineItem();
    infraredLine[i]->setPen(QPen(Qt::yellow));
    infraredLine[i]->hide();
    infraredLine[i]->setZValue(5.0);
    scene.addItem(infraredPoints[i]);
    scene.addItem(infraredLine[i]);
  }


  line->setPen(QPen(Qt::darkGray));

  scene.addItem(wiimoteStdButtonText);
  scene.addItem(wiimoteExtButtonText);
  scene.addItem(batteryItem);
  scene.addItem(infraredGroup);
  scene.addItem(accelerometrGroup);
  scene.addItem(line);

  iface = new DBusDeviceEventsInterface(WIIMOTEDEV_DBUS_SERVICE_NAME,
                                        WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                                        QDBusConnection::systemBus(),
                                        this);

  connect(iface, SIGNAL(dbusNunchukPlugged(quint32)), this, SLOT(dbusNunchukPlugged(quint32)));
  connect(iface, SIGNAL(dbusNunchukUnplugged(quint32)), this, SLOT(dbusNunchukUnplugged(quint32)));

  connect(iface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));

  connect(iface, SIGNAL(dbusNunchukStick(quint32,stickdata)), this, SLOT(dbusNunchukStick(quint32,stickdata)));
  connect(iface, SIGNAL(dbusClassicControllerRStick(quint32,stickdata)), this, SLOT(dbusClassicControllerRStick(quint32,stickdata)));
  connect(iface, SIGNAL(dbusClassicControllerLStick(quint32,stickdata)), this, SLOT(dbusClassicControllerLStick(quint32,stickdata)));

  connect(iface, SIGNAL(dbusWiimoteBatteryLife(quint32,quint8)), this, SLOT(dbusWiimoteBatteryLife(quint32,quint8)));
  connect(iface, SIGNAL(dbusWiimoteInfrared(quint32,QList<struct irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList<struct irpoint>)));
  connect(iface, SIGNAL(dbusWiimoteAcc(quint32,struct accdata)), this, SLOT(dbusWiimoteAcc(quint32,struct accdata)));
  connect(iface, SIGNAL(dbusNunchukAcc(quint32,struct accdata)), this, SLOT(dbusNunchukAcc(quint32,accdata)));

  connect(ledPixmaps[0], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed1(bool)));
  connect(ledPixmaps[1], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed2(bool)));
  connect(ledPixmaps[2], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed3(bool)));
  connect(ledPixmaps[3], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed4(bool)));

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

  batteryItem->setBatteryLevel(iface->dbusWiimoteGetBatteryLife(wiimoteId));

  for (register int i = 0; i < 4; ++i) {
    if ((leds >> i) & true)
      ledPixmaps[i]->switchOn(); else
      ledPixmaps[i]->switchOff();
    scene.addItem(ledPixmaps[i]);
  }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QRect rect = geometry();

  quint32 x = rect.width() - 16;
  quint32 y = rect.height() - 32;

  wiimoteStdButtonText->setY(geometry().height()-50);
  wiimoteExtButtonText->setY(geometry().height()-35);

  line->setLine(-(geometry().width()/10), geometry().height()/2, geometry().width()+(geometry().width()/10), geometry().height()/2);
  line->setTransformOriginPoint(geometry().width()/2,geometry().height()/2);

  for (register int i = 3; i >= 0; --i)
    ledPixmaps[i]->setPos(x -= (16 + 5), y);

  batteryItem->setPos(x, rect.height() - batteryItem->boundingRect().height() - 9);
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

  wiimoteStdButtonText->setHtml(QString("<font color=#999999>std: </font>%1").arg(getReadableWiiremoteSequence(std)));
  wiimoteExtButtonText->setHtml(QString("<font color=#999999>ext: </font>%1").arg(getReadableWiiremoteSequence(ext)));
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

void MainWindow::updateAccelerometrInfo(int x1, int y1, int z1, double p1, double r1,
                                        int x2, int y2, int z2, double p2, double r2) {
  accelerometrPointsText[0]->setHtml(QString("<font color=#555555> X-Axis: " \
    "</font><font color=#ffffff>%1</font>").arg(QString::number(x1)));
  accelerometrPointsText[1]->setHtml(QString("<font color=#555555> Y-Axis: " \
    "</font><font color=#ffffff>%1</font>").arg(QString::number(y1)));
  accelerometrPointsText[2]->setHtml(QString("<font color=#555555> Z-Axis: " \
    "</font><font color=#ffffff>%1</font>").arg(QString::number(z1)));
  accelerometrPointsText[3]->setHtml(QString::fromUtf8("<font color=#555555> Pitch: " \
    "</font><font color=#ffffff>%1°</font>").arg(QString::number(static_cast< qint32>(p1))));
  accelerometrPointsText[4]->setHtml(QString::fromUtf8("<font color=#555555> Roll: " \
    "</font><font color=#ffffff>%1°</font>").arg(QString::number(static_cast< qint32>(r1))));
  accelerometrPointsText[5]->setHtml(QString("<font color=#555555> X-Axis: " \
    "</font><font color=#ffffff>%1</font>").arg(QString::number(x2)));
  accelerometrPointsText[6]->setHtml(QString("<font color=#555555> Y-Axis: " \
    "</font><font color=#ffffff>%1</font>").arg(QString::number(y2)));
  accelerometrPointsText[7]->setHtml(QString("<font color=#555555> Z-Axis: " \
    "</font><font color=#ffffff>%1</font>").arg(QString::number(z2)));
  accelerometrPointsText[8]->setHtml(QString::fromUtf8("<font color=#555555> Pitch: " \
    "</font><font color=#ffffff>%1°</font>").arg(QString::number(static_cast< qint32>(p2))));
  accelerometrPointsText[9]->setHtml(QString::fromUtf8("<font color=#555555> Roll: " \
    "</font><font color=#ffffff>%1°</font>").arg(QString::number(static_cast< qint32>(r2))));
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

void MainWindow::dbusWiimoteBatteryLife(quint32 id, quint8 life) {
  if (id != wiimoteId)
    return;

  batteryItem->setBatteryLevel(life);
}

void MainWindow::getWiimoteStats()
{
  leds = iface->dbusWiimoteGetLedStatus(wiimoteId);
//  ui->checkboxRumble->setChecked(iface->dbusWiimoteGetRumbleStatus(wiimoteId));
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
//    nunchukStickDot->setPos(-x, y);
}

void MainWindow::dbusClassicControllerLStick(quint32 id, struct stickdata stick)
{
  if (id != wiimoteId)
      return;

  register int x = ((0x3F >> 1) - stick.x) * 1.2;
  register int y = ((0x3F >> 1) - stick.y) * 1.2;
//    classicLStickDot->setPos(-x, y);
}

void MainWindow::dbusClassicControllerRStick(quint32 id, struct stickdata stick)
{
  if (id != wiimoteId)
    return;

  register int x = ((0x1F >> 1) - stick.x) * 2.3;
  register int y = ((0x1F >> 1) - stick.y) * 2.3;
//    classicRStickDot->setPos(-x, y);
}

void MainWindow::infraredCleanup()
{
  infraredTimeout.stop();

  for (register int i = 0; i < 4; ++i) {
    infraredPoints[i]->hide();
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

  if (sceneRect().width() != width() ||
      sceneRect().height() != height())
      setSceneRect(0, 0, width(), height());

  widthMultiplier = static_cast< double> (width()) / 1024.0;
  heightMultiplier = static_cast< double> (height()) / 768.0;
  dotSizeMultiplier = (widthMultiplier + heightMultiplier) / 2.0;

  infraredTimeout.start();
  for (register int i = 0; i < 4; ++i) {
    if (i < points.count()) {
      register int size = points.at(i).size * 2;
      infraredPoints[i]->setRect(-size, -size, size, size);

      if (!infraredPoints[i]->isVisible())
        infraredPoints[i]->show();

      infraredPoints[i]->setPos((static_cast< double>( points.at(i).x) * widthMultiplier) - (size/2),
                               (static_cast< double>( points.at(i).y) * heightMultiplier) - (size/2));
    } else {
      if (infraredPoints[i]->isVisible())
        infraredPoints[i]->hide();
    }
  }

  switch (points.count()) {
  case ir0source:
  case ir1source:
    for (register int i = 0; i < 4; ++i)
      if (infraredLine[i]->isVisible()) infraredLine[i]->hide();
    break;

  case ir2source:
    for (register int i = 1; i < 4; ++i)
      if (infraredLine[i]->isVisible()) infraredLine[i]->hide();

    infraredLine[0]->setLine(infraredPoints[0]->x(), infraredPoints[0]->y(), infraredPoints[1]->x(), infraredPoints[1]->y());
    if (!infraredLine[0]->isVisible()) infraredLine[0]->show();
    break;

  case ir3source:
    if (infraredLine[3]->isVisible()) infraredLine[3]->hide();

    infraredLine[0]->setLine(infraredPoints[0]->x(), infraredPoints[0]->y(), infraredPoints[1]->x(), infraredPoints[1]->y());
    infraredLine[1]->setLine(infraredPoints[1]->x(), infraredPoints[1]->y(), infraredPoints[2]->x(), infraredPoints[2]->y());
    infraredLine[2]->setLine(infraredPoints[2]->x(), infraredPoints[2]->y(), infraredPoints[0]->x(), infraredPoints[0]->y());

    if (!infraredLine[0]->isVisible()) infraredLine[0]->show();
    if (!infraredLine[1]->isVisible()) infraredLine[1]->show();
    if (!infraredLine[2]->isVisible()) infraredLine[2]->show();
    break;

  case ir4source:
    infraredLine[0]->setLine(infraredPoints[0]->x(), infraredPoints[0]->y(), infraredPoints[1]->x(), infraredPoints[1]->y());
    infraredLine[1]->setLine(infraredPoints[1]->x(), infraredPoints[1]->y(), infraredPoints[2]->x(), infraredPoints[2]->y());
    infraredLine[2]->setLine(infraredPoints[2]->x(), infraredPoints[2]->y(), infraredPoints[3]->x(), infraredPoints[3]->y());
    infraredLine[3]->setLine(infraredPoints[3]->x(), infraredPoints[3]->y(), infraredPoints[0]->x(), infraredPoints[0]->y());

    if (!infraredLine[0]->isVisible()) infraredLine[0]->show();
    if (!infraredLine[1]->isVisible()) infraredLine[1]->show();
    if (!infraredLine[2]->isVisible()) infraredLine[2]->show();
    if (!infraredLine[3]->isVisible()) infraredLine[3]->show();
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

  memcpy(&wiimote_acc, &table, sizeof(table));

  updateAccelerometrInfo(wiimote_acc.x, wiimote_acc.y, wiimote_acc.z, wiimote_acc.pitch, wiimote_acc.roll,
                         nunchuk_acc.x, nunchuk_acc.y, nunchuk_acc.z, nunchuk_acc.pitch, nunchuk_acc.roll);

  line->setRotation(table.roll);
}

void MainWindow::dbusNunchukAcc(quint32 id, struct accdata table)
{
  if (id != wiimoteId)
      return;

  memcpy(&nunchuk_acc, &table, sizeof(table));

  updateAccelerometrInfo(wiimote_acc.x, wiimote_acc.y, wiimote_acc.z, wiimote_acc.pitch, wiimote_acc.roll,
                         nunchuk_acc.x, nunchuk_acc.y, nunchuk_acc.z, nunchuk_acc.pitch, nunchuk_acc.roll);

}

