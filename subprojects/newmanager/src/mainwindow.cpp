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


#include "mainwindow.h"

#include <QtOpenGL>

const qint32 emptyspace = 20;



MainWindow::MainWindow(DBusDeviceEventsInterface *device):
  device(device),
  styledBackground(new QPixmap(":/styled_bg.png")),
  lastFocusedProfile(0),
  lastActivedProfile(0),
  lastFocusedMenu(0),
  lastActivedMenu(0),
  enabledPixmap(new QPixmap(":/enabled.png")),
  disabledPixmap(new QPixmap(":/disabled.png")),
  font8(new QFont("Luxi Serif Bold Oblique", 8, QFont::Bold)),
  font16(new QFont("Luxi Serif Bold Oblique", 16, QFont::Bold)),
  font24(new QFont("Luxi Serif Bold Oblique", 24, QFont::Bold)),
  font32(new QFont("Luxi Serif Bold Oblique", 32, QFont::Bold)),
  font48(new QFont("Luxi Serif Bold Oblique", 48, QFont::Bold)),
  font64(new QFont("Luxi Serif Bold Oblique", 64, QFont::Bold)),
  cursorPixmap(new QPixmap(":/cursor.png")),
  currentCoverIndex(0)
{
  setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));

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

  setBackgroundBrush(Qt::black);
  setWindowFlags(Qt::WindowStaysOnTopHint);
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setCacheMode(QGraphicsView::CacheBackground);

  setCursor(QCursor(Qt::BlankCursor));
  setMouseTracking(true);
  setGeometry(calculateWindowSize());
  setScene(new QGraphicsScene(0, 0, geometry().width(), geometry().height()));

  scene()->setBackgroundBrush(Qt::white);


  loadProfiles();

  QList <quint32> list = device->dbusGetWiimoteList();
  foreach (quint32 id, list) {
    QString ext = "none";
    if (device->dbusIsClassicConnected(id))
      ext = "Classic Controller";
    if (device->dbusIsNunchukConnected(id))
      ext = "Nunchuk";

    for (int i = 0; i < 12; ++i) {
    GraphicsButton *item = new GraphicsButton();
    item->setWidth(geometry().width());
    item->setHeight(50);
    item->setFont(*font8);
    item->setText(QString("Wiiremote: %1; MAC: %2; Ext: %3").arg(QString::number(id), device->dbusWiimoteGetMacAddress(id), ext));
    item->setIconFromPath(":/connections.png");
    item->setX(geometry().width());
    item->setY(0);
    item->setAlignFlags(AlignLeft | AlignHCenter);
    scene()->addItem(item);
    connections << item;}



  }

//  QDir dir(QDir::homePath() + "/.wiimotedev/profiles");


//  QFileInfoList list = dir.entryInfoList();

//  int i = 0;
//  foreach (const QFileInfo &file, dir.entryInfoList()) {
//    if (!file.isDir())
//      continue;
//    QSettings settings(file.absoluteFilePath() + "/uinput.ini", QSettings::IniFormat);
//    foreach (const QString &key, settings.childGroups()) {
//      settings.beginGroup(key);
//      if (settings.value("module", QString()).toString().toLower() == "manager") {
//        GraphicsButton *item = new GraphicsButton();
//        item->setWidth(geometry().width());
//        item->setHeight(100);
//        item->setFont(QFont("Luxi Serif Bold Oblique", 24, QFont::Bold));
//        item->setText(settings.value("name", QString()).toString());
//        item->setIcon(QPixmap(file.absoluteFilePath() + "/icon.png"));
//        item->setX(geometry().width());
//        item->setY(0);
//        item->setAlignFlags(AlignLeft | AlignHCenter);
//        QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
//        animation->setDuration(1000);
//        animation->setEasingCurve(QEasingCurve::OutQuart);
//        animation->setStartValue(item->pos());
//        animation->setEndValue(QPoint(290, (100 * i + (25*i))));
//        animation->start();
//        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

//        GraphicsProfileCover *item2 = new GraphicsProfileCover();

