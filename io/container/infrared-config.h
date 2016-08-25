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

#include <cstdlib>

class InfraredConfigContainer {
public:
	bool accEnabled() const;
	double accMultiX() const;
	double accMultiY() const;
	double accPowX() const;
	double accPowY() const;
	double deadzoneX() const;
	double deadzoneY() const;
	int32_t accTimeout() const;

	void setAccEnabled(bool accEnabled);
	void setAccMultiX(double accMultiX);
	void setAccMultiY(double accMultiY);
	void setAccPowX(double accPowX);
	void setAccPowY(double accPowY);
	void setAccTimeout(const int32_t &accTimeout);
	void setDeadzoneX(double deadzoneX);
	void setDeadzoneY(double deadzoneY);

private:
	bool m_accEnabled;
	double m_accMultiX;
	double m_accMultiY;
	double m_accPowX;
	double m_accPowY;
	double m_deadzoneX;
	double m_deadzoneY;
	int32_t m_accTimeout;
};
