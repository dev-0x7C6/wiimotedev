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

#include "eiobase/eiouinputobject.h"

#include <QRect>

class UInputMouse : public InputDevice {
public:
	explicit UInputMouse()
			: InputDevice("Virtual Mouse") {}
	void moveMousePointerRel(int32 x, int32 y);
	void moveMousePointerAbs(int32 x, int32 y);
	void moveMouseVWheel(int32 direction);
	void moveMouseHWheel(int32 direction);
	void pressMouseButton(uint16 button);
	void releaseMouseButton(uint16 button);

protected:
	virtual bool configure() override;
};
