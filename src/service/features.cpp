/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2010  Bartłomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
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

#include "wiimotemessagethread.h"
#include "core/wiiremote.h"

void WiimoteMessageThread::connect_animation() {
  m_device->setRumbleStatus(true);
  for (register int i = 0; i < 2; ++i) {
    switch (i%2) {
    case 0:
      for (register int j = 0; j < 4; ++j) {
        m_device->setLedStatus(1 << j);
        msleep(30);
      }
      break;
    case 1:
      for (register int j = 3; j >= 0; --j) {
        m_device->setLedStatus(1 << j);
        msleep(30);
      }
      break;
    }
  }
  m_device->setLedStatus(m_id);
  m_device->setRumbleStatus(false);
}

void WiimoteMessageThread::disconnect_animation() {
  m_device->setRumbleStatus(true);
  for (register int i = 0; i < 2; ++i) {
    switch (i%2) {
    case 0:
      for (register int j = 0; j < 4; ++j) {
        m_device->setLedStatus(1 << j);
        msleep(30);
      }
      break;
    case 1:
      for (register int j = 3; j >= 0; --j) {
        m_device->setLedStatus(1 << j);
        msleep(30);
      }
      break;
    }
  }
  m_device->setLedStatus(0x0F);
  m_device->setRumbleStatus(false);
}


