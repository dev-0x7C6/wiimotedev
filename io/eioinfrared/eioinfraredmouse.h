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

#include "eiobase/eioeventdevice.h"
#include "common/tick-aligned-loop.h"
#include "container/infrared-config.h"
#include "container/infrared-cursor.h"
#include "linux/usr/include/wiimotedev/consts.h"

#include <QTimer>
#include <QTime>
#include <QThread>
#include <QMutex>

#include <atomic>

class EIOInfraredMouse : public QThread {
public:
	enum Axis {
		XAxis = 0,
		YAxis = 1
	};

	enum Mode {
		Absolute,
		Relative
	};

	EIOInfraredMouse(EventDevice &device, QObject *parent = nullptr);

	void dbusVirtualCursorPosition(uint32_t, double, double, double, double);
	void dbusVirtualCursorLost(uint32_t);

	uint32_t id() const;
	void setId(const uint32_t id);

	InfraredConfigContainer &config() {
		return m_config;
	}

	void interrupt();

protected:
	void run();

	void processAbsoluteMouse();
	void processRelativeMouse();

	void setCursor(InfraredCursor &&cursor);

private:
	double m_position[2];
	double m_delta[2];
	double m_size[2];
	double m_acc[2];
	int32_t m_timeout;

	InfraredConfigContainer m_config;

private:
	EventDevice &m_device;
	InfraredCursor m_cursor;

	std::atomic<uint32_t> m_id;
	std::atomic<uint32_t> m_mode;
	std::atomic<bool> m_interrupted;
	QMutex m_mutex;
	TickAlignedLoop m_tick;
};
