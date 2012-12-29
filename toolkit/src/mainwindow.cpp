/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
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
#include "interfaces/deviceevents.h"

#include <math.h>
#include <QDebug>

#include <QtOpenGL/QGLWidget>
#define PI M_PI

MainWindow::MainWindow(DBusDeviceEventsInterface *iface, uint id, QWidget *parent) :
  QGraphicsView(parent),
  cursor(new QGraphicsPixmapItem(QPixmap(":/cursor.png"))),
  iface(iface),
  wiimoteId(id),
  order(RightToLeft)
{ setWindowIcon(QIcon(":/icon16.png"));

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

  widthMultiplier = 0.5;
  heightMultiplier = 0.5;
  dotSizeMultiplier = 4;

  memset(&sticks, 0, sizeof(stickdata)*2);

  connect(&infraredTimeout, SIGNAL(timeout()), this, SLOT(infraredCleanup()));
  infraredTimeout.setInterval(30);

  scene.setBackgroundBrush(QBrush(QColor(0x0A, 0x0A, 0x0A, 0xFF), Qt::SolidPattern));

  scene.setSceneRect(0, 0, 1024/1.5, 768/1.5);
  setGeometry(QRect(0, 0, 1024/1.5, 768/1.5));

  setScene(&scene);

  cursor->setZValue(100);
  cursor->setTransformOriginPoint(7, 2);
  cursor->setTransformationMode(Qt::SmoothTransformation);
  cursor->setVisible(false);
  scene.addItem(cursor);

  QFont font;
  font.setStyleHint(QFont::TypeWriter);

  accelerometrInfo = new QGraphicsTextItem();
  accelerometrInfo->setPos(0, 0);
  accelerometrInfo->setFont(font);

  analogInfo = new QGraphicsTextItem();
  analogInfo->setPos(290, 50);
  analogInfo->setFont(font);

  infraredInfo = new QGraphicsTextItem();
  infraredInfo->setPos(170, 0);
  infraredInfo->setFont(font);


  statusInfo = new QGraphicsTextItem();
  statusInfo->setPos(290, 0);

  updateInfraredInfo(QList < irpoint>() );


  memset(&nunchuk_acc, 0, sizeof(struct accdata));
  memset(&wiimote_acc, 0, sizeof(struct accdata));

  updateAccelerometrInfo(wiimote_acc.x, wiimote_acc.y, wiimote_acc.z, wiimote_acc.pitch, wiimote_acc.roll,
                         nunchuk_acc.x, nunchuk_acc.y, nunchuk_acc.z, nunchuk_acc.pitch, nunchuk_acc.roll);

  wiimoteStdButtonText = new QGraphicsTextItem();
  wiimoteExtButtonText = new QGraphicsTextItem();
  batteryItem = new WiimoteBatteryItem();
  rumbleItem = new WiimoteRumbleItem();

  for (register int i = 0; i < 4; ++i) {
    ledPixmaps[i] = new WiimoteLedItem();
    scene.addItem(ledPixmaps[i]);
  }

  line = new QGraphicsLineItem();
  line->setZValue(-1);

  updateButtonInfo(0);

  for (register int i = 0; i < 4; ++i) {
    infraredPoints[i] = new QGraphicsEllipseItem();
    infraredPoints[i]->setPen(QPen(Qt::white));
    infraredPoints[i]->setBrush(QBrush(Qt::black));
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
  scene.addItem(rumbleItem);

  scene.addItem(accelerometrInfo);
  scene.addItem(analogInfo);
  scene.addItem(infraredInfo);
  scene.addItem(statusInfo);
  scene.addItem(line);

  connect(ledPixmaps[0], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed1(bool)));
  connect(ledPixmaps[1], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed2(bool)));
  connect(ledPixmaps[2], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed3(bool)));
  connect(ledPixmaps[3], SIGNAL(ledSwitched(bool)), this, SLOT(toggleLed4(bool)));

  connect(rumbleItem, SIGNAL(rumbleSwitched(bool)), this, SLOT(toggleRumble(bool)));

  if (iface->dbusIsWiimoteConnected(wiimoteId))
    dbusWiimoteConnected(wiimoteId); else
    dbusWiimoteDisconnected(wiimoteId);
}

