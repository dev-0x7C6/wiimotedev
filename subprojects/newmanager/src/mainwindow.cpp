/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainwindow.h"

const qint32 emptyspace = 20;


ProfileGraphicsItem::ProfileGraphicsItem (quint32 width, quint32 height, quint32 fsize, QString file, QString name, QString icon, QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(width),
  height(height),
  fsize(fsize),
  focused(false),
  actived(false),
  useGradient(false),
  pixmap(new QPixmap(icon)),
  profileFile(file),
  profileName(name)

{
  setObjectName("ProfileGraphicsItem");

  apos = (height/2)-(pixmap->height()/2);
}


QRectF ProfileGraphicsItem ::boundingRect() const {
  return QRectF(0, 0, width, height);
}

void ProfileGraphicsItem::setFocus(bool f) {
  focused = f;
}

void ProfileGraphicsItem::setActive(bool a) {
  actived = a;
}

void ProfileGraphicsItem ::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::NoPen);
  if (actived)
    painter->setBrush(QColor(61, 162, 235, 255)); else
    painter->setBrush(Qt::black);

  if (useGradient) {
    for (int i = 0; i < height; ++i) {
      painter->setOpacity(((height - i)/double(height) * 0.2) + 0.2);
      if (focused)
        painter->setOpacity(painter->opacity() + 0.2);
      if (actived)
        painter->setOpacity(painter->opacity() + 0.6);
      painter->drawRect(QRect(0, i, width, 1));
    }
  } else {
    painter->setOpacity(0.4);
    if (focused)
      painter->setOpacity(painter->opacity() + 0.2);
    if (actived)
      painter->setOpacity(painter->opacity() + 0.6);
    painter->drawRect(QRect(0, 0, width, height));
  }

  painter->setOpacity(1.0);
  painter->drawPixmap(apos+10, apos, 64, 64, *pixmap);

  QFont font("Luxi Serif Bold Oblique", fsize, QFont::Bold);

  painter->setFont(font);

  painter->setPen(Qt::white);
  painter->drawText(apos + 64 + 20, 0, width-(apos + 64 + 20), height, Qt::AlignVCenter | Qt::TextWordWrap,  profileName);
  painter->setPen(Qt::NoPen);
}



GraphicsBarItem::GraphicsBarItem (quint32 width, quint32 height, quint32 location, QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(width),
  height(height),
  location(location)
{
  setObjectName("GraphicsBarItem");
}

QRectF GraphicsBarItem::boundingRect() const {
  return QRectF(-1, 0, width+1, height);
}

void GraphicsBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor(65, 65, 65, 255));
  painter->drawRect(-1, 0, 1, height);
  painter->setBrush(QColor(45, 45, 45, 255));
  painter->drawRect(0, 0, width, height);
  painter->setBrush(QColor(65, 65, 65, 255));
  painter->drawRect(width, 0, 1, height);

}

MainWindow::MainWindow(DBusDeviceEventsInterface *device):
  device(device),
  styledBackground(new QPixmap(":/styled_bg.png")),
  lastFocusedProfile(0),
  lastActivedProfile(0),
  enabledPixmap(new QPixmap(":/enabled.png")),
  disabledPixmap(new QPixmap(":/disabled.png"))
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

  //setWindowFlags(Qt::FramelessWindowHint );
  setWindowFlags(Qt::WindowStaysOnTopHint);
  setWindowOpacity(0.98);

  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setCacheMode(QGraphicsView::CacheBackground);

  setCursor(QCursor(Qt::BlankCursor));
  setMouseTracking(true);
  setGeometry(calculateWindowSize());
  setScene(new QGraphicsScene(0, 0, geometry().width(), geometry().height()));


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
        ProfileGraphicsItem *item = new ProfileGraphicsItem(geometry().width() - 270, 100, 24, file.absoluteFilePath() + "/uinput.ini", settings.value("name", QString("not definited")).toString(),
                                                            file.absoluteFilePath() + "/icon.png");
        item->setX(geometry().width());
        item->setY(0);
        QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
        animation->setDuration(250);
        animation->setEasingCurve(QEasingCurve::OutQuart);
        animation->setStartValue(item->pos());
        animation->setEndValue(QPoint(270, (100 * i + (10*i))));
        animation->start();
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
        i++;
        profiles << item;
        profileGroup.addToGroup(item);
      }
      settings.endGroup();
    }
  }

  profileGroup.setY(50);

  ProfilesMenuItem = new ProfileGraphicsItem(226, 50, 16, "", "Profiles", ":/profile.png");
  ProfilesMenuItem->setX(30);
  ProfilesMenuItem->setY(150);
  ProfilesMenuItem->setActive(true);
  scene()->addItem(ProfilesMenuItem);

  PreferencesMenuItem = new ProfileGraphicsItem(226, 50, 16, "", "Preferences", ":/preferences.png");
  PreferencesMenuItem->setX(30);
  PreferencesMenuItem->setY(220);
  PreferencesMenuItem->setActive(false);
  scene()->addItem(PreferencesMenuItem);

  ConnectionsMenuItem = new ProfileGraphicsItem(226, 50, 16, "", "Connections", ":/connections.png");
  ConnectionsMenuItem->setX(30);
  ConnectionsMenuItem->setY(290);
  ConnectionsMenuItem->setActive(false);
  scene()->addItem(ConnectionsMenuItem);


  GraphicsBarItem *bar = new GraphicsBarItem(2, geometry().height(), 0);
  bar->setZValue(0);
  bar->setX(260);
  bar->setY(0);

  scene()->addItem(bar);


  scene()->addItem(&profileGroup);

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
}

