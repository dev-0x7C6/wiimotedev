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
  enabledPixmap(new QPixmap(":/enabled.png")),
  disabledPixmap(new QPixmap(":/disabled.png")),
  font8(new QFont("Luxi Serif Bold Oblique", 8, QFont::Bold)),
  font16(new QFont("Luxi Serif Bold Oblique", 16, QFont::Bold)),
  font24(new QFont("Luxi Serif Bold Oblique", 16, QFont::Bold)),
  cursorPixmap(new QPixmap(":/cursor.png")),
  currentCoverIndex(0)
{
  setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

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

  //setWindowFlags(Qt::FramelessWindowHint );
  setWindowFlags(Qt::WindowStaysOnTopHint);
  setWindowOpacity(1);

  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setCacheMode(QGraphicsView::CacheBackground);

  setCursor(QCursor(Qt::BlankCursor));
  setMouseTracking(true);
  setGeometry(calculateWindowSize());
  setScene(new QGraphicsScene(0, 0, geometry().width(), geometry().height()));

  scene()->setBackgroundBrush(Qt::white);


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
        GraphicsProfileItem *item = new GraphicsProfileItem();
        item->setWidth(geometry().width() - 270);
        item->setHeight(100);
        item->setFont(QFont("Luxi Serif Bold Oblique", 24, QFont::Bold));
        item->setText(settings.value("name", QString()).toString());
        item->setIcon(QPixmap(file.absoluteFilePath() + "/icon.png"));
        item->setX(geometry().width());
        item->setY(0);
        QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
        animation->setDuration(250);
        animation->setEasingCurve(QEasingCurve::OutQuart);
        animation->setStartValue(item->pos());
        animation->setEndValue(QPoint(270, (100 * i + (10*i))));
        animation->start();
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

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
        item2->setOpacity(0.2);\

        i++;
        profiles << item;
        covers << item2;
        profileGroup.addToGroup(item);
        coverGroup.addToGroup(item2);
      }
      settings.endGroup();
    }
  }

  covers.at(currentCoverIndex)->setOpacity(1.0);

  profileGroup.setY(50);
  coverGroup.setZValue(-1000);

//  ProfilesMenuItem = new GraphicsProfileItem();
//  ProfilesMenuItem->setWidth(226);
//  ProfilesMenuItem->setHeight(50);
//  ProfilesMenuItem->setFont(QFont("Luxi Serif Bold Oblique", 16, QFont::Bold));
//  ProfilesMenuItem->setText("Profiles");  ProfilesMenuItem->setIcon(QPixmap(":/profile.png"));
//  ProfilesMenuItem->setX(30);
//  ProfilesMenuItem->setY(50);
//  ProfilesMenuItem->setFocusState(true);

//  scene()->addItem(ProfilesMenuItem);

//  PreferencesMenuItem = new GraphicsProfileItem();
//  PreferencesMenuItem->setWidth(226);
//  PreferencesMenuItem->setHeight(50);
//  PreferencesMenuItem->setFont(QFont("Luxi Serif Bold Oblique", 16, QFont::Bold));
//  PreferencesMenuItem->setText("Preferences");
//  PreferencesMenuItem->setIcon(QPixmap(":/preferences.png"));
//  PreferencesMenuItem->setX(30);
//  PreferencesMenuItem->setY(130);


//  scene()->addItem(PreferencesMenuItem);

