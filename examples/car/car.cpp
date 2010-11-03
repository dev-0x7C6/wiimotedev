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

#include <QtGui/QtGui>
#include <math.h>

#include "car.h"

#define checkWiimoteBtn(x1, x2) x1 & x2

static const double Pi = 3.14159265358979323846264338327950288419717;

Car::Car() : color(Qt::green),
  wheelsAngle(0), speed(0)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    startTimer(10);
    useRoll = false;
    carBreak = false;
}

Car::~Car()
{
}

void Car::dbusWiimoteButtons(quint32 id, quint64 value)
{
  if (wid != id)
    return;

    if (checkWiimoteBtn(value, WIIMOTE_BTN_B)) carBreak = true; else carBreak = false;
    if (checkWiimoteBtn(value, WIIMOTE_BTN_A)) useRoll = true; else useRoll = false;

    if (carBreak)
    {
        speed = 0;
        return;
    }

    if (checkWiimoteBtn(value, WIIMOTE_BTN_1)) speed = -1; else
    if (checkWiimoteBtn(value, WIIMOTE_BTN_2)) speed = 1; else speed=0;

}

void Car::dbusWiimoteAcc(quint32 id, struct accdata acc)
{
    if (wid != id) return;

    wheelsAngle = acc.pitch;
    if ((wheelsAngle * Pi) >= 170) wheelsAngle = 170 / Pi; else
    if ((wheelsAngle * Pi) <= -170) wheelsAngle = -170 / Pi;

    if (carBreak) return;
    if (useRoll) speed = (acc.roll/35);
}



QRectF Car::boundingRect() const
{
    return QRectF(-35, -81, 70, 115);
}


void Car::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::gray);
    painter->drawRect(-20, -58, 40, 2); // front axel
    painter->drawRect(-20, 7, 40, 2); // rear axel

    painter->setBrush(color);
    painter->drawRect(-25, -79, 50, 10); // front wing

    painter->drawEllipse(-25, -48, 50, 20); // side pods
    painter->drawRect(-25, -38, 50, 35); // side pods
    painter->drawRect(-5, 9, 10, 10); // back pod

    painter->drawEllipse(-10, -81, 20, 100); // main body

    painter->drawRect(-17, 19, 34, 15); // rear wing

    painter->setBrush(Qt::black);
    painter->drawPie(-5, -51, 10, 15, 0, 180 * 16);
    painter->drawRect(-5, -44, 10, 10); // cocpit

    painter->save();
    painter->translate(-20, -58);
    painter->rotate(wheelsAngle);
    painter->drawRect(-10, -7, 10, 15); // front left
    painter->restore();

    painter->save();
    painter->translate(20, -58);
    painter->rotate(wheelsAngle);
    painter->drawRect(0, -7, 10, 15); // front left
    painter->restore();

    painter->drawRect(-30, 0, 12, 17); // rear left
    painter->drawRect(19, 0, 12, 17);  // rear right
    painter->setPen(Qt::red);
    painter->setFont(QFont("Arial", 30));
    painter->drawText(QPoint(-10,-5), QString::number(wid));
}

void Car::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    const qreal axelDistance = 54;

    qreal wheelsAngleRads = (wheelsAngle * Pi) / 180;
    qreal turnDistance = ::cos(wheelsAngleRads) * axelDistance * 2;
    qreal turnRateRads = wheelsAngleRads / turnDistance;  // rough estimate
    qreal turnRate = (turnRateRads * 180) / Pi;
    qreal rotation = speed * turnRate;
    
    rotate(rotation);
    translate(0, -speed);
    update();
}
