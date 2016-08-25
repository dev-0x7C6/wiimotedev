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

#include <io/emulation/input-device.h>
#include <io/functionals/uinput-helper.h>

#include <cstring>
#include <iostream>
#include <sys/stat.h>

InputDevice::InputDevice(std::string name)
		: m_file(nullptr)
		, m_fd(-1)

{
	auto path = UInputHelper::findUinputInterface();

	if (path.empty()) {
		std::cerr << "error: unable to find uinput interface!" << std::endl;
		return;
	}

	m_file = fopen(path.c_str(), "rw");

	if (m_file == nullptr) {
		std::cerr << "error: unable to open uinput interface!" << std::endl;
		return;
	}

	m_fd = fileno(m_file);

	memset(&m_dev, 0, sizeof(m_dev));
	name.resize(std::min(name.size(), sizeof(m_dev.name)));
	strcpy(m_dev.name, name.c_str());
	m_dev.id.product = 1;
	m_dev.id.version = 1;
	m_dev.id.vendor = 1;
	m_dev.id.bustype = BUS_USB;
}

bool InputDevice::create() {
	write(m_fd, &m_dev, sizeof(m_dev));

	if (ioctl(m_fd, UI_DEV_CREATE)) {
		std::cerr << "error: unable to create input device" << std::endl;
		return false;
	}

	return true;
}

bool InputDevice::destroy()
{
	ioctl(m_fd, UI_DEV_DESTROY);
}

InputDevice::~InputDevice() {
	if (isValid())
		fclose(m_file);
}

int InputDevice::set_ev_bit(int bit) { return UInputHelper::set_ev_bit(m_fd, bit); }
int InputDevice::set_key_bit(int bit) { return UInputHelper::set_key_bit(m_fd, bit); }
int InputDevice::set_rel_bit(int bit) { return UInputHelper::set_rel_bit(m_fd, bit); }
int InputDevice::set_abs_bit(int bit) { return UInputHelper::set_abs_bit(m_fd, bit); }
void InputDevice::set_range(int abs, int max, int min) {	UInputHelper::range(m_dev, abs, max, min); }

void InputDevice::report(uint16_t type, uint16_t code, int32_t value, bool triggerSync) {
	if (!isValid())
		return;

	struct input_event event;
	memset(&event.time, 0, sizeof(event.time));
	event.code = code;
	event.type = type;
	event.value = value;
	write(m_fd, &event, sizeof(event));

	if (triggerSync)
		sync();
}

void InputDevice::sync() { report(EV_SYN, SYN_REPORT, 0); }

bool InputDevice::isValid() const { return m_file != nullptr && m_fd != -1; }
