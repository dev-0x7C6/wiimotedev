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

#include <QDesktopWidget>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QPointer>
#include <QTextStream>
#include <QCursor>
#include <QMouseEvent>
#include <QDebug>
#include <QScrollBar>
#include "config.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString wiimotedevDirectory = ".wiimotedev/";
const QString profileDirectory = "profiles/";

MainWindow::MainWindow(DBusDeviceEventsInterface *daemon, QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
  logo(new QPixmap(":/logo.png")),
  daemon(daemon),
  logoOpacity(0.5),
  lastButtons(0),
  currentProfile(0)
{
  this->setCursor(QCursor(QPixmap(":/cursor.png"), 0, 0));
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
  QBrush brush3(QColor(100, 100, 100, 255));
  brush2.setStyle(Qt::SolidPattern);
  brush3.setStyle(Qt::SolidPattern);
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
  windowColor.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
  windowColor.setBrush(QPalette::Disabled, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Text, brush3);
  windowColor.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Window, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
  setPalette(windowColor);

  devicebuttons.insert("wiimote.1", WIIMOTE_BTN_1);
  devicebuttons.insert("wiimote.2", WIIMOTE_BTN_2);
  devicebuttons.insert("wiimote.a", WIIMOTE_BTN_A);
  devicebuttons.insert("wiimote.b", WIIMOTE_BTN_B);
  devicebuttons.insert("wiimote.plus", WIIMOTE_BTN_PLUS);
  devicebuttons.insert("wiimote.minus", WIIMOTE_BTN_MINUS);
  devicebuttons.insert("wiimote.home", WIIMOTE_BTN_HOME);
  devicebuttons.insert("wiimote.up", WIIMOTE_BTN_UP);
  devicebuttons.insert("wiimote.down", WIIMOTE_BTN_DOWN);
  devicebuttons.insert("wiimote.left", WIIMOTE_BTN_LEFT);
  devicebuttons.insert("wiimote.right", WIIMOTE_BTN_RIGHT);
  devicebuttons.insert("wiimote.shift.up", WIIMOTE_BTN_SHIFT_UP);
  devicebuttons.insert("wiimote.shift.down", WIIMOTE_BTN_SHIFT_DOWN);
  devicebuttons.insert("wiimote.shift.left", WIIMOTE_BTN_SHIFT_LEFT);
  devicebuttons.insert("wiimote.shift.right", WIIMOTE_BTN_SHIFT_RIGHT);
  devicebuttons.insert("wiimote.shift.forward", WIIMOTE_BTN_SHIFT_FORWARD);
  devicebuttons.insert("wiimote.shift.backward", WIIMOTE_BTN_SHIFT_BACKWARD);
  devicebuttons.insert("wiimote.tilt.front", WIIMOTE_BTN_TILT_FRONT);
  devicebuttons.insert("wiimote.tilt.back", WIIMOTE_BTN_TILT_BACK);
  devicebuttons.insert("wiimote.tilt.left", WIIMOTE_BTN_TILT_LEFT);
  devicebuttons.insert("wiimote.tilt.right", WIIMOTE_BTN_TILT_RIGHT);
  devicebuttons.insert("nunchuk.z", NUNCHUK_BTN_Z);
  devicebuttons.insert("nunchuk.c", NUNCHUK_BTN_C);
  devicebuttons.insert("nunchuk.stick.up", NUNCHUK_BTN_STICK_UP);
  devicebuttons.insert("nunchuk.stick.down", NUNCHUK_BTN_STICK_DOWN);
  devicebuttons.insert("nunchuk.stick.left", NUNCHUK_BTN_STICK_LEFT);
  devicebuttons.insert("nunchuk.stick.right", NUNCHUK_BTN_STICK_RIGHT);
  devicebuttons.insert("nunchuk.shift.up", NUNCHUK_BTN_SHIFT_UP);
  devicebuttons.insert("nunchuk.shift.down", NUNCHUK_BTN_SHIFT_DOWN);
  devicebuttons.insert("nunchuk.shift.left", NUNCHUK_BTN_SHIFT_LEFT);
  devicebuttons.insert("nunchuk.shift.right", NUNCHUK_BTN_SHIFT_RIGHT);
  devicebuttons.insert("nunchuk.shift.forward", NUNCHUK_BTN_SHIFT_FORWARD);
  devicebuttons.insert("nunchuk.shift.backward", NUNCHUK_BTN_SHIFT_BACKWARD);
  devicebuttons.insert("nunchuk.tilt.front", NUNCHUK_BTN_TILT_FRONT);
  devicebuttons.insert("nunchuk.tilt.back", NUNCHUK_BTN_TILT_BACK);
  devicebuttons.insert("nunchuk.tilt.left", NUNCHUK_BTN_TILT_LEFT);
  devicebuttons.insert("nunchuk.tilt.right", NUNCHUK_BTN_TILT_RIGHT);
  devicebuttons.insert("classic.x", CLASSIC_BTN_X);
  devicebuttons.insert("classic.y", CLASSIC_BTN_Y);
  devicebuttons.insert("classic.a", CLASSIC_BTN_A);
  devicebuttons.insert("classic.b", CLASSIC_BTN_B);
  devicebuttons.insert("classic.l", CLASSIC_BTN_L);
  devicebuttons.insert("classic.r", CLASSIC_BTN_R);
  devicebuttons.insert("classic.zl", CLASSIC_BTN_ZL);
  devicebuttons.insert("classic.zr", CLASSIC_BTN_ZR);
  devicebuttons.insert("classic.minus", CLASSIC_BTN_MINUS);
  devicebuttons.insert("classic.plus", CLASSIC_BTN_PLUS);
  devicebuttons.insert("classic.home", CLASSIC_BTN_HOME);
  devicebuttons.insert("classic.up", CLASSIC_BTN_UP);
  devicebuttons.insert("classic.down", CLASSIC_BTN_DOWN);
  devicebuttons.insert("classic.left", CLASSIC_BTN_LEFT);
  devicebuttons.insert("classic.right", CLASSIC_BTN_RIGHT);
  devicebuttons.insert("classic.lstick.up", CLASSIC_BTN_LSTICK_UP);
  devicebuttons.insert("classic.lstick.down", CLASSIC_BTN_LSTICK_DOWN);
  devicebuttons.insert("classic.lstick.left", CLASSIC_BTN_LSTICK_LEFT);
  devicebuttons.insert("classic.lstick.right", CLASSIC_BTN_LSTICK_RIGHT);
  devicebuttons.insert("classic.rstick.up", CLASSIC_BTN_RSTICK_UP);
  devicebuttons.insert("classic.rstick.down", CLASSIC_BTN_RSTICK_DOWN);
  devicebuttons.insert("classic.rstick.left", CLASSIC_BTN_RSTICK_LEFT);
  devicebuttons.insert("classic.rstick.right", CLASSIC_BTN_RSTICK_RIGHT);

  ui->setupUi(this);

  setWindowTitle(QString("Wiimotedev Manager %1.%2.%3").arg(
                   QString::number(WIIMOTEDEV_VERSION_MAJOR),
                   QString::number(WIIMOTEDEV_VERSION_MINOR),
                   QString::number(WIIMOTEDEV_VERSION_PATCH)));
  setWindowIcon(QIcon("/usr/share/qwiimotedev/tray.png"));

  pageMap["Profiles"] = 1;
  pageMap["Connections"] = 0;

  tray = new QSystemTrayIcon(QIcon("/usr/share/qwiimotedev/tray.png"));
  menu = new QMenu();

  QMenu *subMenu;
  connect(menu->addAction("Quit"), SIGNAL(triggered()), this, SLOT(close()));

  tray->setContextMenu(menu);
  tray->show();

  mouseAccY = 0;
  setWindowOpacity(0.92);
  setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Popup);

  profileInterface = new DBusProfileManagerInterface("org.wiimotedev.uinput",  "/profileManager", QDBusConnection::systemBus(), this);
  customJobsInterface = new DBusCustomJobsInterface("org.wiimotedev.uinput",  "/customJobs", QDBusConnection::systemBus(), this);

  ui->profilePage->setEnabled(profileInterface->isValid());

  connect(customJobsInterface, SIGNAL(executeRequest(QStringList)), this, SLOT(executeRequest(QStringList)));
  connect(ui->list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPage(QListWidgetItem*)));

  connectedWithService = false;

  connect(&checkMousePos, SIGNAL(timeout()), this, SLOT(slotMousePosition()));
  checkMousePos.start(100);
  startTimer(3000);
  setTrayTooltip();
  scrollTimer.setInterval(5);
  scrollTimer.start();

  connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));


  profileVerticalLayout = new QVBoxLayout(ui->profileScrollAreaContents);
  profileVerticalLayout->setSpacing(4);
  profileVerticalLayout->setObjectName(QString::fromUtf8("profileVerticalLayout"));
  profileVerticalLayout->setContentsMargins(4, 4, -1, -1);
  profileVerticalLayout->addSpacing(0);

  QDir dir(QDir::homePath() + "/.wiimotedev/profiles");


  QFileInfoList list = dir.entryInfoList();

  foreach (const QFileInfo &file, dir.entryInfoList()) {
    if (!file.isFile())
      continue;

    QSettings settings(file.absoluteFilePath(), QSettings::IniFormat);
    foreach (const QString &key, settings.childGroups()) {
      settings.beginGroup(key);
      if (settings.value("module", QString()).toString().toLower() == "manager") {
        ProfileWidget *widget =
            new ProfileWidget(settings.value("name", QString("not definited")).toString(),
                              file.absoluteFilePath(),
                              settings.value("author", QString("not definited")).toString(),
                              settings.value("version", QString("not definited")).toString(),
                              settings.value("email", QString("not definited")).toString(),
                              profileInterface, ui->profileScrollAreaContents);
        widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        widget->setGraphicsEffect(new QGraphicsOpacityEffect());
        static_cast< QGraphicsOpacityEffect*>(widget->graphicsEffect())->setOpacity(1.0);

        profileVerticalLayout->addWidget(widget, 0, Qt::AlignTop);
        connect(widget, SIGNAL(loadProfile(ProfileWidget*)), this, SLOT(loadProfile(ProfileWidget*)));
        connect(daemon, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), widget, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));
      }
      settings.endGroup();
    }
  }



  deviceVerticalLayout = new QVBoxLayout(ui->connectionScrollAreaContents);
  deviceVerticalLayout->setSpacing(4);
  deviceVerticalLayout->setObjectName(QString::fromUtf8("deviceVerticalLayout"));
  deviceVerticalLayout->setContentsMargins(4, 4, -1, -1);
  deviceVerticalLayout->addSpacing(0);

  {
    QList <quint32> list = daemon->dbusGetWiimoteList();
    foreach (quint32 id, list) {
      QString physicalAddress = daemon->dbusWiimoteGetMacAddress(id);
      QString ident = QString::number(id);
      quint32 exts = ExtNone;
      if (daemon->dbusIsNunchukConnected(id))
        exts = ExtNunchuk;
      if (daemon->dbusIsClassicConnected(id))
        exts = ExtClassic;

      DeviceWidget *widget = new DeviceWidget(physicalAddress, id, exts, ui->connectionScrollAreaContents);
      widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
      widget->setGraphicsEffect(new QGraphicsOpacityEffect());
      connect(daemon, SIGNAL(dbusNunchukPlugged(quint32)), widget, SLOT(dbusNunchukPlugged(quint32)));
      connect(daemon, SIGNAL(dbusNunchukUnplugged(quint32)), widget, SLOT(dbusNunchukUnplugged(quint32)));
      connect(daemon, SIGNAL(dbusClassicControllerPlugged(quint32)), widget, SLOT(dbusClassicControllerPlugged(quint32)));
      connect(daemon, SIGNAL(dbusClassicControllerUnplugged(quint32)), widget, SLOT(dbusClassicControllerUnplugged(quint32)));
      static_cast< QGraphicsOpacityEffect*>(widget->graphicsEffect())->setOpacity(1.0);

      deviceVerticalLayout->addWidget(widget, 0, Qt::AlignTop);
      deviceWidgets[id] = widget;
    }
  }

}