//        item2->setWidth(geometry().width() - 264);
//        item2->setHeight(height());
//        item2->setX(264 + ((geometry().width() - 264)*i));
//        item2->setY(0);
//        if (QFile::exists(file.absoluteFilePath() + "/cover.png"))
//          item2->setCover(QPixmap(file.absoluteFilePath() + "/cover.png")); else
//          item2->setCover(QPixmap(":/nocover.png"));
//        item2->setFont(*font24);
//        item2->setText(settings.value("name", QString()).toString());
//        item2->setOpacity(0.2);

//        i++;
//        profiles << item;
//        covers << item2;
//        scene()->addItem(item);
//        item->setGroupId(1);
//      }
//      settings.endGroup();
//    }
//  }

  //covers.at(currentCoverIndex)->setOpacity(1.0);



  profileRunning = new QGraphicsPixmapItemPlus();
  profileRunning->setPixmap(QPixmap(":/media-optical.png"));
  profileRunning->setTransformOriginPoint(double(profileRunning->boundingRect().width())/2.0, double(profileRunning->boundingRect().height())/2.0);
  profileRunning->setTransformationMode(Qt::SmoothTransformation);

  profileRunning->setScale(1.0);
  profileRunning->setZValue(-1000);

  menuBackground = new GraphicsManagerMenu();
  scene()->addItem(menuBackground);


  profileButton = new GraphicsButton();
  profileButton->setX(33);
  profileButton->setY(50);
  profileButton->setWidth(226);
  profileButton->setHeight(50);

  profileButton->setFont(*font16);
  profileButton->setText("Profiles");
  profileButton->setIconFromPath(":/profile.png");
  scene()->addItem(profileButton);

  connect(profileButton, SIGNAL(enter()), this, SLOT(showProfiles()));

  connect(profileButton, SIGNAL(clicked()), this, SLOT(showProfilesPage()));
  connect(profileButton, SIGNAL(clicked()), this, SLOT(hideCoversPage()));
  connect(profileButton, SIGNAL(clicked()), this, SLOT(hideConnectionsPage()));
  connect(profileButton, SIGNAL(clicked()), this, SLOT(hidePreferncesPage()));


  coverButton = new GraphicsButton();
  coverButton->setX(33);
  coverButton->setY(140);
  coverButton->setActive(false);
  coverButton->setWidth(226);
  coverButton->setHeight(50);
  coverButton->setFont(*font16);
  coverButton->setText("Covers");
  coverButton->setIconFromPath(":/covers.png");
  scene()->addItem(coverButton);
  connect(coverButton, SIGNAL(enter()), this, SLOT(showCovers()));


  connect(coverButton, SIGNAL(clicked()), this, SLOT(hideProfilesPage()));
  connect(coverButton, SIGNAL(clicked()), this, SLOT(showCoversPage()));
  connect(coverButton, SIGNAL(clicked()), this, SLOT(hidePreferencesPage()));
  connect(coverButton, SIGNAL(clicked()), this, SLOT(hideConnectionsPage()));

  preferencesButton = new GraphicsButton();
  preferencesButton->setX(33);
  preferencesButton->setY(230);
  preferencesButton->setActive(false);
  preferencesButton->setWidth(226);
  preferencesButton->setHeight(50);
  preferencesButton->setFont(*font16);
  preferencesButton->setText("Preferences");
  preferencesButton->setIconFromPath(":/preferences.png");
  scene()->addItem(preferencesButton);

  connect(preferencesButton, SIGNAL(clicked()), this, SLOT(hideProfilesPage()));
  connect(preferencesButton, SIGNAL(clicked()), this, SLOT(hideCoversPage()));
  connect(preferencesButton, SIGNAL(clicked()), this, SLOT(showPreferencesPage()));
  connect(preferencesButton, SIGNAL(clicked()), this, SLOT(hideConnectionsPage()));


  connectionButton = new GraphicsButton();
  connectionButton->setX(33);
  connectionButton->setY(320);
  connectionButton->setWidth(226);
  connectionButton->setHeight(50);
  connectionButton->setFont(*font16);
  connectionButton->setText("Connections");
  connectionButton->setIconFromPath(":/connections.png");
  connectionButton->setActive(false);
  scene()->addItem(connectionButton);


  connect(connectionButton, SIGNAL(clicked()), this, SLOT(hideProfilesPage()));
  connect(connectionButton, SIGNAL(clicked()), this, SLOT(hideCoversPage()));
  connect(connectionButton, SIGNAL(clicked()), this, SLOT(hidePreferencesPage()));
  connect(connectionButton, SIGNAL(clicked()), this, SLOT(showConnectionsPage()));


  prefAutoHideMenu = new GraphicsCheckbox();
  prefAutoHideMenu->setX(300);
  prefAutoHideMenu->setY(40);
  prefAutoHideMenu->setWidth(550);
  prefAutoHideMenu->setHeight(40);
  prefAutoHideMenu->setFont(*font8);
  prefAutoHideMenu->setText("Enable auto-hide feature for main menu");

  prefAutoHideMenu->setActive(false);
  scene()->addItem(prefAutoHideMenu);

  prefDisplayBatteryStatus = new GraphicsCheckbox();
  prefDisplayBatteryStatus->setX(300);
  prefDisplayBatteryStatus->setY(90);
  prefDisplayBatteryStatus->setWidth(550);
  prefDisplayBatteryStatus->setHeight(40);
  prefDisplayBatteryStatus->setFont(*font8);
  prefDisplayBatteryStatus->setText("Display additional battery status");

  prefDisplayBatteryStatus->setActive(false);
  scene()->addItem(prefDisplayBatteryStatus);

  prefDisplayBatteryStatus = new GraphicsCheckbox();
  prefDisplayBatteryStatus->setX(300);
  prefDisplayBatteryStatus->setY(140);
  prefDisplayBatteryStatus->setWidth(550);
  prefDisplayBatteryStatus->setHeight(40);
  prefDisplayBatteryStatus->setFont(*font8);
  prefDisplayBatteryStatus->setText("Display additional service status");
  prefDisplayBatteryStatus->setActive(false);
  scene()->addItem(prefDisplayBatteryStatus);

  QList < QGraphicsItem*> menuComponents;


