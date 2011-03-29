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

GraphicsProfileItem::GraphicsProfileItem (QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(0),
  height(0),
  font(QFont()),
  text(""),
  state(itemInactive),
  focusColor(QColor(61, 162, 235,  150)),
  activeColor(QColor(61, 162, 235, 50)),
  inactiveColor(QColor(61, 235, 162, 25)),
  actived(false),
  focused(false)
{
  setObjectName("GraphicsProfileItem");
  setAcceptHoverEvents(true);

}

void GraphicsProfileItem::hoverEnterEvent (QGraphicsSceneHoverEvent * event) {
}

void GraphicsProfileItem::hoverLeaveEvent (QGraphicsSceneHoverEvent * event)
{

}

void GraphicsProfileItem::hoverEnter() {
    if ((scale() == 1.2) && focused)
        return;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "scale");
  animation->setDuration(200);
  animation->setEasingCurve(QEasingCurve::OutQuart);
  animation->setStartValue(1.0);
  animation->setEndValue(1.2);
  animation->start();
  connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

}

void GraphicsProfileItem::hoverLeave() {
  if (focused)
      return;
  QPropertyAnimation *animation = new QPropertyAnimation(this, "scale");
  animation->setDuration(200);
  animation->setEasingCurve(QEasingCurve::OutQuart);
  animation->setStartValue(1.2);
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

  painter->drawRect(QRect(0, 0, width, height));
  QColor color = painter->brush().color();
  for (register int i = 0; i < 10; ++i) {

    color.setAlpha(10.0/double(i));

    painter->drawRect(QRect(height+i, 0, width, 1));
  }

  painter->setOpacity(1.0);
  painter->drawPixmap(apos+10, apos, 64, 64, icon);
  painter->setFont(font);
  painter->setPen(Qt::white);
  painter->drawText(apos + 64 + 20, 0, width-(apos + 64 + 20), height, Qt::AlignVCenter | Qt::TextWordWrap,  text);
  painter->setPen(Qt::NoPen);
}
