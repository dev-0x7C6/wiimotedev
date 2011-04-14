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


#ifndef GraphicsButton_H
#define GraphicsButton_H

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

struct GraphicsButtonTheme {
  QFont buttonFont;
  QColor buttonColorFocus;
  QColor buttonColorActive;
  QColor buttonColorInactive;
  double maxScaleSize;
  double minScaleSize;
};

class GraphicsButton :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
  Q_PROPERTY (qreal scale READ scale WRITE setScale)

  static QHash< int, GraphicsButton*>  profiles;


  GraphicsButtonTheme theme;
  quint32 width;
  quint32 height;
  QString text;

  bool focused;
  bool actived;
  quint32 hoverAlign;

  quint32 groupId;

  QPropertyAnimation *scaleAnimation;
  bool scaleLock;

  QPixmap *pixmap;

public:
  GraphicsButton (QObject *parent = 0);
  QRectF boundingRect() const;
  QPainterPath shape() const;

  void setAlignFlags(quint32 flags) { hoverAlign = flags; }

  void setGroupId(quint32 id ) { groupId = id; };

  void setActiveState(bool f) { actived = f; }
  void setFocusState(bool a) { focused = a; }


public:
  void setWidth(int w);
  void setHeight(int h);

  void setFont(QFont &font);
  void setIconFromPath(QString path);
  void setText(QString string);

  void setScaleLock(bool lock);
  void setScaleAnim(double scale);


protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  void virtual mousePressEvent(QGraphicsSceneMouseEvent *event);
  void virtual mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void virtual hoverEnterEvent (QGraphicsSceneHoverEvent * event);
  void virtual hoverLeaveEvent (QGraphicsSceneHoverEvent * event);

signals:
  void clicked();
  void subClicked(int);
};

inline void GraphicsButton::setScaleLock(bool lock) {
  scaleLock = lock;
}

inline void GraphicsButton::setWidth(int w) {
  width = w;
}

inline void GraphicsButton::setHeight(int h) {
  height = h;
}

inline void GraphicsButton::setFont(QFont &font) {
  theme.buttonFont = font;
}

inline void GraphicsButton::setIconFromPath(QString path) {
  pixmap = new QPixmap(path);
}

inline void GraphicsButton::setText(QString string) {
  text = string;
}

#endif // GraphicsButton_H
