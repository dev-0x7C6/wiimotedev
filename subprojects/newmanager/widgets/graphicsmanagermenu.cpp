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

#include "graphicsmanagermenu.h"

#include <QPainter>
#include <QDebug>

GraphicsManagerMenuItem::GraphicsManagerMenuItem(QObject *parent) :
  QGraphicsItem(),
  QObject(parent),
  hovered(false)

{
  setObjectName("ProfileItem");
  setAcceptHoverEvents(true);
}


void GraphicsManagerMenuItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  timeline.setFrameRange(timeline.currentFrame(), 4000);
  timeline.setEasingCurve(QEasingCurve::OutQuad);
  timeline.setDuration(200);
  timeline.start();
  hovered = true;
}

void GraphicsManagerMenuItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  timeline.setFrameRange(timeline.currentFrame(), 3200);
  timeline.setDuration(200);
  timeline.setEasingCurve(QEasingCurve::OutQuad);
  timeline.start();
  hovered = false;
}

void GraphicsManagerMenuItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QFont font = painter->font();
  font.setPixelSize(double(timeline.currentFrame())/100.0);

  font.setBold(true);


  painter->setFont(font);

  if (hovered)
    painter->setPen(Qt::white); else
    painter->setPen(Qt::darkGray);



  painter->drawText(boundingRect(), Qt::AlignRight | Qt::AlignBottom, "PROFILES");
}




GraphicsManagerMenu::GraphicsManagerMenu(QObject *parent) :
  QGraphicsItem(),
  QObject(parent),
  hovered(false)

{
  setObjectName("ProfileItem");
  setAcceptHoverEvents(true);
}


void GraphicsManagerMenu::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  hovered = true;

}

void GraphicsManagerMenu::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hovered = false;
}

void GraphicsManagerMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);

  qreal sOpacity = opacity();

  if (hovered)
    painter->setOpacity(0.5); else
    painter->setOpacity(0.5);

  painter->setBrush(Qt::black);
  painter->drawRect(0, 0, width, height);


  for (register int i = 0; i < 200; i++) {
    painter->setBrush(QColor(235, 162, 61, (double(i)/200)*150));
    painter->drawRect(width-200+i, 0, 1, height);
  }

  painter->setOpacity(1.0);
  painter->setBrush(QColor(235, 162, 61, 255));
  painter->drawRect(width, 0, 3, height);
}
