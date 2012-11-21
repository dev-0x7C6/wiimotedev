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

#ifndef GRAPHICSCHECKBOX_H
#define GRAPHICSCHECKBOX_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QWidget>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>

struct GraphicsCheckboxTheme {
  QFont buttonFont;
  QColor buttonColorFocus;
  QColor buttonColorActive;
  QColor buttonColorInactive;
  double maxScaleSize;
  double minScaleSize;
};

class GraphicsCheckbox :public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)
  Q_PROPERTY (qreal scale READ scale WRITE setScale)
  Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)

  Q_PROPERTY (double motion READ motion WRITE setMotion)

  GraphicsCheckboxTheme theme;
  quint32 width;
  quint32 height;
  QString text;
  QPixmap *pixmap;

  bool actived;
  bool focused;

  quint32 hoverAlign;

  quint32 groupId;


  bool isChecked;
  double m_motion;

  QPropertyAnimation *changeAnimation;

public:
  GraphicsCheckbox (QObject *parent = 0);
  QRectF boundingRect() const;
  QPainterPath shape() const;

  void setAlignFlags(quint32 flags) { hoverAlign = flags; }

  void setGroupId(quint32 id ) { groupId = id; };

  void setActiveState(bool f) { actived = f; }
  void setFocusState(bool a) { focused = a; }

  double motion() { return m_motion; update(width-height*2,0, height*2, height); }
  void setMotion(double m) { m_motion = m; update(width-height*2,0, height*2, height);}

public:
  void setWidth(int w);
  void setHeight(int h);

  void setFont(QFont &font);
  void setIconFromPath(QString path);
  void setText(QString string);


protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  void virtual mousePressEvent(QGraphicsSceneMouseEvent *event);
  void virtual mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void virtual hoverEnterEvent (QGraphicsSceneHoverEvent * event);
  void virtual hoverLeaveEvent (QGraphicsSceneHoverEvent * event);

signals:
  void checked(bool);

};

inline void GraphicsCheckbox::setWidth(int w) {
  width = w;
}

inline void GraphicsCheckbox::setHeight(int h) {
  height = h;
}

inline void GraphicsCheckbox::setFont(QFont &font) {
  theme.buttonFont = font;
}

inline void GraphicsCheckbox::setIconFromPath(QString path) {
  pixmap = new QPixmap(path);
}

inline void GraphicsCheckbox::setText(QString string) {
  text = string;
}

#endif // GRAPHICSCHECKBOX_H
