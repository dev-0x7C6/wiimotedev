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

#include <io/emulation/event-device.h>

EventDevice::EventDevice(const std::string &name, const uint32_t id)
		: InputDevice(name, id) {
}

bool EventDevice::pressKey(const uint16_t key) { return report(EV_KEY, key, 1, true); }
bool EventDevice::releaseKey(const uint16_t key) { return report(EV_KEY, key, 0, true); }
bool EventDevice::tapKey(const uint16_t key) {
	bool isValid = true;
	isValid &= report(EV_KEY, key, 1, true);
	isValid &= report(EV_KEY, key, 0, true);
	return isValid;
}

bool EventDevice::moveMouseVWheel(const int32_t delta) { return report(EV_REL, REL_WHEEL, delta, true); }
bool EventDevice::moveMouseHWheel(const int32_t delta) { return report(EV_REL, REL_HWHEEL, delta, true); }

bool EventDevice::moveMousePointer(const int32_t x, const int32_t y) {
	bool isValid = true;
	isValid &= report(EV_REL, REL_X, x);
	isValid &= report(EV_REL, REL_Y, y);
	isValid &= sync();
	return isValid;
}

bool EventDevice::configure() {
	bool isValid = true;
	isValid &= set_ev_bit(EV_KEY) == 0;
	isValid &= set_ev_bit(EV_REP) == 0;
	isValid &= set_ev_bit(EV_REL) == 0;

	for (uint16_t i = 0; i < 0xFF; ++i)
		isValid &= set_key_bit(i) == 0;

	for (uint16_t i = BTN_MOUSE; i < BTN_JOYSTICK; ++i)
		isValid &= set_key_bit(i) == 0;

	isValid &= set_rel_bit(REL_X) == 0;
	isValid &= set_rel_bit(REL_Y) == 0;
	isValid &= set_rel_bit(REL_HWHEEL) == 0;
	isValid &= set_rel_bit(REL_WHEEL) == 0;
	return isValid;
};