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

#include "widgets/graphicsbutton.h"
#include <QPainter>
#include <QDebug>

#include <QGraphicsSceneMouseEvent>

QHash< int, GraphicsButton*>  GraphicsButton::profiles = QHash< int, GraphicsButton*>();

GraphicsButton::GraphicsButton (QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(0),
  height(0),
  margin(10),
  bgstyle(1),
  text(""),
  pixmap(0),
  actived(false),
  focused(false),
  selectable(true),
  hoverAlign(AlignVCenter | AlignHCenter),
  iconAlign(AlignLeft | AlignHCenter),
  groupId(0),
  scaleAnimation(new QPropertyAnimation(this, "scale")),
  scaleLock(false)
{

  theme.buttonColorFocus = QColor(61, 162, 235,  100);
  theme.buttonColorActive = QColor(61, 162, 235,  50);
  theme.buttonColorInactive = QColor(255, 255, 255, 10);
  theme.buttonFont = QFont("Luxi Serif Bold Oblique", 16, QFont::Bold);
  theme.maxScaleSize = 1.2;
  theme.minScaleSize = 1.0;

  setObjectName("GraphicsButton");
  setAcceptHoverEvents(true);

  scaleAnimation->setDuration(250);
  scaleAnimation->setEasingCurve(QEasingCurve::OutQuart);
}

void GraphicsButton::setScaleAnim(double scale) {
  if (scaleLock)
    return;

  QPointF p;
  if (hoverAlign & AlignLeft) p.setX(0); else
  if (hoverAlign & AlignVCenter) p.setX(width/2); else
  if (hoverAlign & AlignRight) p.setX(width);
  if (hoverAlign & AlignTop) p.setY(0); else
  if (hoverAlign & AlignHCenter) p.setY(height/2); else
  if (hoverAlign & AlignBottom) p.setY(height);
  setTransformOriginPoint(p);

  if (scaleAnimation->state() == QAbstractAnimation::Running)
    scaleAnimation->stop();

  scaleAnimation->setStartValue(this->scale());
  scaleAnimation->setEndValue(scale);
  scaleAnimation->start();
}

void GraphicsButton::hoverEnterEvent (QGraphicsSceneHoverEvent * event) {
  this->setActiveState(true);
  this->setScaleAnim(theme.maxScaleSize);
}

void GraphicsButton::hoverLeaveEvent (QGraphicsSceneHoverEvent * event) {
  this->setActiveState(false);
  this->setScaleAnim(theme.minScaleSize);
}

void GraphicsButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (selectable == true) {
    if ((event->button() != Qt::LeftButton) ||
        (this == profiles[groupId]))
      return;

    if (profiles[groupId]) {
      profiles[groupId]->setFocusState(false);
      profiles[groupId]->setScaleLock(false);
      profiles[groupId]->setScaleAnim(theme.minScaleSize);
    }
    setFocusState(true);
    setScaleLock(true);
    profiles[groupId] = this;
  }
  emit clicked();
}

void GraphicsButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

QPainterPath GraphicsButton::shape() const {
  QPainterPath path;
  path.addRect(boundingRect());
  return path;
}

QRectF GraphicsButton::boundingRect() const {
  if (height < 64)
    return QRectF(0, -16, width, height+32);
    return QRectF(0, 0, width, height);
}

void GraphicsButton::renderBackgrounds() {
  QPainter *painter;
  QColor color;
  int alpha;

  backgroundActive = new QPixmap(width, height);
  color = theme.buttonColorActive;
  alpha = color.alpha();
  painter = new QPainter(backgroundActive);

  painter->setBrush(color);
  painter->drawRect(QRect(0, height/3*2, width, height/3));

  for (register int i = 0; i < height/3*2; i++) {
    color.setAlpha(alpha+(10 * (double(i)/double(height/3*2))));
    painter->setBrush(color);
    painter->drawRect(QRect(20, i, width, 1));
  }

}

void GraphicsButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(widget);
  painter->setClipRect(option->exposedRect);
  painter->setPen(Qt::NoPen);

  if (actived)
    painter->setBrush(theme.buttonColorActive); else
    painter->setBrush(theme.buttonColorInactive);

  if (focused)
    painter->setBrush(theme.buttonColorFocus);



  if (bgstyle == 1) {


    painter->drawRect(QRect(0, height/3*2, width, height/3));

    QLinearGradient gradient(0, 0, width, height/3*2);
    gradient.setColorAt(0, theme.buttonColorInactive);
    QColor color = theme.buttonColorInactive;
    color.setAlpha(0);
    gradient.setColorAt(1, color);

    painter->setBrush(gradient);
    painter->fillRect(QRect(0, 0, width, height/3*2), gradient);


//    QColor color = painter->brush().color();
//    int alpha = color.alpha();

//    painter->setBrush(color);
//    painter->drawRect(QRect(0, height/3*2, width, height/3));

//    for (register int i = 0; i < height/3*2; i++) {
//      color.setAlpha(alpha+(10 * (double(i)/double(height/3*2))));
//      painter->setBrush(color);
//      painter->drawRect(QRect(00, i, width, 1));
//    }
  }




  register float pos = (height/2.0) - (pixmap->height()/2.0);

  if (pixmap) {
    QPointF p;
    if (iconAlign & AlignLeft) p.setX(10); else
    if (iconAlign & AlignVCenter) p.setX(width/2 - (pixmap->width()/2.0)); else
    if (iconAlign & AlignRight) p.setX(width - (pixmap->width()));
    if (iconAlign & AlignTop) p.setY(0); else
    if (iconAlign & AlignHCenter) p.setY((height/2) - (pixmap->height()/2.0)); else
    if (iconAlign & AlignBottom) p.setY(height - (pixmap->height()));

     painter->drawPixmap(p.x(), p.y(), pixmap->width(), pixmap->height(), *pixmap);
  }

  painter->setFont(theme.buttonFont);
  painter->setPen(Qt::white);


  painter->drawText(pos + 64 + (margin*2), 0, width-(pos + 64 + (margin*2)), height, Qt::AlignVCenter | Qt::TextWordWrap,  text);
}