void MainWindow::updateStatusInfo() {
  QString html = "<font color=#ffffff>Extensions</font><br>";

  html += "<font color=#555555>classic: </font>";
  if (iface->dbusIsClassicConnected(wiimoteId))
    html += "<b><font color=#eeeeee>connected</font></b>"; else
    html += "<font color=#aaaaaa>disconnected</font>";
  html += "<br>";

  html += "<font color=#555555>nunchuk: </font>";
  if (iface->dbusIsNunchukConnected(wiimoteId))
    html += "<b><font color=#eeeeee>connected</font></b>"; else
    html += "<font color=#aaaaaa>disconnected</font>";
  html += "<br>";

  statusInfo->setHtml(html);
}

void MainWindow::dbusWiimoteConnected(uint id) {
  if (wiimoteId != id)
    return;

  leds = iface->dbusWiimoteGetLedStatus(id);
  batteryItem->setBatteryLevel(iface->dbusWiimoteGetBatteryLife(wiimoteId));

  for (register int i = 0; i < 4; ++i)  {
    ledPixmaps[i]->setEnabled(true);
    if ((leds >> i) & 1)
      ledPixmaps[i]->switchOn(); else
      ledPixmaps[i]->switchOff();
  }

  batteryItem->setEnabled(true);

  if (iface->dbusWiimoteGetRumbleStatus(id))
    rumbleItem->switchOn(); else
    rumbleItem->switchOff();

  rumbleItem->setEnabled(true);

  analogInfo->setVisible(iface->dbusIsClassicConnected(wiimoteId) ||
                         iface->dbusIsNunchukConnected(wiimoteId));
  if (iface->dbusIsClassicConnected(wiimoteId)) analogMode = modeClassic; else
  if (iface->dbusIsNunchukConnected(wiimoteId)) analogMode = modeNunchuk; else
    analogMode = modeNone;

  macAddress = iface->dbusWiimoteGetMacAddress(wiimoteId);

  setWindowTitle(QString("Wiimotedev Toolkit :: Wiiremote[%2] %1 :: Connected").arg(macAddress, QString::number(wiimoteId)));
  updateStatusInfo();
}

void MainWindow::dbusWiimoteDisconnected(uint id){
  if (wiimoteId != id)
    return;

  for (register int i = 0; i < 4; ++i) {
    ledPixmaps[i]->setEnabled(false);
    ledPixmaps[i]->switchOff();
  }
  batteryItem->setEnabled(false);
  batteryItem->setBatteryLevel(0);
  rumbleItem->setEnabled(false);
  rumbleItem->switchOff();

  setWindowTitle(QString("Wiimotedev Toolkit :: Wiiremote[%1] :: Disconnected").arg(QString::number(wiimoteId)));
  updateStatusInfo();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QRect rect = geometry();

  uint x = rect.width() - 16;
  uint y = rect.height() - 32;

  wiimoteStdButtonText->setY(geometry().height()-50);
  wiimoteExtButtonText->setY(geometry().height()-35);

  line->setLine(-(geometry().width()/10), geometry().height()/2, geometry().width()+(geometry().width()/10), geometry().height()/2);
  line->setTransformOriginPoint(geometry().width()/2,geometry().height()/2);



  for (register int i = 3; i >= 0; --i)
    ledPixmaps[i]->setPos(x -= (16 + 5), y);

  rumbleItem->setPos(x - rumbleItem->pixmap().width() - 10, y + 2);

  batteryItem->setPos(x, rect.height() - batteryItem->boundingRect().height() - 9);
}

void MainWindow::updateButtonInfo(uint64 value) {
  uint64 std = 0;
  std |= value & WIIMOTE_BUTTON_MASK;
  std |= value & NUNCHUK_BUTTON_MASK;
  std |= value & CLASSIC_BUTTON_MASK;

  uint64 ext = 0;
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

  QString html = "<font color=#ffffff>Infrared</font><br>";
  int i = 0;
  for (; i < list.count(); ++i)
    html +=
      "<font color=#555555>" + QString::number(i) +  ":</font>" \
      "<font color=#ffffff>" + QString::number(list.at(i).x) + "</font><font color=#999999>x</font>" \
      "<font color=#ffffff>" + QString::number(list.at(i).y) + "</font><font color=#555555> size: </font>" \
      "<font color=#ffffff>" + QString::number(list.at(i).size) + "</font><br>";
  for (; i < 4; ++i)
    html +=
      "<font color=#555555>" + QString::number(i) +  ":</font>" \
      "<font color=#ffffff>0</font><font color=#999999>x</font>" \
      "<font color=#ffffff>0</font><font color=#555555> size: </font>" \
      "<font color=#ffffff>-1</font><br>";

  if (list.count() > 1) {
    html +=
      "<font color=#555555>Infrared roll: </font><font color=#ffffff>" + QString::number(int(p*180/PI)) + "°</font><br>" \
      "<font color=#555555>Angle diff: </font><font color=#ffffff>" + QString::number(int(angleDiff)) + "°</font><br>" \
      "<font color=#555555>Line length: </font><font color=#ffffff>" + QString::number(int(lineLength)) + "px</font>";

  }

  infraredInfo->setHtml(html);
}

