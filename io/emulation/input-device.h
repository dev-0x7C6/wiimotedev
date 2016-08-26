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
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <string>
#include <functional>

#include "linux/usr/include/wiimotedev/consts.h"
#include <io/emulation/iinput-device.h>

class InputDevice : public QObject, public IInputDevice {
public:
	explicit InputDevice(const std::string &name, uint32_t id);
	virtual ~InputDevice();

	virtual bool open() override;
	virtual bool configure() override = 0;
	virtual bool create() override;
	virtual bool destroy() override;
	virtual bool close() override;

	virtual bool isOpen() const override;
	virtual bool isCreated() const override;

protected:
	int set_ev_bit(int bit);
	int set_key_bit(int bit);
	int set_rel_bit(int bit);
	int set_abs_bit(int bit);
	void set_range(int abs, int max, int min);

	bool report(uint16_t type, uint16_t code, int32_t value, bool triggerSync = false);
	bool sync();

private:
	const std::string m_name;
	FILE *m_file;
	int m_fd;
	struct uinput_user_dev m_dev;
	bool m_isCreated;
};
