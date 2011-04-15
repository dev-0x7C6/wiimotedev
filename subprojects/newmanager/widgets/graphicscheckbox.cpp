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

#include "widgets/graphicscheckbox.h"
#include <QPainter>
#include <QDebug>

#include <QGraphicsSceneMouseEvent>


GraphicsCheckbox::GraphicsCheckbox (QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(0),
  height(0),
  text(""),
  pixmap(0),
  actived(false),
  focused(false),
  groupId(0),
  checked(false)
{

  theme.buttonColorFocus = QColor(61, 162, 235,  100);
  theme.buttonColorActive = QColor(61, 162, 235,  50);
  theme.buttonColorInactive = QColor(255, 255, 255, 10);
  theme.buttonFont = QFont("Luxi Serif Bold Oblique", 16, QFont::Bold);
  theme.maxScaleSize = 1.2;
  theme.minScaleSize = 1.0;

  setObjectName("GraphicsCheckbox");
  setAcceptHoverEvents(true);

}


void GraphicsCheckbox::hoverEnterEvent (QGraphicsSceneHoverEvent * event) {

}

void GraphicsCheckbox::hoverLeaveEvent (QGraphicsSceneHoverEvent * event) {

}

void GraphicsCheckbox::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  emit clicked();
}

void GraphicsCheckbox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}


QPainterPath GraphicsCheckbox::shape() const {
  QPainterPath path;
  path.addRect(boundingRect());
  return path;
}

QRectF GraphicsCheckbox::boundingRect() const {
  if (height < 64)
    return QRectF(0, -16, width, height+32);
    return QRectF(0, 0, width, height);
}

void GraphicsCheckbox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(widget);
  painter->setClipRect(option->exposedRect);
  painter->setPen(Qt::NoPen);

  if (actived)
    painter->setBrush(theme.buttonColorActive); else
    painter->setBrush(theme.buttonColorInactive);

  if (focused)
    painter->setBrush(theme.buttonColorFocus);


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



  register float pos = 10;

  if (pixmap) {
     pos = (height/2.0) - (pixmap->height()/2.0);
     painter->drawPixmap(pos+10, pos, pixmap->width(), pixmap->height(), *pixmap);
     painter->setFont(theme.buttonFont);
  }

  if (checked) {
    painter->setBrush(theme.buttonColorInactive);
    painter->drawRect(QRect(width-(height*2), 0, height, height));
    painter->setBrush(theme.buttonColorFocus);
    painter->drawRect(QRect(width-(height), 0, height, height));
  } else {
    painter->setBrush(theme.buttonColorInactive);
    painter->drawRect(QRect(width-(height), 0, height, height));
    painter->setBrush(theme.buttonColorFocus);
    painter->drawRect(QRect(width-(height*2), 0, height, height));
  }

  painter->setPen(Qt::white);
  painter->drawText(10, 0, width-(height*2), height , Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextJustificationForced,  text);

  painter->drawText(width-(height*2), 0, height, height, Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,  QString("YES"));
  painter->drawText(width-(height), 0, height, height, Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,  "NO");

  painter->setPen(Qt::NoPen);



}