void MainWindow::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event);
  if (connectedWithService != profileInterface->isValid()) {
    ui->profilePage->setEnabled(profileInterface->isValid());
    if (profileInterface->isValid())
      tray->showMessage(QString::fromUtf8("D-Bus"), QString::fromUtf8("Connection with service established"), QSystemTrayIcon::Information, 5000);
      tray->showMessage(QString::fromUtf8("D-Bus"), QString::fromUtf8("Disconnected from service"), QSystemTrayIcon::Information, 5000);
    connectedWithService = profileInterface->isValid();
  }
}

void MainWindow::selectPage(QListWidgetItem *item) {
  if (!(item->flags() & Qt::ItemIsEnabled))
    return;

  if (pageMap.value(item->text(), -1) == -1)
    return;

  ui->stackedWidget->setCurrentIndex(pageMap.value(item->text(), 0));
}


void MainWindow::setTrayTooltip() {
  QString hint;
  QTextStream stream(&hint);
  stream << "Wiimotedev Manager " <<  WIIMOTEDEV_VERSION_MAJOR << '.' <<  WIIMOTEDEV_VERSION_MINOR
         << '.' <<  WIIMOTEDEV_VERSION_PATCH << '\n' << '\n';
  stream << "Services: " << '\n';

  stream << "\torg.wiimotedev.daemon service: ";


  if (daemon->isValid())
    stream << "available"; else
    stream << "not available";
  stream << '\n';

  stream << "\torg.wiimotedev.uinput service: ";

  if (profileInterface->isValid())
    stream << "available"; else
    stream << "not available";
  stream << '\n' << '\n';


  if (daemon->isValid()) {
    QList <quint32> list = daemon->dbusGetWiimoteList();

    stream << "Devices:" << '\n';

    if (!list.count())
      stream << "\tno wiiremotes connected";

    qSort(list.begin(), list.end());
    for (register int i = 0; i < list.count(); ++i) {
      QString address = daemon->dbusWiimoteGetMacAddress(list.at(i));
      stream << "\tbluetooth device " << address << " is registered as " << list.at(i) << " wiiremote";
      storeMacAddresses[list.at(i)] =  address;
      if (i != (list.count() - 1))
        stream << '\n';
    }
  }

  tray->setToolTip(hint);
}

