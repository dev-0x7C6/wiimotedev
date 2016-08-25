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

#include "eioeventdevice.h"

void EIOEventDevice::pressKeyboardButton(uint16_t button) { report(EV_KEY, button, 1, true); }
void EIOEventDevice::releaseKeyboardButton(uint16_t button) { report(EV_KEY, button, 0, true); }

void EIOEventDevice::pressKeyboardButtonOnce(uint16_t button) {
	report(EV_KEY, button, 1, true);
	report(EV_KEY, button, 0, true);
}

void EIOEventDevice::moveMouseVWheel(int32_t direction) { report(EV_REL, REL_WHEEL, direction, true); }
void EIOEventDevice::moveMouseHWheel(int32_t direction) { report(EV_REL, REL_HWHEEL, direction, true); }

void EIOEventDevice::moveMousePointerRel(int32_t x, int32_t y) {
	report(EV_REL, REL_X, x);
	report(EV_REL, REL_Y, y);
	sync();
}

bool EIOEventDevice::configure() {
	bool isValid = true;
	isValid &= evbit(EV_KEY) == 0;
	isValid &= evbit(EV_REP) == 0;
	isValid &= evbit(EV_REL) == 0;

	for (uint16_t i = 0; i < 0xFF; ++i)
		isValid &= keybit(i) == 0;

	for (uint16_t i = BTN_MOUSE; i < BTN_JOYSTICK; ++i)
		isValid &= keybit(i) == 0;

	isValid &= relbit(REL_X) == 0;
	isValid &= relbit(REL_Y) == 0;
	isValid &= relbit(REL_HWHEEL) == 0;
	isValid &= relbit(REL_WHEEL) == 0;
	return isValid;
};
