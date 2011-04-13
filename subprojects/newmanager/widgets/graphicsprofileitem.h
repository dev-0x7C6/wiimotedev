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


#ifndef GRAPHICSPROFILEITEM_H
#define GRAPHICSPROFILEITEM_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QWidget>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>


const quint8 AlignLeft = 1 << 0;
const quint8 AlignVCenter = 1 << 1;
const quint8 AlignRight = 1 << 2;
const quint8 AlignTop = 1 << 3;
const quint8 AlignHCenter = 1 << 4;
const quint8 AlignBottom = 1 << 5;


class GraphicsProfileItem :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
  Q_PROPERTY (qreal scale READ scale WRITE setScale)

  quint32 width;
  quint32 height;
  QFont font;
  QString text;
  QPixmap icon;
  quint32 state;

  QColor focusColor;
  QColor activeColor;
  QColor inactiveColor;

  bool focused;
  bool actived;

  quint32 hoverAlign;
  double hoverScale;

  quint32 groupId;


  QPropertyAnimation *scaleAnimation;
  bool lockScaleAnimation;

public:
  GraphicsProfileItem (QObject *parent = 0);
  QRectF boundingRect() const;
  QPainterPath shape() const;

  enum States {
    itemActive = 0,
    itemInactive,
    itemFocus
  };

  static QHash< int, GraphicsProfileItem*>  profiles;


  void setScaleAnimationEnabled(bool lock);
  void runScaleAnimation(double scale);


  void setAlignFlags(quint32 flags) { hoverAlign = flags; }
  void setHoverScale(double scale) { hoverScale = scale; }



  void setGroupId(quint32 id ) { groupId = id; };

  void setWidth(int w) { width = w; setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2); }
  void setHeight(int h) { height = h; setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2); }


  void setText(QString t) { text = t; }
  void setIcon(QPixmap i) { icon = i; apos = (height/2)-(icon.height()/2);}
  void setFont(QFont f) { font = f; }

  void setFocusColor(QColor c) { focusColor = c;  }
  void setActiveColor(QColor c) { inactiveColor = c; }
  void setInactiveColor(QColor c) { inactiveColor = c; }

  void setActiveState(bool f) { actived = f; }
  void setFocusState(bool a) { focused = a; }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  void virtual mousePressEvent(QGraphicsSceneMouseEvent *event);
  void virtual mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void virtual hoverEnterEvent (QGraphicsSceneHoverEvent * event);
  void virtual hoverLeaveEvent (QGraphicsSceneHoverEvent * event);

private:
  int apos;

signals:
  void buttonPressed();
};


#endif // GRAPHICSPROFILEITEM_H