//  QPropertyAnimation *animation = new QPropertyAnimation(menuGroup, "pos");
//  animation->setDuration(1000);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(QPoint(-260, 0));
//  animation->setEndValue(QPoint(0, 0));
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));



  cursorHandle = scene()->addPixmap(QPixmap(":/cursor.png"));
  cursorHandle->setTransformOriginPoint(7, 2);
  cursorHandle->setTransformationMode(Qt::SmoothTransformation);
  confineCursor();

  rumble.setFrameRange(1, 0);
  rumble.setDuration(25);
  connect(&rumble, SIGNAL(frameChanged(int)), this, SLOT(setRumbleStatus(int)));

  startTimer(10);
  visibleMenu = true;
}

void MainWindow::confineCursor() {
  cursorHandle->setZValue(300);
  cursorHandle->setX((geometry().width() / 2));
  cursorHandle->setY((geometry().height() / 2));
}

MainWindow::~MainWindow()
{  
}


void MainWindow::profileButtonClicked() {
  showProfilesPage();

}

void MainWindow::coverButtonClicked() {
}

void MainWindow::preferenceButtonClicked() {
}

void MainWindow::connectionButtonClicked() {
}

void MainWindow::loadProfiles() {
  QDir dir(QDir::homePath() + "/.wiimotedev/profiles");
  QFileInfoList list = dir.entryInfoList();

  int i = 0;
  foreach (const QFileInfo &file, dir.entryInfoList()) {
    if (!file.isDir())
      continue;
    QSettings settings(file.absoluteFilePath() + "/uinput.ini", QSettings::IniFormat);
    foreach (const QString &key, settings.childGroups()) {
      settings.beginGroup(key);
      if (settings.value("module", QString()).toString().toLower() == "manager") {
        GraphicsButton *item = new GraphicsButton();
        item->setWidth(geometry().width());
        item->setHeight(100);
        item->setFont(*font24);
        item->setText(settings.value("name", QString()).toString());
        item->setIconFromPath(file.absoluteFilePath() + "/icon.png");
        item->setX(geometry().width());
        item->setY(0);
        item->setAlignFlags(AlignLeft | AlignHCenter);

        GraphicsProfileCover *item2 = new GraphicsProfileCover();

        item2->setWidth(geometry().width() - 264);
        item2->setHeight(height());
        item2->setX(264 + ((geometry().width() - 264)*i));
        item2->setY(0);
        if (QFile::exists(file.absoluteFilePath() + "/cover.png"))
          item2->setCover(QPixmap(file.absoluteFilePath() + "/cover.png")); else
          item2->setCover(QPixmap(":/nocover.png"));
        item2->setFont(*font24);
        item2->setText(settings.value("name", QString()).toString());
        item2->setOpacity(0);


        covers << item2;
        scene()->addItem(item2);

        profiles << item;
        scene()->addItem(item);
        item->setGroupId(1);
        i++;


      }
      settings.endGroup();
    }
  }

  showProfilesPage();
}

