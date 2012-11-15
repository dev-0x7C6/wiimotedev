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

void WiimoteMessageThread::cwiid_process_classic_init() {
  cwiid_process_classic_clear();
}

void WiimoteMessageThread::cwiid_process_classic_done() {
  cwiid_process_classic_clear();
}

void WiimoteMessageThread::cwiid_process_classic_clear() {
  cstate[ix_classic_device] = 0x00;
  lstate[ix_classic_device] = 0x00;
  stick[ix_classic_lstick].x = 0x00;
  stick[ix_classic_lstick].y = 0x00;
  stick[ix_classic_rstick].x = 0x00;
  stick[ix_classic_rstick].y = 0x00;
  available[ix_classic_device] = false;
}

void WiimoteMessageThread::cwiid_process_classic_status(cwiid_ext_type type) {
  switch (type) {
  case CWIID_EXT_NONE:
    if (available[ix_classic_device]) {
      cwiid_process_classic_clear();
      emit dbusClassicControllerUnplugged(m_id);
    }
    break;

  case CWIID_EXT_CLASSIC:
    if (!available[ix_classic_device]) {
      available[ix_classic_device] = true;
      emit dbusClassicControllerPlugged(m_id);
    }
    break;
  }
}

void WiimoteMessageThread::cwiid_process_classic_lstick(quint8 cwiid_stick[2]) {
  cstate[ix_classic_device] &= CLASSIC_LSTICK_NOTMASK;
  if (stick[ix_classic_lstick].x != cwiid_stick[ix_x_axis] ||
      stick[ix_classic_lstick].y != cwiid_stick[ix_y_axis]) {
    stick[ix_classic_lstick].x = cwiid_stick[ix_x_axis];
    stick[ix_classic_lstick].y = cwiid_stick[ix_y_axis];
    emit dbusClassicControllerLStick(m_id, stick[ix_classic_lstick]);
  }

  if (stick[ix_classic_lstick].x > classicLStickMaxX) cstate[ix_classic_device] |= CLASSIC_BTN_LSTICK_RIGHT; else
  if (stick[ix_classic_lstick].x < classicLStickMinX) cstate[ix_classic_device] |= CLASSIC_BTN_LSTICK_LEFT;
  if (stick[ix_classic_lstick].y > classicLStickMaxY) cstate[ix_classic_device] |= CLASSIC_BTN_LSTICK_UP; else
  if (stick[ix_classic_lstick].y < classicLStickMinY) cstate[ix_classic_device] |= CLASSIC_BTN_LSTICK_DOWN;
}

void WiimoteMessageThread::cwiid_process_classic_rstick(quint8 cwiid_stick[2]) {
  cstate[ix_classic_device] &= CLASSIC_RSTICK_NOTMASK;
  if (stick[ix_classic_rstick].x != cwiid_stick[ix_x_axis] ||
      stick[ix_classic_rstick].y != cwiid_stick[ix_y_axis]) {
    stick[ix_classic_rstick].x = cwiid_stick[ix_x_axis];
    stick[ix_classic_rstick].y = cwiid_stick[ix_y_axis];
    emit dbusClassicControllerRStick(m_id, stick[ix_classic_rstick]);
  }

  if (stick[ix_classic_rstick].x > classicRStickMaxX) cstate[ix_classic_device] |= CLASSIC_BTN_RSTICK_RIGHT; else
  if (stick[ix_classic_rstick].x < classicRStickMinX) cstate[ix_classic_device] |= CLASSIC_BTN_RSTICK_LEFT;
  if (stick[ix_classic_rstick].y > classicRStickMaxY) cstate[ix_classic_device] |= CLASSIC_BTN_RSTICK_UP; else
  if (stick[ix_classic_rstick].y < classicRStickMinY) cstate[ix_classic_device] |= CLASSIC_BTN_RSTICK_DOWN;
}

void WiimoteMessageThread::cwiid_process_classic_buttons(quint16 cwiid_buttons) {
  cstate[ix_classic_device] &= CLASSIC_BUTTON_NOTMASK;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_A) cstate[ix_classic_device] |= CLASSIC_BTN_A;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_B) cstate[ix_classic_device] |= CLASSIC_BTN_B;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_X) cstate[ix_classic_device] |= CLASSIC_BTN_X;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_Y) cstate[ix_classic_device] |= CLASSIC_BTN_Y;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_MINUS) cstate[ix_classic_device] |= CLASSIC_BTN_MINUS;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_PLUS) cstate[ix_classic_device] |= CLASSIC_BTN_PLUS;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_HOME) cstate[ix_classic_device]|= CLASSIC_BTN_HOME;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_L) cstate[ix_classic_device] |= CLASSIC_BTN_L;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_R) cstate[ix_classic_device] |= CLASSIC_BTN_R;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_ZL) cstate[ix_classic_device] |= CLASSIC_BTN_ZL;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_ZR) cstate[ix_classic_device] |= CLASSIC_BTN_ZR;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_UP) cstate[ix_classic_device] |= CLASSIC_BTN_UP;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_DOWN) cstate[ix_classic_device] |= CLASSIC_BTN_DOWN;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_LEFT) cstate[ix_classic_device] |= CLASSIC_BTN_LEFT;
  if (cwiid_buttons & CWIID_CLASSIC_BTN_RIGHT) cstate[ix_classic_device] |= CLASSIC_BTN_RIGHT;
}