//  ConnectionsMenuItem = new GraphicsProfileItem();
//  ConnectionsMenuItem->setWidth(226);
//  ConnectionsMenuItem->setHeight(50);
//  ConnectionsMenuItem->setFont(QFont("Luxi Serif Bold Oblique", 16, QFont::Bold));
//  ConnectionsMenuItem->setText("Connections");
//  ConnectionsMenuItem->setIcon(QPixmap(":/connections.png"));
//  ConnectionsMenuItem->setX(30);
//  ConnectionsMenuItem->setY(210);
//  ConnectionsMenuItem->setActive(false);
//  scene()->addItem(ConnectionsMenuItem);


  profileRunning = new QGraphicsPixmapItem();
  profileRunning->setPixmap(QPixmap(":/media-optical.png"));
  profileRunning->setTransformOriginPoint(profileRunning->boundingRect().width()/2, profileRunning->boundingRect().height()/2);
  profileRunning->setTransformationMode(Qt::SmoothTransformation);

  profileRunning->setScale(1.0);
  profileRunning->setZValue(-1000);
  scene()->addItem(profileRunning);



  scene()->addItem(&profileGroup);
  scene()->addItem(&coverGroup);

  menu = new GraphicsManagerMenu();
  menu->setWidth(260);
  menu->setHeight(geometry().height());
  menu->setZValue(100);

  GraphicsManagerMenuItem* abc = new GraphicsManagerMenuItem();
  abc->setWidth(240);
  abc->setHeight(48);
  abc->setZValue(1400);
  abc->setPos(10,50);
  scene()->addItem(abc);

  abc = new GraphicsManagerMenuItem();
    abc->setWidth(240);
    abc->setHeight(48);
    abc->setZValue(1400);
    abc->setPos(10,120);
    scene()->addItem(abc);

    abc = new GraphicsManagerMenuItem();
      abc->setWidth(240);
      abc->setHeight(48);
      abc->setZValue(1400);
      abc->setPos(10,190);
      scene()->addItem(abc);


  profileGroup.setVisible(true);
  coverGroup.setVisible(false);

  scene()->addItem(menu);

  cursorHandle = scene()->addPixmap(QPixmap(":/cursor.png"));
  cursorHandle->setTransformOriginPoint(7, 2);
  cursorHandle->setTransformationMode(Qt::SmoothTransformation);
  confineCursor();

  startTimer(10);
}

void MainWindow::confineCursor() {
  cursorHandle->setZValue(100);
  cursorHandle->setX((geometry().width() / 2));
  cursorHandle->setY((geometry().height() / 2));
}

MainWindow::~MainWindow()
{  
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
    item->setWidth(geometry().width() - 264);
    item->setHeight(height());
    item->setX(264 + ((geometry().width() - 264)*i));
    item->setY(0);
    item->rescale();
    i++;
  }

  profileRunning->setPos(geometry().width() - 360, geometry().height() - 400);


  coverGroup.setPos(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup.pos().y()));

}

quint32 barsize = 80.0;


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


//  painter->setOpacity(1.0);
//  painter->setPen(Qt::white);
//  painter->setFont(*font16);
////  painter->drawText(15, 40, "Services");
//  painter->drawText(15, 10, "Menu");

//  painter->setPen(Qt::darkGray);
//  painter->setFont(*font8);
//  painter->drawText(40, 60, "wiimotedev-daemon: avaliable");
//  painter->drawPixmap(20, 49, *enabledPixmap);