void MainWindow::dbusWiimoteConnected(quint32 id) {
  setTrayTooltip();
  QString hint;
  QTextStream stream(&hint);
  stream << "Bluetooth device " << daemon->dbusWiimoteGetMacAddress(id) << " is registered as " << id << " wiiremote";
  tray->showMessage("Connected", hint);

  QString physicalAddress = daemon->dbusWiimoteGetMacAddress(id);
  QString ident = QString::number(id);
  quint32 exts = ExtNone;
  if (daemon->dbusIsNunchukConnected(id))
    exts = ExtNunchuk;
  if (daemon->dbusIsClassicConnected(id))
    exts = ExtClassic;

  DeviceWidget *widget = new DeviceWidget(physicalAddress, id, exts, ui->connectionScrollAreaContents);
  widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
  widget->setGraphicsEffect(new QGraphicsOpacityEffect());
  static_cast< QGraphicsOpacityEffect*>(widget->graphicsEffect())->setOpacity(1.0);

  deviceVerticalLayout->addWidget(widget, 0, Qt::AlignTop);
  deviceWidgets[id] = widget;
  QGraphicsOpacityEffect *effect;
  widget->setGraphicsEffect(effect = new QGraphicsOpacityEffect);

  QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
  animation->setDuration(1000);
  animation->setEasingCurve(QEasingCurve::Linear);
  animation->setStartValue(qreal(0.0));
  animation->setEndValue(qreal(1.0));
  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  animation->start();


}