void MainWindow::showProfilesPage() {
  for (register int i = 0; i < profiles.count(); ++i) {
    QPropertyAnimation *animation = new QPropertyAnimation(profiles.at(i), "pos");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->setStartValue(profiles.at(i)->pos());
    animation->setEndValue(QPoint(290, (100 * i + (25*i))));
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  }
}

void MainWindow::hideProfilesPage() {
  for (register int i = 0; i < profiles.count(); ++i) {
    QPropertyAnimation *animation = new QPropertyAnimation(profiles.at(i), "pos");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->setStartValue(profiles.at(i)->pos());
    animation->setEndValue(QPoint(geometry().width(), -profiles.at(i)->boundingRect().height()));
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  }
}

void MainWindow::showCoversPage() {
  for (register int i = 0; i < covers.count(); ++i) {
    QPropertyAnimation *animation = new QPropertyAnimation(covers.at(i), "opacity");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setStartValue(covers.at(i)->opacity());
    animation->setEndValue(1.0);
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  }
}

void MainWindow::hideCoversPage() {
  for (register int i = 0; i < profiles.count(); ++i) {
    QPropertyAnimation *animation = new QPropertyAnimation(covers.at(i), "opacity");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setStartValue(covers.at(i)->opacity());
    animation->setEndValue(0.0);
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  }
}

void MainWindow::showConnectionsPage() {
  for (register int i = 0; i < connections.count(); ++i) {
    QPropertyAnimation *animation = new QPropertyAnimation(connections.at(i), "pos");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->setStartValue(connections.at(i)->pos());
    animation->setEndValue(QPoint(290, (50 * i + (25*i))));
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  }
}

void MainWindow::hideConnectionsPage() {
  for (register int i = 0; i < connections.count(); ++i) {
    QPropertyAnimation *animation = new QPropertyAnimation(connections.at(i), "pos");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->setStartValue(connections.at(i)->pos());
    animation->setEndValue(QPoint(geometry().width(), -connections.at(i)->boundingRect().height()));
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
  }
}

void MainWindow::showPreferencesPage() {

}

void MainWindow::hidePreferencesPage() {

}

QRect MainWindow::calculateWindowSize() {
  int screenId = QApplication::desktop()->screenNumber(cursor().pos());

  QRect rect = QApplication::desktop()->screenGeometry(screenId);
  int width = rect.width() * 0.75;
  int height = rect.height() * 0.50;
  QRect pos;

  pos.setX(rect.x() + ((rect.width()/2 ) - (width/2)));
  pos.setY(rect.y() + ((rect.height()/2 ) - (height/2)));

  pos.setWidth(width);
  pos.setHeight(height);

  return pos;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
  setSceneRect(0, 0, geometry().width(), geometry().height());

  int i = 0;
  foreach (GraphicsProfileCover* item, covers) {
    item->setWidth(geometry().width());
    item->setHeight(height());
    item->setX(((geometry().width())*i));
    item->setY(0);
    item->rescale();
    i++;
  }

  menuBackground->setWidth(290);
  menuBackground->setHeight(geometry().height());

  profileRunning->setPos(400, geometry().height() - 400);

}

