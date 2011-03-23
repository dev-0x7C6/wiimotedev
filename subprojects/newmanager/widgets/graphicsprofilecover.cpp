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

#include "graphicsprofilecover.h"

#include <QPainter>

GraphicsProfileCover::GraphicsProfileCover(QObject *parent) :
  QObject(parent),
  QGraphicsItem(),
  width(0),
  height(0),
  font(QFont()),
  text(""),
  state(itemInactive),
  focusColor(QColor(61, 162, 235, 255)),
  activeColor(QColor(61, 162, 235, 50)),
  inactiveColor(QColor(0, 0, 0, 100)),
  actived(false),
  focused(false)

{
  setObjectName("ProfileItem");

}

void GraphicsProfileCover::setCover(QPixmap c)
{
  orginal = c;
  cover = orginal.scaled(QSize(width, height*0.7), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void GraphicsProfileCover::rescale(bool smooth) {
  if (smooth)
    cover = orginal.scaled(QSize(width, height*0.7), Qt::KeepAspectRatio, Qt::SmoothTransformation); else
    cover = orginal.scaled(QSize(width, height*0.7), Qt::KeepAspectRatio, Qt::FastTransformation);
}

void GraphicsProfileCover::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::NoPen);

//  if (actived)
    painter->setBrush(activeColor); //else
   // painter->setBrush(inactiveColor);

//  if (focused)
//    painter->setBrush(focusColor);

  painter->drawRect(QRect(0, 0, width, height));


// painter->setOpacity(1.0);
  painter->drawPixmap((width/2) - (cover.width()/2), (height/2) - (cover.height()/2), cover.width(), cover.height(), cover);
  painter->setFont(font);
  painter->setPen(Qt::white);
  QRectF rect2(0, height - 60, width, 60);
  painter->drawText(rect2, Qt::AlignHCenter | Qt::TextWordWrap, text);
  painter->setPen(Qt::NoPen);
}
