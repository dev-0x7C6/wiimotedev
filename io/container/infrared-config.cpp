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

#include "infrared-config.h"

bool InfraredConfigContainer::accEnabled() const {
  return m_accEnabled;
}

void InfraredConfigContainer::setAccEnabled(bool accEnabled) {
  m_accEnabled = accEnabled;
}

double InfraredConfigContainer::accMultiX() const {
  return m_accMultiX;
}

void InfraredConfigContainer::setAccMultiX(double accMultiX) {
  m_accMultiX = accMultiX;
}

double InfraredConfigContainer::accMultiY() const {
  return m_accMultiY;
}

void InfraredConfigContainer::setAccMultiY(double accMultiY) {
  m_accMultiY = accMultiY;
}

double InfraredConfigContainer::accPowX() const {
  return m_accPowX;
}

void InfraredConfigContainer::setAccPowX(double accPowX) {
  m_accPowX = accPowX;
}

double InfraredConfigContainer::accPowY() const {
  return m_accPowY;
}

void InfraredConfigContainer::setAccPowY(double accPowY) {
  m_accPowY = accPowY;
}

double InfraredConfigContainer::deadzoneX() const {
  return m_deadzoneX;
}

void InfraredConfigContainer::setDeadzoneX(double deadzoneX) {
  m_deadzoneX = deadzoneX;
}

double InfraredConfigContainer::deadzoneY() const {
  return m_deadzoneY;
}

void InfraredConfigContainer::setDeadzoneY(double deadzoneY) {
  m_deadzoneY = deadzoneY;
}

int32_t InfraredConfigContainer::accTimeout() const {
  return m_accTimeout;
}

void InfraredConfigContainer::setAccTimeout(const int32_t &accTimeout) {
  m_accTimeout = accTimeout;
}







