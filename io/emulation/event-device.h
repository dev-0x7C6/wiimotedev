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

#include <io/emulation/input-device.h>

class EventDevice final : public InputDevice {
public:
	explicit EventDevice(const std::string &name, const uint32_t id);

	bool pressKey(const uint16_t key);
	bool releaseKey(const uint16_t key);
	bool tapKey(const uint16_t key);
	bool moveMousePointer(const int32_t x, const int32_t y);
	bool moveMouseVWheel(const int32_t delta);
	bool moveMouseHWheel(const int32_t delta);

	virtual bool configure() override;
};
