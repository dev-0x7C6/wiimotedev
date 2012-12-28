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

void WiimoteMessageThread::cwiid_process_nunchuk_init() {
  cwiid_process_nunchuk_clear();
}

void WiimoteMessageThread::cwiid_process_nunchuk_done() {
  cwiid_process_nunchuk_clear();
}

void WiimoteMessageThread::cwiid_process_nunchuk_clear() {
  cstate[ix_nunchuk_device] = lstate[ix_nunchuk_device] =
  stick[ix_nunchuk_stick].x = stick[ix_nunchuk_stick].y = 0x00;
  setDeviceAvailable(ix_nunchuk_device, false);
}

void WiimoteMessageThread::cwiid_process_nunchuk_buttons(uint8 cwiid_buttons) {
  cstate[ix_nunchuk_device] &= NUNCHUK_BUTTON_NOTMASK;
  if (cwiid_buttons & CWIID_NUNCHUK_BTN_C) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_C;
  if (cwiid_buttons & CWIID_NUNCHUK_BTN_Z) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_Z;
}

void WiimoteMessageThread::cwiid_process_nunchuk_stick(uint8 cwiid_stick[2]) {
  cstate[ix_nunchuk_device] &= NUNCHUK_STICK_NOTMASK;
  if (stick[ix_nunchuk_stick].x != cwiid_stick[ix_x_axis] ||
      stick[ix_nunchuk_stick].y != cwiid_stick[ix_y_axis]) {
    stick[ix_nunchuk_stick].x = cwiid_stick[ix_x_axis];
    stick[ix_nunchuk_stick].y = cwiid_stick[ix_y_axis];
    emit dbusNunchukStick(m_id, stick[ix_nunchuk_stick]);
  }

  if (stick[ix_nunchuk_stick].x > nunchukStickMaxX) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_RIGHT; else
  if (stick[ix_nunchuk_stick].x < nunchukStickMinX) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_LEFT;
  if (stick[ix_nunchuk_stick].y > nunchukStickMaxY) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_UP; else
  if (stick[ix_nunchuk_stick].y < nunchukStickMinY) cstate[ix_nunchuk_device]|= NUNCHUK_BTN_STICK_DOWN;
}

void WiimoteMessageThread::cwiid_process_nunchuk_acc(uint8 cwiid_acc[3]) {
  cstate[ix_nunchuk_device] &= NUNCHUK_TILT_NOTMASK;
  calcAccelerometerValues(cwiid_acc, calibration[ix_nunchuk_device], acc[ix_nunchuk_device]);

  if (acc[ix_wiimote_device].pitch < -45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_FRONT; else
  if (acc[ix_wiimote_device].pitch >  45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_BACK;
  if (acc[ix_wiimote_device].roll >  45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_RIGHT; else
  if (acc[ix_wiimote_device].roll < -45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_LEFT;
  emit dbusNunchukAcc(m_id, acc[ix_nunchuk_device]);
}

void WiimoteMessageThread::cwiid_process_nunchuk_status(cwiid_ext_type type) {
  switch (type) {
  case CWIID_EXT_NONE:
    if (deviceAvailable(ix_nunchuk_device)) {
      cwiid_process_nunchuk_clear();
      emit dbusNunchukUnplugged(m_id);
    }
    break;

  case CWIID_EXT_NUNCHUK:
    if (!deviceAvailable(ix_nunchuk_device)) {
      setDeviceAvailable(ix_nunchuk_device, true);
      emit dbusNunchukPlugged(m_id);
    }
    break;
  default:
    break;
  }
}

