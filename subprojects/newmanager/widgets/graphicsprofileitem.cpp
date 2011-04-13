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

#include "graphicsprofileitem.h"
#include <QPainter>
#include <QDebug>

#include <QGraphicsSceneMouseEvent>

QHash< int, GraphicsProfileItem*>  GraphicsProfileItem::profiles = QHash< int, GraphicsProfileItem*>();

GraphicsProfileItem::GraphicsProfileItem (QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(0),
  height(0),
  font(QFont()),
  text(""),
  state(itemInactive),
  focusColor(QColor(61, 162, 235,  100)),
  activeColor(QColor(61, 162, 235, 50)),
  inactiveColor(QColor(255, 255, 255, 10)),
  actived(false),
  focused(false),
  hoverAlign(AlignVCenter | AlignHCenter),
  hoverScale(1.2),
  groupId(0),
  scaleAnimation(new QPropertyAnimation(this, "scale")),
  lockScaleAnimation(false)
{
  setObjectName("GraphicProfileItem");
  setAcceptHoverEvents(true);
}

void GraphicsProfileItem::setScaleAnimationEnabled(bool lock) {
  lockScaleAnimation = lock;
}

void GraphicsProfileItem::runScaleAnimation(double scale) {
  if (lockScaleAnimation)
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

  scaleAnimation->setDuration(250);
  scaleAnimation->setEasingCurve(QEasingCurve::OutQuart);
  scaleAnimation->setStartValue(this->scale());
  scaleAnimation->setEndValue(scale);
  scaleAnimation->start();
}


void GraphicsProfileItem::hoverEnterEvent (QGraphicsSceneHoverEvent * event) {
  this->setActiveState(true);
  runScaleAnimation(hoverScale);
}

void GraphicsProfileItem::hoverLeaveEvent (QGraphicsSceneHoverEvent * event) {
  this->setActiveState(false);
  runScaleAnimation(1.0);
}

void GraphicsProfileItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if ((event->button() != Qt::LeftButton) ||
      (this == profiles[groupId]))
    return;

  if (profiles[groupId]) {
    profiles[groupId]->setFocusState(false);
    profiles[groupId]->setScaleAnimationEnabled(false);
    profiles[groupId]->runScaleAnimation(1.0);
  }
  setFocusState(true);
  setScaleAnimationEnabled(true);
  profiles[groupId] = this;

  emit buttonPressed();
}

void GraphicsProfileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}


QPainterPath GraphicsProfileItem::shape() const {
  QPainterPath path;
  path.addRect(boundingRect());
  return path;
}

QRectF GraphicsProfileItem::boundingRect() const {
  if (height < 64)
    return QRectF(0, -16, width, height+32);
    return QRectF(0, 0, width, height);
}

void GraphicsProfileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setClipRect( option->exposedRect );
  Q_UNUSED(widget);


  painter->setPen(Qt::NoPen);

  if (actived)
    painter->setBrush(activeColor); else
    painter->setBrush(inactiveColor);

  if (focused)
    painter->setBrush(focusColor);

  QColor color = painter->brush().color();
  int alpha = color.alpha();

  painter->setBrush(color);
  painter->drawRect(QRect(0, height/3*2, width, height/3));

  for (register int i = 0; i < height/3*2; i++) {
    color.setAlpha(alpha+(10 * (double(i)/double(height/3*2))));
    painter->setBrush(color);
    painter->drawRect(QRect(0, i, width, 1));
  }


  painter->setOpacity(1.0);
  painter->drawPixmap(apos+10, apos, 64, 64, icon);
  painter->setFont(font);
  painter->setPen(Qt::white);
  painter->drawText(apos + 64 + 20, 0, width-(apos + 64 + 20), height, Qt::AlignVCenter | Qt::TextWordWrap,  text);
  painter->setPen(Qt::NoPen);
}
