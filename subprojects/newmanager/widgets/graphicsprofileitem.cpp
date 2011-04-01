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
  hoverScale(1.2)
{
  setObjectName("GraphicProfileItem");
  setAcceptHoverEvents(true);

}

void GraphicsProfileItem::hoverEnterEvent (QGraphicsSceneHoverEvent * event) {
}

void GraphicsProfileItem::hoverLeaveEvent (QGraphicsSceneHoverEvent * event)
{

}

void GraphicsProfileItem::hoverEnter() {
  QPointF p;
  if (hoverAlign & AlignLeft) p.setX(0); else
  if (hoverAlign & AlignVCenter) p.setX(width/2); else
  if (hoverAlign & AlignRight) p.setX(width);

  if (hoverAlign & AlignTop) p.setY(0); else
  if (hoverAlign & AlignHCenter) p.setY(height/2); else
  if (hoverAlign & AlignBottom) p.setY(height);

  setTransformOriginPoint(p);

  if ((scale() == hoverScale) && focused)
    return;
  QPropertyAnimation *animation = new QPropertyAnimation(this, "scale");
  animation->setDuration(200);
  animation->setEasingCurve(QEasingCurve::OutQuart);
  animation->setStartValue(1.0);
  animation->setEndValue(hoverScale);
  animation->start();
  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

}

void GraphicsProfileItem::hoverLeave() {
  QPointF p;
  if (hoverAlign & AlignLeft) p.setX(0); else
  if (hoverAlign & AlignVCenter) p.setX(width/2); else
  if (hoverAlign & AlignRight) p.setX(width);

  if (hoverAlign & AlignTop) p.setY(0); else
  if (hoverAlign & AlignHCenter) p.setY(height/2); else
  if (hoverAlign & AlignBottom) p.setY(height);

  setTransformOriginPoint(p);


  if (focused)
      return;
  QPropertyAnimation *animation = new QPropertyAnimation(this, "scale");
  animation->setDuration(200);
  animation->setEasingCurve(QEasingCurve::OutQuart);
  animation->setStartValue(hoverScale);
  animation->setEndValue(1.0);
  animation->start();
  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
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
    color.setAlpha(alpha+(30 * (double(i)/double(height/3*2))));
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