void MainWindow::updateAccelerometrInfo(int x1, int y1, int z1, double p1, double r1,
                                        int x2, int y2, int z2, double p2, double r2) {
  accelerometrInfo->setHtml(
    "<font color=#ffffff>Accelerometer (wiimote)</font><br>" \
    "<font color=#555555> X-Axis: </font><font color=#ffffff>" + QString::number(x1) + "</font><br>" \
    "<font color=#555555> Y-Axis: </font><font color=#ffffff>" + QString::number(y1) + "</font><br>" \
    "<font color=#555555> Z-Axis: </font><font color=#ffffff>" + QString::number(z1) + "</font><br>" \
    "<font color=#555555> Roll: </font><font color=#ffffff>" + QString::number(int(r1)) + "°</font><br>" \
    "<font color=#555555> Pitch: </font><font color=#ffffff>" + QString::number(int(p1)) + "°</font><br><br>" \
    "<font color=#ffffff>Accelerometer (nunchuk)</font><br>" \
    "<font color=#555555> X-Axis: </font><font color=#ffffff>" + QString::number(x2) + "</font><br>" \
    "<font color=#555555> Y-Axis: </font><font color=#ffffff>" + QString::number(y2) + "</font><br>" \
    "<font color=#555555> Z-Axis: </font><font color=#ffffff>" + QString::number(z2) + "</font><br>" \
    "<font color=#555555> Roll: </font><font color=#ffffff>" + QString::number(int(r2)) + "°</font><br>" \
    "<font color=#555555> Pitch: </font><font color=#ffffff>" + QString::number(int(p2)) + "°</font><br><br>");
}