quint32 barsize = 100.0;

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

  painter->setOpacity(1.0);


  {
    painter->setPen(Qt::white);
    QFont font("Luxi Serif Bold Oblique", 16, QFont::Bold);
    painter->setFont(font);
    painter->drawText(15, 40, "Services");
    painter->setPen(Qt::NoPen);
  }

  {
    painter->setPen(Qt::darkGray);
    QFont font("Luxi Serif Bold Oblique", 8, QFont::Bold);
    painter->setFont(font);

    painter->drawText(40, 60, "wiimotedev-daemon: avaliable");
    painter->drawPixmap(20, 49, *enabledPixmap);

    painter->drawText(40, 80, "wiimotedev-uinput: avaliable");
    painter->drawPixmap(20, 69, *enabledPixmap);
    painter->setPen(Qt::NoPen);
  }

  {
    painter->setPen(Qt::white);
    QFont font("Luxi Serif Bold Oblique", 16, QFont::Bold);
    painter->setFont(font);
    painter->drawText(15, 130, "Menu");
    painter->setPen(Qt::NoPen);
  }
}

void MainWindow::drawBackground(QPainter *painter, const QRectF &rect) {
  painter->setOpacity(1);
  painter->drawPixmap(rect.width() - styledBackground->width(),
                      rect.height() - styledBackground->height(), styledBackground->width(),
                      styledBackground->height(), *styledBackground);
}

void MainWindow::dbusWiimoteGeneralButtons(quint32 id, quint64 value) {
  if (id != 1)
    return;

  if (value & WIIMOTE_BTN_HOME)
    setVisible(!isVisible());

  if (value & WIIMOTE_BTN_A) {
    if (lastFocusedProfile) {
      if (lastActivedProfile == lastFocusedProfile) {
        lastActivedProfile->setActive(false);
        lastActivedProfile = 0;
      } else {
        if (lastActivedProfile)
          lastActivedProfile->setActive(false);
        lastActivedProfile = lastFocusedProfile;
        lastActivedProfile->setActive(true);
      }
    }
  }

  lastButtons = value;

  //  QObject *obj = mouse
//  QCoreApplication::postEvent()
}

void MainWindow::mouseMoveEvent (QMouseEvent *event) {
  cursorHandle->setPos(event->x(), event->y());
  cursorHandle->setScale(1.0);
  cursorHandle->setRotation(0);
}

void MainWindow::mousePressEvent (QMouseEvent *event) {

}

void MainWindow::mouseReleaseEvent (QMouseEvent *event)
{

}

void MainWindow::timerEvent(QTimerEvent *event) {
  profileGroup.setY(profileGroup.y() + mouseAccY);


  if (profileGroup.y() < ((profiles.count() * -110) + geometry().height() - int(barsize/2)))
    profileGroup.setY((profiles.count() * -110) + geometry().height() - int(barsize/2));

  if (profileGroup.y() > (barsize/2))
    profileGroup.setY(barsize/2);
}

void MainWindow::dbusVirtualCursorPosition(quint32 id, double x, double y, double size, double angle) {
  if (id != 1)
    return;

  if (!isVisible())
    return;

  mouseX = (geometry().width()/2) - (x * (double(geometry().width()) / 1024.0 * (1.3+(double(abs(512-size))/2)/512)));
  mouseY = (geometry().height()/2) - (y * (double(geometry().height()) / 768.0 * (1.0+(double(abs(386-size))/2)/384)));

  if (mouseX > 240)
    mouseAccY = y * 32/512; else
    mouseAccY = 0;

  QGraphicsItem *obj = scene()->itemAt(mouseX, mouseY);

  if (obj) {
    if (obj->parentItem() == reinterpret_cast< QGraphicsItem*>(&profileGroup)) {
      if (lastFocusedProfile != static_cast< ProfileGraphicsItem*>( obj) )
      {

        if (lastFocusedProfile)
          lastFocusedProfile->setFocus(false);
        lastFocusedProfile = static_cast< ProfileGraphicsItem*>( obj);
        lastFocusedProfile->setFocus(true);
      }
    }
  }


  cursorHandle->setPos(mouseX, mouseY);
  cursorHandle->setScale(((1024-size)/500)*1.3);
  cursorHandle->setRotation(-angle*180/M_PI);
}

