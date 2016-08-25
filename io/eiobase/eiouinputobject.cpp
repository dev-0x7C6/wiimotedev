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

#include "eiouinputobject.h"
#include <iostream>
#include <sys/stat.h>

InputDevice::InputDevice(std::string name)
		: m_file(nullptr)
		, m_fd(-1)

{
	m_interfaceFilePath = findUinputInterface();

	if (m_interfaceFilePath.empty()) {
		std::cerr << "error: unable to find uinput interface!" << std::endl;
		return;
	}

	m_file = fopen(m_interfaceFilePath.c_str(), "rw");

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

InputDevice::~InputDevice() {
	if (isValid()) {
		ioctl(m_fd, UI_DEV_DESTROY);
		fclose(m_file);
	}
}

std::string InputDevice::findUinputInterface() {
	static auto paths = {
		"/dev/uinput",
		"/dev/input/uinput",
		"/dev/misc/uinput",
	};

	for (const auto &path : paths) {
		struct stat buffer;
		if (stat(path, &buffer) == 0)
			return path;
	}

	return {};
}

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

std::string InputDevice::interfaceFilePath() const { return m_interfaceFilePath; }
bool InputDevice::isValid() const { return m_file != nullptr && m_fd != -1; }
