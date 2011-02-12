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

#include "config.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString wiimotedevDirectory = ".wiimotedev/";
const QString profileDirectory = "profiles/";

MainWindow::MainWindow(DBusDeviceEventsInterface *daemon, QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
  profileName(""),
  profileAuthor(""),
  profileEmail(""),
  profileVersion(""),
  profilePath(""),
  logo(new QPixmap(":/logo.png")),
  daemon(daemon),
  logoOpacity(0.5),
  logoGlow(true)
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
//
  ui->setupUi(this);
//  ui->nextProfile->setIcon(QIcon("/usr/share/qwiimotedev/arrow-right.png"));
//  ui->previousProfile->setIcon(QIcon("/usr/share/qwiimotedev/arrow-left.png"));


  setWindowTitle(QString("Wiimotedev Manager %1.%2.%3").arg(
                   QString::number(WIIMOTEDEV_VERSION_MAJOR),
                   QString::number(WIIMOTEDEV_VERSION_MINOR),
                   QString::number(WIIMOTEDEV_VERSION_PATCH)));
  setWindowIcon(QIcon("/usr/share/qwiimotedev/tray.png"));

  tray = new QSystemTrayIcon(QIcon("/usr/share/qwiimotedev/tray.png"));
  menu = new QMenu();

  QMenu *subMenu;
  subMenu = menu->addMenu("Screen");
  subMenu->addAction("At mouse position")->setCheckable(true);
  for (register int i = 0; i < QApplication::desktop()->screenCount(); ++i)
    subMenu->addAction(QString("Screen %1").arg(i + 1))->setCheckable(true);

  connect(menu->addAction("Quit"), SIGNAL(triggered()), this, SLOT(close()));

  tray->setContextMenu(menu);
  tray->show();

  QStringList directoryList;

  directoryList << QDir::homePath() + QString::fromUtf8("/");
  directoryList << directoryList.at(0) + wiimotedevDirectory;
  directoryList << directoryList.at(1) + profileDirectory;

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

  QDir dir(QChar('/'));

  for (register qint8 i = 0; i < directoryList.count(); ++i)
    if (!dir.exists(directoryList.at(i)))
      dir.mkdir(directoryList.at(i));

  dir.setPath(directoryList.at(2));

  QFileInfoList list = dir.entryInfoList();

  foreach (const QFileInfo &file, dir.entryInfoList()) {
    if (!file.isFile() || (file.suffix().toLower() != "ini"))
      continue;

    QSettings settings(file.absoluteFilePath(), QSettings::IniFormat);
    Profile profile;
    profile.name = settings.value("name", "undefined").toString();
    profile.author = settings.value("author", "undefined").toString();
    profile.email = settings.value("email", "undefined").toString();
    profile.version = settings.value("version", "undefined").toString();
    profile.path = file.absoluteFilePath();
    uinputProfileList << profile;
  }

  windowAtMousePosition = true;
  moveToCenter();

  index = 0;
//  ui->previousProfile->setEnabled(false);

  if (!uinputProfileList.count());
//    ui->nextProfile->setEnabled(false);

  if (profileList.count() > 0) {
//    ui->nextProfile->setEnabled(false);
//    showProfile(profileList.at(0));
  }

//  ui->nextProfile->setVisible(false);
//  ui->previousProfile->setVisible(false);

//  connect(ui->nextProfile, SIGNAL(clicked()), this, SLOT(nextProfile()));
//  connect(ui->previousProfile, SIGNAL(clicked()), this, SLOT(previousProfile()));


  profileInterface = new DBusProfileManagerInterface("org.wiimotedev.uinput",
                                                     "/profileManager",
                                                     QDBusConnection::systemBus(), this);

  customJobsInterface = new DBusCustomJobsInterface("org.wiimotedev.uinput",
                                                    "/customJobs",
                                                    QDBusConnection::systemBus(), this);



  connect(customJobsInterface, SIGNAL(executeRequest(QStringList)), this, SLOT(executeRequest(QStringList)));

  connectedWithService = false;

  connect(&checkMousePos, SIGNAL(timeout()), this, SLOT(slotMousePosition()));
  checkMousePos.start(100);
  startTimer(1000);
  setTrayTooltip();



  //for (register int i = 0; i < ui->connections->columnCount(); ++i) {
  //  QHeaderView *view = ui->connections->header();
  //  view->setResizeMode(i, QHeaderView::ResizeToContents);
  //  view->setMinimumSectionSize(50);
 // }


  deviceVerticalLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
  deviceVerticalLayout->setSpacing(6);
  deviceVerticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
  deviceVerticalLayout->setContentsMargins(4, 4, -1, -1);
  deviceVerticalLayout->addSpacing(10);

  {
    QList <quint32> list = daemon->dbusGetWiimoteList();
    foreach (quint32 id, list) {
      QString physicalAddress = daemon->dbusWiimoteGetMacAddress(id);
      QString ident = QString::number(id);
      QString extensions = "none";
      if (daemon->dbusIsNunchukConnected(id))
        extensions = "Nunchuk"; else
      if (daemon->dbusIsClassicConnected(id))
        extensions = "Classic Controller";

      DeviceWidget *widget = new DeviceWidget(ui->scrollAreaWidgetContents);
      widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
      widget->ui->physicalAddress->setText(QString("Physical address: %1").arg(physicalAddress));
      widget->ui->id->setText(QString("Ident number: %1").arg(ident));
      widget->ui->extensions->setText(QString("Extensions: %1").arg(extensions));
      widget->setGraphicsEffect(new QGraphicsOpacityEffect());
      static_cast< QGraphicsOpacityEffect*>(widget->graphicsEffect())->setOpacity(1.0);

      deviceVerticalLayout->addWidget(widget, 0, Qt::AlignTop);
      deviceWidgets[id] = widget;
    }
  }

 // deviceVerticalLayout->addStretch(0);




}


