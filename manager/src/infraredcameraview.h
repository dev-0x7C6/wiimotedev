/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2015  Bartłomiej Burdukiewicz                               *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
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

#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QMenu>

#include "linux/usr/include/wiimotedev/deviceevents.h"

#include "linux/usr/include/wiimotedev/consts.h"

const double defaultPointMultiplier = 2.5;

class InfraredCameraView : public QGraphicsView {
	Q_OBJECT
private:
	QString macAddress;

public:
	InfraredCameraView(WiimotedevDeviceEvents *iface, uint32_t id = 1, QWidget *parent = 0);
	~InfraredCameraView();

	enum Points {
		ir0source = 0,
		ir1source,
		ir2source,
		ir3source,
		ir4source
	};

	enum IROrder {
		LeftToRight,
		RightToLeft
	};

	enum AnalogMode {
		modeNone,
		modeClassic,
		modeNunchuk
	};

	void setWiimoteId(uint32_t id);

protected:
	virtual void resizeEvent(QResizeEvent *);

private:
	QGraphicsLineItem *line;
	QGraphicsPixmapItem *cursor;

	QGraphicsEllipseItem *infraredPoints[4];
	QGraphicsLineItem *infraredLine[4];
	QTime timer;

	WiimotedevDeviceEvents *iface;
	uint32_t wiimoteId;

	QGraphicsScene scene;
	QTimer infraredTimeout;

	double widthMultiplier;
	double heightMultiplier;
	double dotSizeMultiplier;

	QString getReadableWiiremoteSequence(uint64_t);

	void infraredCleanup();

public:
	void dbusVirtualCursorPosition(uint32_t id, double x, double y, double size, double angle);
	void dbusVirtualCursorLost(uint32_t id);
	void dbusVirtualCursorFound(uint32_t id);

	void dbusWiimoteAcc(uint32_t id, const accdata &acc);
	void dbusWiimoteConnected(uint32_t id);
	void dbusWiimoteDisconnected(uint32_t id);
	void dbusWiimoteInfrared(uint32_t id, const QList<struct irpoint> &points);
};