//  painter->drawText(40, 80, "wiimotedev-uinput: avaliable");
//  painter->drawPixmap(20, 69, *enabledPixmap);
//  painter->setPen(Qt::NoPen);
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
    profileGroup.setVisible(true);
    coverGroup.setVisible(false);
  }

  if (value & WIIMOTE_BTN_2) {
    profileGroup.setVisible(false);
    coverGroup.setVisible(true);
  }

  if (value & WIIMOTE_BTN_LEFT) {
    currentCoverIndex--;
    if (currentCoverIndex < 0) {
      currentCoverIndex = covers.count() - 1;
      coverGroup.setPos(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup.pos().y()));
    }

    QPropertyAnimation *animation = new QPropertyAnimation(&coverGroup, "pos");
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->setStartValue(coverGroup.pos());
    animation->setEndValue(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup.pos().y()));
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

    for (register int i = 0; i < covers.count(); ++i) {
        QPropertyAnimation *animation = new QPropertyAnimation(covers.at(i), "opacity");
        animation->setDuration(250);
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->setStartValue(covers.at(i)->opacity());
        animation->setEndValue((i == currentCoverIndex) ? 1 : 0.2);
        animation->start();
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    }

  }

  if (value & WIIMOTE_BTN_RIGHT) {
    currentCoverIndex++;

    if (currentCoverIndex >= covers.count()) {
      currentCoverIndex = 0;
      coverGroup.setPos(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup.pos().y()));
    }

    QPropertyAnimation *animation = new QPropertyAnimation(&coverGroup, "pos");
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->setStartValue(coverGroup.pos());
    animation->setEndValue(QPoint(-(currentCoverIndex*(geometry().width()-264)), coverGroup.pos().y()));
    animation->start();
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

    for (register int i = 0; i < covers.count(); ++i) {
        animation = new QPropertyAnimation(covers.at(i), "opacity");
        animation->setDuration(250);
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->setStartValue(covers.at(i)->opacity());
        animation->setEndValue((i == currentCoverIndex) ? 1 : 0.2);
        animation->start();
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    }

  }



  lastButtons = value;
}

void MainWindow::mouseMoveEvent (QMouseEvent *event) {
  mouseX = event->x();
  mouseY = event->y();
  cursorHandle->setPos(event->x(), event->y());
  cursorHandle->setScale(1.0);
  cursorHandle->setRotation(0);

  double x = ((geometry().height()/2) - event->y())*(512/double(geometry().height()));

  if (mouseX > 260)
    mouseAccY = x * 32/512; else
    mouseAccY = 0;


  QGraphicsItem *obj = scene()->itemAt(mouseX, mouseY);
//  qDebug() << obj;
 if (obj) {
    if (obj->parentItem() == reinterpret_cast< QGraphicsItemGroupPlus*>(&profileGroup)) {
      if (lastFocusedProfile != static_cast< GraphicsProfileItem*>( obj) ) {
        if (lastFocusedProfile)
          lastFocusedProfile->setActiveState(false);
        lastFocusedProfile = static_cast< GraphicsProfileItem*>( obj);
        lastFocusedProfile->setActiveState(true);
      }
    }
  }

 QGraphicsView::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent (QMouseEvent *event) {
   if (mouseX > 260)

  if (lastFocusedProfile) {
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
   QGraphicsView::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent (QMouseEvent *event)
{
QGraphicsView::mouseReleaseEvent(event);
}

void MainWindow::timerEvent(QTimerEvent *event) {
  profileGroup.setY(profileGroup.y() + mouseAccY);

  if (profileGroup.y() < ((profiles.count() * -110) + geometry().height() - int(barsize/2)))
    profileGroup.setY((profiles.count() * -110) + geometry().height() - int(barsize/2));

  if (profileGroup.y() > (barsize/2))
    profileGroup.setY(barsize/2);

  profileRunning->setRotation(profileRunning->rotation()+1);

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

  QGraphicsItem *obj = scene()->itemAt(mouseX, mouseY);
 // qDebug() << obj;
  if (obj) {
    if (obj->parentItem() == reinterpret_cast< QGraphicsItemGroupPlus*>(&profileGroup)) {
      if (lastFocusedProfile != static_cast< GraphicsProfileItem*>( obj) ) {
        if (lastFocusedProfile)
          lastFocusedProfile->setActiveState(false);
        lastFocusedProfile = static_cast< GraphicsProfileItem*>( obj);
        lastFocusedProfile->setActiveState(true);
      }
    }
  }
  cursorSize = ((1024-size)/500)*1.3;
  cursorAngle = -angle*180/M_PI;

  cursorHandle->setPos(mouseX, mouseY);
  cursorHandle->setScale(((1024-size)/500)*1.3);
  cursorHandle->setRotation(-angle*180/M_PI);
}