void MainWindow::dbusWiimoteDisconnected(quint32 id) {
  setTrayTooltip();
  QString hint;
  QTextStream stream(&hint);
  stream << "Bluetooth device " << storeMacAddresses[id] << " is registered as " << id << " wiiremote";
  tray->showMessage("Disconnected", hint);

  DeviceWidget *widget = deviceWidgets.value(id, 0);
  if (widget) {
    QPropertyAnimation* animation = new QPropertyAnimation(widget->graphicsEffect(), "opacity");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setStartValue(qreal(1.0));
    animation->setEndValue(qreal(0.0));
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(animation, SIGNAL(finished()), widget, SLOT(deleteLater()));
    animation->start();
  }
}


void MainWindow::paintEvent(QPaintEvent *event) {
  QPainter p;

  p.begin(this);

  p.setOpacity(logoOpacity);
  p.drawPixmap(geometry().width() - logo->width() - 10, geometry().height() - logo->height() - 10,
               logo->width(), logo->height(), *logo);
  p.end();


}

void MainWindow::slotMousePosition()
{
  if (isVisible())
  {
      QRect windowPos = calcGeometry();
      if (windowPos != defaultGeometry)
          moveToCenter();
  }
}

QRect MainWindow::calcGeometry()
{
  int screenId = QApplication::desktop()->screenNumber(cursor().pos());

  QRect rect = QApplication::desktop()->screenGeometry(screenId);
  QRect pos;
  pos.setX(rect.x() + ((rect.width()/2 ) - (geometry().width()/2)));
  pos.setY(rect.y() + ((rect.height()/2 ) - (geometry().height()/2)));
  pos.setWidth(geometry().width());
  pos.setHeight(geometry().height());

  return pos;
}

void MainWindow::moveToCenter()
{
  setGeometry(calcGeometry());
  defaultGeometry = calcGeometry();
}

