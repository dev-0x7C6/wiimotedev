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

void EventDevice::pressKeyboardButton(uint16_t button) { report(EV_KEY, button, 1, true); }
void EventDevice::releaseKeyboardButton(uint16_t button) { report(EV_KEY, button, 0, true); }

void EventDevice::pressKeyboardButtonOnce(uint16_t button) {
	report(EV_KEY, button, 1, true);
	report(EV_KEY, button, 0, true);
}

void EventDevice::moveMouseVWheel(int32_t direction) { report(EV_REL, REL_WHEEL, direction, true); }
void EventDevice::moveMouseHWheel(int32_t direction) { report(EV_REL, REL_HWHEEL, direction, true); }

void EventDevice::moveMousePointerRel(int32_t x, int32_t y) {
	report(EV_REL, REL_X, x);
	report(EV_REL, REL_Y, y);
	sync();
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