quint32 barsize = 60.0;

void MainWindow::showProfiles() {
//  profileGroup->setVisible(true);
//  coverGroup->setVisible(false);
}

void MainWindow::showCovers() {
//  profileGroup->setVisible(false);
//  coverGroup->setVisible(true);
}

void MainWindow::drawBackground(QPainter *painter, const QRectF &rect) {
//  painter->setOpacity(1);
//  painter->setPen(Qt::NoPen);
//  painter->setBrush(QColor(65, 65, 65, 255));
//  painter->drawRect(260, 0, 1, geometry().height());
//  painter->setBrush(QColor(45, 45, 45, 255));
//  painter->drawRect(261, 0, 2, geometry().height());
//  painter->setBrush(QColor(65, 65, 65, 255));
//  painter->drawRect(263, 0, 1, geometry().height());

  painter->drawPixmap(rect.width() - styledBackground->width(),
                      rect.height() - styledBackground->height(), styledBackground->width(),
                      styledBackground->height(), *styledBackground);
}

void MainWindow::drawForeground(QPainter *painter, const QRectF &rect){
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::black);

  for (register int i = 0; i < barsize; ++i) {
    painter->setOpacity((barsize-i)/double(barsize));
    painter->drawRect(QRect(0, i, geometry().width(), 1));
  }

  for (register int i = 0; i < barsize; ++i) {
    painter->setOpacity((barsize-i)/double(barsize));
    painter->drawRect(QRect(0, geometry().height() - i, geometry().width(), 1));
  }


  painter->setOpacity(0.5);
  painter->setBrush(Qt::black);

  painter->drawRect(QRect(20, geometry().height() - 95, 230, 80));

  painter->setOpacity(1.0);

  painter->setPen(Qt::white);
  painter->setFont(*font8);
  painter->drawText(30, geometry().height()-80, "Services status");

  painter->setPen(Qt::darkGray);
  painter->setFont(*font8);
  painter->drawText(60, geometry().height()-60, "wiimotedev-daemon: avaliable");
  painter->drawPixmap(40, geometry().height()-71, *enabledPixmap);

  painter->drawText(60, geometry().height()-40, "wiimotedev-uinput: avaliable");
  painter->drawPixmap(40, geometry().height()-51, *enabledPixmap);
  painter->setPen(Qt::NoPen);
}



void MainWindow::dbusWiimoteGeneralButtons(quint32 id, quint64 value) {
  if (id != 1)
    return;

  if (value & WIIMOTE_BTN_HOME)
    setVisible(!isVisible());

  if (value & WIIMOTE_BTN_A) {
    if (lastFocusedProfile) {
      if (lastActivedProfile == lastFocusedProfile) {
        lastActivedProfile->setFocusState(false);
        lastActivedProfile = 0;
      } else {
        if (lastActivedProfile)
          lastActivedProfile->setFocusState(false);
        lastActivedProfile = lastFocusedProfile;
        lastActivedProfile->setFocusState(true);
      }
    }
  }

  if (value & WIIMOTE_BTN_1) {
//    profileGroup->setVisible(true);
//    coverGroup->setVisible(false);
  }

  if (value & WIIMOTE_BTN_2) {
//    profileGroup->setVisible(false);
//    coverGroup->setVisible(true);
  }

  if (value & WIIMOTE_BTN_LEFT) {
    prevCover();
  }

  if (value & WIIMOTE_BTN_RIGHT) {
    nextCover();
  }



  lastButtons = value;
}



void MainWindow::mousePressEvent (QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);