MainWindow::~MainWindow()
{
  tray->hide();
  delete tray;
  delete ui;
}

void MainWindow::executeRequest(QStringList list)
{
  if (!list.count())
      return;

  QString process = list.at(0);
  list.removeAt(0);

  QProcess::execute(process, list);
}

void MainWindow::scroll() {
  double ay = 0;

  if (y < -20)
    ay = y;
  if (y > 20)
    ay = y;

  mouseAccY -= (ay / (384 - 20))*4.0;

  ui->profileScrollArea->verticalScrollBar()->setValue(ui->profileScrollArea->verticalScrollBar()->value() + mouseAccY);
  ui->connectionScrollArea->verticalScrollBar()->setValue(ui->connectionScrollArea->verticalScrollBar()->value() + mouseAccY);
  mouseAccY  -= static_cast< int>(mouseAccY);
}

void MainWindow::dbusVirtualCursorPosition(quint32 id, double x, double y, double size, double angle) {
  if (!isVisible())
    return;

  x = 512.0 - x;
  y = 384.0 - y;

  this->x = x;
  this->y = y;

  if (x > 512)
    x = 512;
  if (x < -512)
    x =-512;
  if (y > 384)
    y = 384;
  if (y < -384)
    y = -384;


  mouseX =  (geometry().width()/2) - (x * (double(geometry().width()) / 1024.0 * (1.3+(double(abs(512-size))/2)/512)));
  mouseY =  (geometry().height()/2) - (y * (double(geometry().height()) / 768.0 * (1.0+(double(abs(386-size))/2)/384)));

  if (mouseX  < 4)
    mouseX  = 4;
  if (mouseY < 4)
    mouseY = 4;
  if (mouseY >  (geometry().height() - 4))
    mouseY = geometry().height() - 4;
  if (mouseX  > (geometry().width() - 4))
    mouseX  = geometry().width() - 4;



  QPixmap pcursor(":/cursor.png");

  angle = angle*(180/3.14);

  if (angle > 180)
    angle = angle-360;

  if (angle > 30)
    angle = 30;
  if (angle < -30)
    angle = -30;

 // qDebug() << angle;
  QMatrix matrix;
  matrix.rotate(-angle);

  setCursor(QCursor(pcursor.transformed(matrix, Qt::SmoothTransformation), 11, 0));



  cursor().setPos(this->mapToParent(QPoint(mouseX, mouseY)));
}

void MainWindow::loadProfile(ProfileWidget *widget) {
  if (currentProfile)
    currentProfile->setInactive();
  (currentProfile = widget)->setActive();
}

void MainWindow::dbusWiimoteGeneralButtons(quint32 id, quint64 value)
{
  buttons = value & WIIMOTE_BUTTON_MASK;

  if ((id != 1) || (buttons == lastButtons)) return;


  if (value & WIIMOTE_BTN_HOME) {
    if (isVisible()) {
      hide();
      if (currentProfile)
        profileInterface->loadProfile(currentProfile->profileFile);
    } else {
      profileInterface->loadProfile("/root/.wiimotedev/uinput.conf");
      moveToCenter();
      ui->stackedWidget->setCurrentIndex(pageMap["Profiles"]);
      show();
    }
  }

  if (value & WIIMOTE_BTN_A) {
    QWidget *widget = childAt(mouseX, mouseY);
    if (widget) {
      qDebug() << widget->objectName();
      if (widget->objectName() == "profileInfo" ||
          widget->objectName() == "profileIcon")
        widget = static_cast< QWidget*>( widget->parent());

      if (widget->objectName() == "profileGroupBox")
        widget = static_cast< QWidget*>( widget->parent());

      if (widget->objectName() == "ProfileWidget") {
        if (currentProfile)
          currentProfile->setInactive();
        (currentProfile = static_cast< ProfileWidget*>( widget))->setActive();
      }
    }
  }
////    QMouseEvent *e = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
////    QPoint p = QPoint(mouseX, mouseY);
////    QWidget *widget = childAt(p);
////    QApplication::postEvent(widget, e);
//  }

////  if ((lastButtons & WIIMOTE_BTN_A) && !(value & WIIMOTE_BTN_A)) {
////    QPoint p = QPoint(mouseX, mouseY);
////    QWidget *widget = childAt(p);

////    QMouseEvent *e = new QMouseEvent(QEvent::MouseButtonRelease, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
////    QApplication::postEvent(widget, e);
////  }

  lastButtons = buttons;
}
