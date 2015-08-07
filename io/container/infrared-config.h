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
  void setAccEnabled(bool accEnabled);

  double accMultiX() const;
  void setAccMultiX(double accMultiX);

  double accMultiY() const;
  void setAccMultiY(double accMultiY);

  double accPowX() const;
  void setAccPowX(double accPowX);

  double accPowY() const;
  void setAccPowY(double accPowY);

  double deadzoneX() const;
  void setDeadzoneX(double deadzoneX);

  double deadzoneY() const;
  void setDeadzoneY(double deadzoneY);

  int32_t accTimeout() const;
  void setAccTimeout(const int32_t &accTimeout);

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
