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

#include "eiomousedevice.h"

void UInputMouse::moveMousePointerRel(int32_t x, int32_t y) {
	report(EV_REL, REL_X, x);
	report(EV_REL, REL_Y, y);
	sync();
}

void UInputMouse::pressMouseButton(uint16_t button) {
	if (button < BTN_MOUSE || button >= BTN_JOYSTICK)
		return;

	report(EV_KEY, button, true, true);
}

void UInputMouse::releaseMouseButton(uint16_t button) {
	if (button < BTN_MOUSE || button >= BTN_JOYSTICK)
		return;

	report(EV_KEY, button, false, true);
}

bool UInputMouse::configure() {
	auto isValid = true;
	const auto abs = false;
	QRect absRect = QRect(-512, -384, 1024, 768);
	isValid &= evbit(EV_KEY) == 0;
	isValid &= evbit(abs ? EV_ABS : EV_REL) == 0;

	for (uint16_t i = BTN_MOUSE; i < BTN_JOYSTICK; ++i)
		isValid &= keybit(i) == 0;

	if (abs) {
		isValid &= absbit(ABS_X) == 0;
		isValid &= absbit(ABS_Y) == 0;
		range(ABS_X, absRect.right(), absRect.left());
		range(ABS_Y, absRect.bottom(), absRect.top());
	} else {
		isValid &= relbit(REL_X) == 0;
		isValid &= relbit(REL_Y) == 0;
		isValid &= relbit(REL_HWHEEL) == 0;
		isValid &= relbit(REL_WHEEL) == 0;
	}
	return isValid;
}

void UInputMouse::moveMouseVWheel(int32_t direction) { report(EV_REL, REL_WHEEL, direction); }
void UInputMouse::moveMouseHWheel(int32_t direction) { report(EV_REL, REL_HWHEEL, direction); }

void UInputMouse::moveMousePointerAbs(int32_t x, int32_t y) {
	report(EV_ABS, ABS_X, x);
	report(EV_ABS, ABS_Y, y);
	sync();
}