QString MainWindow::getReadableWiiremoteSequence(uint64 value) {
  QMapIterator<uint64, QString> text(text_buttons_);

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

void MainWindow::toggleRumble(bool rumble) {
  iface->dbusWiimoteSetRumbleStatus(wiimoteId, rumble);
}

void MainWindow::changeDevicePushed()
{
//  QList <uint> list = iface->dbusGetWiimoteList();

//  selectWiimote = new SelectWiimote(this);
//  selectWiimote->setWiimoteList(list);
//  selectWiimote->show();
//  selectWiimote->exec();
//  if (selectWiimote->getSelectedWiimote())
//    wiimoteId = selectWiimote->getSelectedWiimote();
//  delete selectWiimote;
}

void MainWindow::dbusWiimoteBatteryLife(uint id, uint8 life) {
  if (id != wiimoteId)
    return;

  batteryItem->setBatteryLevel(life);
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

void MainWindow::dbusWiimoteLedStatusChanged(uint id, uint8 value) {
  if (id != wiimoteId)
    return;

  leds = value;

  for (register int i = 0; i < 4; ++i) {
    if (leds & (1 << i))
      ledPixmaps[i]->switchOn(); else
      ledPixmaps[i]->switchOff();
  }
}

void MainWindow::dbusWiimoteRumbleStatusChanged(uint id, uint8 value) {
  if (id != wiimoteId)
    return;

  if (value)
    rumbleItem->switchOn(); else
    rumbleItem->switchOff();
}

void MainWindow::dbusNunchukStick(uint id, const stickdata &stick)
{
  if (id != wiimoteId)
    return;

  sticks[0].x = stick.x;
  sticks[0].y = stick.y;

  updateAnalogInfo();
}

void MainWindow::updateAnalogInfo() {
  QString html;

  switch (analogMode) {
  case modeClassic:
    html = "<font color=#ffffff>Analog data (classic)</font><br>" \
           "<font color=#555555> Left X-Axis: </font><font color=#ffffff>" + QString::number(sticks[0].x) + "</font><br>" \
           "<font color=#555555> Left Y-Axis: </font><font color=#ffffff>" + QString::number(sticks[0].y) + "</font><br>" \
           "<font color=#555555> Right X-Axis: </font><font color=#ffffff>" + QString::number(sticks[1].x) + "</font><br>" \
           "<font color=#555555> Right Y-Axis: </font><font color=#ffffff>" + QString::number(sticks[1].y) + "</font>";
    break;
  case modeNunchuk:
    html = "<font color=#ffffff>Analog data (nunchuk)</font><br>" \
           "<font color=#555555> X-Axis: </font><font color=#ffffff>" + QString::number(sticks[0].x) + "</font><br>" \
           "<font color=#555555> Y-Axis: </font><font color=#ffffff>" + QString::number(sticks[0].y) + "</font>";
    break;
  }
  analogInfo->setHtml(html);
}

void MainWindow::dbusClassicControllerLStick(uint id, const stickdata &stick)
{
  if (id != wiimoteId)
    return;

  sticks[0].x = stick.x;
  sticks[0].y = stick.y;

  updateAnalogInfo();
}

void MainWindow::dbusClassicControllerRStick(uint id, const stickdata &stick)
{
  if (id != wiimoteId)
    return;

  sticks[1].x = stick.x;
  sticks[1].y = stick.y;

  updateAnalogInfo();
}

void MainWindow::infraredCleanup()
{
  infraredTimeout.stop();

  for (register int i = 0; i < 4; ++i) {
    infraredPoints[i]->hide();
  }
}

void MainWindow::dbusNunchukPlugged(uint id) {
  Q_UNUSED(id);
  analogInfo->setVisible(true);
  analogMode = modeNunchuk;
  updateAnalogInfo();
  updateStatusInfo();
}

void MainWindow::dbusNunchukUnplugged(uint id) {
  Q_UNUSED(id);
  analogInfo->setVisible(false);
  analogMode = modeNone;
  updateStatusInfo();
}

void MainWindow::dbusClassicPlugged(uint id) {
  Q_UNUSED(id);
  analogInfo->setVisible(true);
  analogMode = modeClassic;
  updateAnalogInfo();
  updateStatusInfo();
}

void MainWindow::dbusClassicUnplugged(uint id) {
  Q_UNUSED(id);
  analogInfo->setVisible(false);
  analogMode = modeNone;
  updateAnalogInfo();
  updateStatusInfo();
}

void MainWindow::dbusWiimoteInfrared(uint id, const QList<irpoint> &points)
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
      infraredPoints[i]->setRect(-size/2, -size/2, size, size);

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

void MainWindow::dbusVirtualCursorFound(uint id) {
  if (id != wiimoteId)
    return;

  cursor->setVisible(true);
}

void MainWindow::dbusVirtualCursorLost(uint id) {
  if (id != wiimoteId)
    return;

  cursor->setVisible(false);
}


void MainWindow::dbusVirtualCursorPosition(uint id, double x, double y, double size, double angle)
{
  if (id != wiimoteId)
    return;

  if (!cursor->isVisible())
    cursor->setVisible(true);

  cursor->setX((512.0 - x) * (geometry().width()/1024.0));
  cursor->setY((384.0 - y) * (geometry().height()/768.0));
  cursor->setScale((1024-size)/500);
  cursor->setRotation(-angle*180/M_PI);

  p = angle;
  lineLength = size;
}

void MainWindow::dbusWiimoteGeneralButtons(uint id, uint64 value) {
  if (id != wiimoteId)
    return;

  updateButtonInfo(value);
}

void MainWindow::dbusWiimoteAcc(uint id, const accdata &table)
{
  if (id != wiimoteId)
    return;

  memcpy(&wiimote_acc, &table, sizeof(table));

  double roll = -wiimote_acc.roll;

  if (roll < 0)
    roll = 360 - wiimote_acc.roll;

  updateAccelerometrInfo(wiimote_acc.x, wiimote_acc.y, wiimote_acc.z, wiimote_acc.pitch, roll,
                         nunchuk_acc.x, nunchuk_acc.y, nunchuk_acc.z, nunchuk_acc.pitch, nunchuk_acc.roll);

  line->setRotation(-roll);
}

void MainWindow::dbusNunchukAcc(uint id, const accdata &table)
{
  if (id != wiimoteId)
    return;

  memcpy(&nunchuk_acc, &table, sizeof(table));

  double roll = -wiimote_acc.roll;

  if (roll < 0)
    roll = 360 - wiimote_acc.roll;

  updateAccelerometrInfo(wiimote_acc.x, wiimote_acc.y, wiimote_acc.z, wiimote_acc.pitch, roll,
                         nunchuk_acc.x, nunchuk_acc.y, nunchuk_acc.z, nunchuk_acc.pitch, nunchuk_acc.roll);

}