void MainWindow::setTrayTooltip() {
  QString hint;
  QTextStream stream(&hint);
  stream << "Wiimotedev Manager " <<  WIIMOTEDEV_VERSION_MAJOR << '.' <<  WIIMOTEDEV_VERSION_MINOR
         << '.' <<  WIIMOTEDEV_VERSION_PATCH << '\n' << '\n';
  stream << "Services: " << '\n';

  stream << "\torg.wiimotedev.daemon service: ";


  if (daemon->isValid())
    stream << "connected"; else
    stream << "waiting for connection";
  stream << '\n';

  stream << "\torg.wiimotedev.uinput service: ";

  if (profileInterface->isValid())
    stream << "connected"; else
    stream << "waiting for connection";
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
  QString extensions = "none";
  if (daemon->dbusIsNunchukConnected(id))
    extensions = "Nunchuk"; else
  if (daemon->dbusIsClassicConnected(id))
    extensions = "Classic Controller";


  DeviceWidget *widget = new DeviceWidget(ui->scrollAreaWidgetContents);
  widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
  widget->ui->physicalAddress->setText(QString("Physical address: %1").arg(physicalAddress));
  widget->ui->id->setText(QString("Ident number: %1").arg(ident));
  widget->ui->extensions->setText(QString("Extensions: %1").arg(extensions));

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
  int screenId = 0;

  for (register int i = QApplication::desktop()->screenCount(); i > 0; --i)
  {
      if (QApplication::desktop()->screenGeometry(i - 1).x() <= QCursor::pos().x())
      {
          screenId = i - 1;
          break;
      }
  }

  QRect desktopRect = QApplication::desktop()->screenGeometry(screenId);
  QRect windowPos;

  windowPos.setX(desktopRect.x() + ((desktopRect.width() / 2) - (geometry().width() / 2)));
  windowPos.setY(desktopRect.y() + ((desktopRect.height() / 2) - (geometry().height() / 2)));
  windowPos.setWidth(geometry().width());
  windowPos.setHeight(geometry().height());

  return windowPos;
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

void MainWindow::dbusWiimoteGeneralButtons(quint32 id, quint64 value)
{
  buttons = value & WIIMOTE_BUTTON_MASK;

  if ((id != 1) || (buttons == lastButtons)) return;
  lastButtons = buttons;

  if (value & WIIMOTE_BTN_HOME) {
    if (isVisible()) {
      hide();
    } else
    {
      moveToCenter();
      show();
    }
  }

  if (isVisible())
  {
//      if ((value & WIIMOTE_BTN_B) || (value & WIIMOTE_BTN_PLUS))
//          ui->stackedWidget->setCurrentIndex(1); else
//          ui->stackedWidget->setCurrentIndex(0);
      if ((value & WIIMOTE_BTN_LEFT) || (value & WIIMOTE_BTN_UP)) previousProfile();
      if ((value & WIIMOTE_BTN_RIGHT) || (value & WIIMOTE_BTN_DOWN)) nextProfile();
      if ((value & WIIMOTE_BTN_A) || (value & WIIMOTE_BTN_2))
      {
          profileInterface->loadProfile(profileList.at(index - 1).ProfilePath);
          setVisible(false);
      }
  }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event);

  if (logoGlow) {
    logoOpacity += 0.001;

    if (logoOpacity > 0.5)
      logoGlow = false;
  } else
  {
    logoOpacity -= 0.001;

    if (logoOpacity < 0.1)
      logoGlow = true;
  }

  repaint(QRect(geometry().width() - logo->width() - 10, geometry().height() - logo->height() - 10,
          logo->width(), logo->height()));



  if (connectedWithService != profileInterface->isValid())
  {
      if (profileInterface->isValid())
          tray->showMessage(QString::fromUtf8("D-Bus"), QString::fromUtf8("Connection with service established"), QSystemTrayIcon::Information, 5000); else
          tray->showMessage(QString::fromUtf8("D-Bus"), QString::fromUtf8("Disconnected from service"), QSystemTrayIcon::Information, 5000);
      connectedWithService = profileInterface->isValid();
  }
}

void MainWindow::nextProfile()
{
  if (index < profileList.count()) {
      index++;
//      if (index == profileList.count())
//          ui->nextProfile->setEnabled(false);
//      if (index > 1)
//          ui->previousProfile->setEnabled(true);
      showProfile(profileList.at(index - 1));
  }
}

void MainWindow::previousProfile()
{
  if (index > 1)
  {
      index--;
//      if (index == 1)
//          ui->previousProfile->setEnabled(false);
//      if (index < profileList.count())
//          ui->nextProfile->setEnabled(true);
      showProfile(profileList.at(index - 1));
  }
}

void MainWindow::showProfile(struct ProfileItem item)
{
//  ui->profileName->setText(item.ProfileName);
//  ui->profileVersion->setText(item.ProfileVersion);
//  ui->profilePath->setText(item.ProfilePath);
//  ui->profileEmail->setText(item.ProfileEmail);
//  ui->profileAuthor->setText(item.ProfileAuthor);
//  ui->largeProfileName->setText(item.ProfileName);
  this->repaint();
  if (geometry() != defaultGeometry) moveToCenter();
}

