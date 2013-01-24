/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#include "wiimotedevconnection.h"
#include "wiimotedevdevice.h"

void WiimotedevConnection::cwiid_process_wiimote_init() {
  cwiid_process_wiimote_clear();
  setDeviceAvailable(ix_wiimote_device, true);
}

void WiimotedevConnection::cwiid_process_wiimote_done() {
  cwiid_process_wiimote_clear();
  setDeviceAvailable(ix_wiimote_device, false);
}

void WiimotedevConnection::cwiid_process_wiimote_clear() {
  cstate[ix_wiimote_device] = lstate[ix_wiimote_device] =
  cstate[ix_general_device] = lstate[ix_general_device] = 0x00;
}

void WiimotedevConnection::cwiid_process_wiimote_buttons(uint16 buttons) {
  cstate[ix_wiimote_device] &= WIIMOTE_BUTTON_NOTMASK;
  if (buttons & CWIID_BTN_1) cstate[ix_wiimote_device] |= WIIMOTE_BTN_1;
  if (buttons & CWIID_BTN_2) cstate[ix_wiimote_device] |= WIIMOTE_BTN_2;
  if (buttons & CWIID_BTN_A) cstate[ix_wiimote_device] |= WIIMOTE_BTN_A;
  if (buttons & CWIID_BTN_B) cstate[ix_wiimote_device] |= WIIMOTE_BTN_B;
  if (buttons & CWIID_BTN_MINUS) cstate[ix_wiimote_device] |= WIIMOTE_BTN_MINUS;
  if (buttons & CWIID_BTN_PLUS) cstate[ix_wiimote_device] |= WIIMOTE_BTN_PLUS;
  if (buttons & CWIID_BTN_HOME) cstate[ix_wiimote_device] |= WIIMOTE_BTN_HOME;
  if (buttons & CWIID_BTN_RIGHT) cstate[ix_wiimote_device] |= WIIMOTE_BTN_RIGHT;
  if (buttons & CWIID_BTN_LEFT) cstate[ix_wiimote_device] |= WIIMOTE_BTN_LEFT;
  if (buttons & CWIID_BTN_DOWN) cstate[ix_wiimote_device] |= WIIMOTE_BTN_DOWN;
  if (buttons & CWIID_BTN_UP) cstate[ix_wiimote_device] |= WIIMOTE_BTN_UP;
}

void WiimotedevConnection::cwiid_process_wiimote_acc(uint8 cwiid_acc[3]) {
  cstate[ix_wiimote_device] &= WIIMOTE_TILT_NOTMASK;
  calcAccelerometerValues(cwiid_acc, calibration[ix_wiimote_device], acc[ix_wiimote_device]);

  if (acc[ix_wiimote_device].pitch < -45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_FRONT; else
  if (acc[ix_wiimote_device].pitch > 45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_BACK;
  if (acc[ix_wiimote_device].roll > 45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_RIGHT; else
  if (acc[ix_wiimote_device].roll < -45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_LEFT;
  emit dbusWiimoteAcc(m_id, acc[ix_wiimote_device]);
}

void WiimotedevConnection::cwiid_process_wiimote_ir(cwiid_ir_src ir[]) {
  current_ir_table.clear();

  irpoint point;
  for (register int i = 0; i < 4; ++i) if (ir[i].valid) {
    point.size =  (ir[i].size <= 0) ? 1 : ir[i].size;
    point.x = ir[i].pos[0];
    point.y = ir[i].pos[1];
    current_ir_table << point;
  }

  if (current_ir_table.count())
    emit dbusWiimoteInfrared(m_id, current_ir_table);

  if (m_virtualCursor->calculate(current_ir_table, acc[ix_wiimote_device].roll)) {
    if (!m_virtualCursorVisible) {
      emit dbusVirtualCursorFound(m_id);
      m_virtualCursorVisible = true;
    }
    emit dbusVirtualCursorPosition(m_id, m_virtualCursor->cursor().x(), m_virtualCursor->cursor().y(),
                                   m_virtualCursor->distance(), -m_virtualCursor->angle());
  } else {
    if (m_virtualCursorVisible) {
      emit dbusVirtualCursorLost(m_id);
      m_virtualCursorVisible = false;
    }
  }

  last_ir_table = current_ir_table;
}

void WiimotedevConnection::cwiid_process_wiimote_error() {
  m_device->disconnectFromDevice(false);
  setThreadQuitState(true);
}

void WiimotedevConnection::cwiid_process_wiimote_status(uint8 battery) {
  setDeviceBatteryState(100.0 * double(battery)/double(CWIID_BATTERY_MAX));
  emit dbusWiimoteBatteryLife(m_id, deviceBatteryState());
}
