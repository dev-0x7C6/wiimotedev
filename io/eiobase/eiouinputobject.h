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

	int set_ev_bit(int bit);
	int set_key_bit(int bit);
	int set_rel_bit(int bit);
	int set_abs_bit(int bit);
	void set_range(int abs, int max, int min);

	void report(uint16_t type, uint16_t code, int32_t value, bool triggerSync = false);
	void sync();

	std::string interfaceFilePath() const;
	bool isValid() const;

	virtual bool configure() = 0;
	virtual bool create();
	virtual bool destroy();

private:
	std::string m_interfaceFilePath;
	FILE *m_file;
	int m_fd;
	struct uinput_user_dev m_dev;
};
