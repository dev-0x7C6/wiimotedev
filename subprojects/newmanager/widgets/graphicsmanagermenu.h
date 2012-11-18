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

#ifndef GRAPHICSMANAGERMENU_H
#define GRAPHICSMANAGERMENU_H


#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QTimeLine>

class GraphicsManagerMenuItem :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
  Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)

  quint32 width;
  quint32 height;
  bool hovered;
  QTimeLine timeline;
  quint32 currentFontSize;

public:
  GraphicsManagerMenuItem(QObject *parent = 0);

  QRectF boundingRect() const { return QRectF(0, 0, width, height); }
  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }


protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  void virtual hoverEnterEvent (QGraphicsSceneHoverEvent * event);
  void virtual hoverLeaveEvent (QGraphicsSceneHoverEvent * event);
};


class GraphicsManagerMenu :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
  Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)

  quint32 width;
  quint32 height;
  bool hovered;

public:
  GraphicsManagerMenu(QObject *parent = 0);

  QRectF boundingRect() const { return QRectF(0, 0, width, height); }
  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }


protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  void virtual hoverEnterEvent (QGraphicsSceneHoverEvent * event);
  void virtual hoverLeaveEvent (QGraphicsSceneHoverEvent * event);
};

#endif // GRAPHICSMANAGERMENU_H