//  if (lastFocusedProfile) {
//    if (lastFocusedProfile) {
//      if (lastActivedProfile == lastFocusedProfile) {
//        lastActivedProfile->setFocusState(false);
//        lastActivedProfile = 0;
//      } else {
//        if (lastActivedProfile)
//          lastActivedProfile->setFocusState(false);
//        lastActivedProfile = lastFocusedProfile;
//        lastActivedProfile->setFocusState(true);
//      }
//    }
//  }

//  if (lastFocusedMenu) {
//     if (lastFocusedMenu) {
//       if (lastActivedMenu == lastFocusedMenu) {
//         lastActivedMenu->setFocusState(false);
//         lastActivedMenu = 0;
//       } else {
//         if (lastActivedMenu)
//           lastActivedMenu->setFocusState(false);
//         lastActivedMenu = lastFocusedMenu;
//         lastActivedMenu->setFocusState(true);
//         lastActivedMenu->press();
//       }
//     }
//   }
}

void MainWindow::mouseReleaseEvent (QMouseEvent *event)
{
QGraphicsView::mouseReleaseEvent(event);
}

void MainWindow::timerEvent(QTimerEvent *event) {
//  profileGroup->setY(profileGroup->y() + mouseAccY);

//  if (profileGroup->y() < ((profiles.count() * -110) + geometry().height() - int(barsize/2)))
//    profileGroup->setY((profiles.count() * -110) + geometry().height() - int(barsize/2));

//  if (profileGroup->y() > (barsize/2))
//    profileGroup->setY(barsize/2);

  //profileRunning->setRotation(profileRunning->rotation()+1);
}


void MainWindow::showMenu(int duration) {
//  QPropertyAnimation *animation = new QPropertyAnimation(menuGroup, "pos");
//  animation->setDuration(duration);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(QPoint(menuGroup->pos().x(), 0));
//  animation->setEndValue(QPoint(-260, 0));
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

//  animation = new QPropertyAnimation(profileGroup, "x");
//  animation->setDuration(duration);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(profileGroup->pos().x());
//  animation->setEndValue(-264);
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
}

void MainWindow::hideMenu(int duration) {
//  QPropertyAnimation *animation = new QPropertyAnimation(menuGroup, "pos");
//  animation->setDuration(duration);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(QPoint(menuGroup->pos().x(), 0));
//  animation->setEndValue(QPoint(0, 0));
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
//  visibleMenu = true;

//  animation = new QPropertyAnimation(profileGroup, "x");
//  animation->setDuration(duration);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(profileGroup->pos().x());
//  animation->setEndValue(0);
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
}

void MainWindow::setCoverIndex(int index) {

}

void MainWindow::nextCover() {
//  currentCoverIndex++;

//  if (currentCoverIndex >= covers.count()) {
//    currentCoverIndex = 0;
//    coverGroup->setPos(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup->pos().y()));
//  }
//  rumble.start();

//  QPropertyAnimation *animation = new QPropertyAnimation(coverGroup, "pos");
//  animation->setDuration(250);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(coverGroup->pos());
//  animation->setEndValue(QPoint(-(currentCoverIndex*(geometry().width())), coverGroup->pos().y()));
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

//  for (register int i = 0; i < covers.count(); ++i) {
//    animation = new QPropertyAnimation(covers.at(i), "opacity");
//    animation->setDuration(500);
//    animation->setEasingCurve(QEasingCurve::Linear);
//    animation->setStartValue(covers.at(i)->opacity());
//    animation->setEndValue((i == currentCoverIndex) ? 1 : 0.0);
//    animation->start();
//    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
//  }
}

