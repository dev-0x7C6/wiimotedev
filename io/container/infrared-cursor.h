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

class InfraredCursor {
public:
	InfraredCursor(double x, double y, double distance, double angle, bool valid = true);
	InfraredCursor(const InfraredCursor &cursor);
	InfraredCursor();

	double x() const;
	double y() const;
	double distance() const;
	double angle() const;
	bool isValid() const;

	InfraredCursor &operator=(const InfraredCursor &other);

private:
	double m_x;
	double m_y;
	double m_distance;
	double m_angle;
	bool m_valid;
};
