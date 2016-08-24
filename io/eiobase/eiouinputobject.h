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

#include <QObject>
#include <QString>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <string>
#include <functional>

#include "linux/usr/include/wiimotedev/consts.h"

class InputDevice {
public:
	explicit InputDevice(std::string name);
	virtual ~InputDevice();

	static std::string findUinputInterface();
	static int evbit(int fd, int bit) { return ioctl(fd, UI_SET_EVBIT, bit); }
	static int keybit(int fd, int bit) { return ioctl(fd, UI_SET_KEYBIT, bit); }
	static int relbit(int fd, int bit) { return ioctl(fd, UI_SET_RELBIT, bit); }
	static int absbit(int fd, int bit) { return ioctl(fd, UI_SET_ABSBIT, bit); }
	static void range(struct uinput_user_dev &dev, int abs, int max, int min) {
		dev.absmax[abs] = max;
		dev.absmin[abs] = min;
		dev.absflat[abs] = 0;
		dev.absfuzz[abs] = 0;
	}

	inline int evbit(int bit) { return evbit(m_fd, bit); }
	inline int keybit(int bit) { return keybit(m_fd, bit); }
	inline int relbit(int bit) { return relbit(m_fd, bit); }
	inline int absbit(int bit) { return absbit(m_fd, bit); }
	inline void range(int abs, int max, int min) { range(m_dev, abs, max, min); }

	void sendEvent(uint16 type, uint16 code, int32 value);
	void sendEventSync();

	std::string interfaceFilePath() const;
	bool isValid() const;

	virtual bool configure() = 0;
	virtual bool create();

private:
	std::string m_interfaceFilePath;
	FILE *m_file;
	int m_fd;
	struct uinput_user_dev m_dev;
};