void MainWindow::prevCover() {
//  currentCoverIndex--;
//  if (currentCoverIndex < 0) {
//    currentCoverIndex = covers.count() - 1;
//    coverGroup->setPos(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup->pos().y()));
//  }
//  rumble.start();

//  QPropertyAnimation *animation = new QPropertyAnimation(coverGroup, "pos");
//  animation->setDuration(250);
//  animation->setEasingCurve(QEasingCurve::OutQuart);
//  animation->setStartValue(coverGroup->pos());
//  animation->setEndValue(QPoint(-(currentCoverIndex*(geometry().width())), coverGroup->pos().y()));
//  animation->start();
//  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

//  for (register int i = 0; i < covers.count(); ++i) {
//    QPropertyAnimation *animation = new QPropertyAnimation(covers.at(i), "opacity");
//    animation->setDuration(250);
//    animation->setEasingCurve(QEasingCurve::Linear);
//    animation->setStartValue(covers.at(i)->opacity());
//    animation->setEndValue((i == currentCoverIndex) ? 1 : 0.0);
//    animation->start();
//    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
//  }
}



void MainWindow::mouseMoveEvent (QMouseEvent *event) {
  if (event->button() == Qt::XButton1)
    return;

  mouseX = event->x();
  mouseY = event->y();

   double y = ((geometry().height()/2) - event->y())*(512/double(geometry().height()));

  if (mouseX > 260)
    mouseAccY = y * 32/512; else
    mouseAccY = 0;

  moveCursor();
  rotateCursor(0);
  scaleCursor(1.0);
  QGraphicsView::mouseMoveEvent(event);
}

void MainWindow::dbusVirtualCursorPosition(quint32 id, double x, double y, double size, double angle) {
  if (id != 1)
    return;

  if (!isVisible())
    return;

  mouseX = (geometry().width()/2) - (x * (double(geometry().width()) / 1024.0 * (1.3+(double(abs(512-size))/2)/512)));
  mouseY = (geometry().height()/2) - (y * (double(geometry().height()) / 768.0 * (1.0+(double(abs(386-size))/2)/384)));

  if (mouseX > 260)
    mouseAccY = y * 32/512; else
    mouseAccY = 0;

  moveCursor();
  scaleCursor(((1024-size)/500)*1.3);
  rotateCursor(-angle*180/M_PI);

  QMouseEvent *event = new QMouseEvent(QMouseEvent::MouseMove, QPoint(mouseX, mouseY), Qt::XButton1, Qt::NoButton, Qt::NoModifier);
  QGraphicsView::mouseMoveEvent(event);
}

void MainWindow::setRumbleStatus(int status) {
  device->dbusWiimoteSetRumbleStatus(1, status);
}

void MainWindow::rotateCursor(double angle) {
  cursorHandle->setRotation(angle);
}

void MainWindow::scaleCursor(double scale) {
  cursorHandle->setScale(scale);
}

void MainWindow::moveCursor() {
  if ((mouseX > 264)) {
    if (!visibleMenu) {
      showMenu(750);
      visibleMenu = true;
    }
  } else if (mouseX < 4) {
    if (lastFocusedProfile)
      lastFocusedProfile->setActiveState(false);
    lastFocusedProfile = 0;
    if (visibleMenu) {
      hideMenu(750);
      visibleMenu = false;
    }
  }


//  QGraphicsItem *obj = scene()->itemAt(mouseX, mouseY);
//  if (obj) {
//    if (obj->parentItem() == reinterpret_cast< QGraphicsItemGroupPlus*>(&profileGroup)) {
//      if (lastFocusedProfile != static_cast< GraphicsButton*>( obj) ) {
//        rumble.start();
//        if (lastFocusedProfile)
//          lastFocusedProfile->setActiveState(false);
//        lastFocusedProfile = static_cast< GraphicsButton*>( obj);
//        lastFocusedProfile->setActiveState(true);
//      }
//    }

//    if (obj->parentItem() == reinterpret_cast< QGraphicsItemGroupPlus*>(&menuGroup)) {
//      if (lastFocusedMenu != static_cast< GraphicsButton*>( obj) ) {
//        rumble.start();
//        if (lastFocusedMenu)
//          lastFocusedMenu->setActiveState(false);
//        lastFocusedMenu = static_cast< GraphicsButton*>( obj);
//        lastFocusedMenu->setActiveState(true);
//      }
//    }
//  }
  cursorHandle->setPos(mouseX, mouseY);


}

