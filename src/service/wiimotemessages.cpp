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

#include <QMutex>
#include <qmath.h>

void WiimoteMessageThread::cwiid_process_wiimote_init() {
  cwiid_process_wiimote_clear();
  setDeviceAvailable(ix_wiimote_device, true);
}

void WiimoteMessageThread::cwiid_process_wiimote_done() {
  cwiid_process_wiimote_clear();
  setDeviceAvailable(ix_wiimote_device, false);
}

void WiimoteMessageThread::cwiid_process_wiimote_clear() {
  cstate[ix_wiimote_device] = 0x00;
  lstate[ix_wiimote_device] = 0x00;
  cstate[ix_general_device] = 0x00;
  lstate[ix_general_device] = 0x00;
}

void WiimoteMessageThread::cwiid_process_wiimote_buttons(quint16 buttons) {
  cstate[ix_wiimote_device] &= WIIMOTE_BUTTON_NOTMASK;
  if (buttons & CWIID_BTN_1) cstate[ix_wiimote_device] |= WIIMOTE_BTN_1;
  if (buttons & CWIID_BTN_2) cstate[ix_wiimote_device] |= WIIMOTE_BTN_2;
  if (buttons & CWIID_BTN_A) cstate[ix_wiimote_device] |= WIIMOTE_BTN_A;
  if (buttons & CWIID_BTN_B) cstate[ix_wiimote_device] |= WIIMOTE_BTN_B;
  if (buttons & CWIID_BTN_MINUS) cstate[ix_wiimote_device] |= WIIMOTE_BTN_MINUS;
  if (buttons & CWIID_BTN_PLUS)  cstate[ix_wiimote_device] |= WIIMOTE_BTN_PLUS;
  if (buttons & CWIID_BTN_HOME)  cstate[ix_wiimote_device] |= WIIMOTE_BTN_HOME;
  if (buttons & CWIID_BTN_RIGHT) cstate[ix_wiimote_device] |= WIIMOTE_BTN_RIGHT;
  if (buttons & CWIID_BTN_LEFT)  cstate[ix_wiimote_device] |= WIIMOTE_BTN_LEFT;
  if (buttons & CWIID_BTN_DOWN)  cstate[ix_wiimote_device] |= WIIMOTE_BTN_DOWN;
  if (buttons & CWIID_BTN_UP)    cstate[ix_wiimote_device] |= WIIMOTE_BTN_UP;
}

void WiimoteMessageThread::cwiid_process_wiimote_acc(quint8 cwiid_acc[3]) {
  calcAccelerometerValues(cwiid_acc, calibration[ix_wiimote_device], acc[ix_wiimote_device]);
  emit dbusWiimoteAcc(m_id, acc[ix_wiimote_device]);

  cstate[ix_wiimote_device] &= WIIMOTE_TILT_NOTMASK;
  if (acc[ix_wiimote_device].pitch < -45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_FRONT; else
  if (acc[ix_wiimote_device].pitch > 45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_BACK;
  if (acc[ix_wiimote_device].roll > 45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_RIGHT; else
  if (acc[ix_wiimote_device].roll < -45.0) cstate[ix_wiimote_device] |= WIIMOTE_BTN_TILT_LEFT;
}

void WiimoteMessageThread::cwiid_process_wiimote_ir(cwiid_ir_src ir[]) {
  current_ir_table.clear();

  for (register int i = 0; i < 4; ++i) if (ir[i].valid) {
    irpoint point;
    point.size =  (ir[i].size <= 0) ? 1 : ir[i].size;
    point.x = ir[i].pos[0];
    point.y = ir[i].pos[1];
    current_ir_table << point;
  }

  if (current_ir_table.count())
    emit dbusWiimoteInfrared(m_id, current_ir_table);



  if (m_virtualCursor->calculate(current_ir_table, acc[ix_wiimote_device].roll)) {
    emit dbusVirtualCursorPosition(m_id, m_virtualCursor->cursor().x(), m_virtualCursor->cursor().y(),
                                   200, -m_virtualCursor->angle());
  } else {


  }



//      emit dbusVirtualCursorFound(m_id);


//      emit dbusVirtualCursorLost(m_id);
//    virtualCursorLost = true;
//  }

  last_ir_table = current_ir_table;
}

void WiimoteMessageThread::cwiid_process_wiimote_error() {
  m_device->disconnectFromDevice(false);
  setThreadQuitState(true);
}

void WiimoteMessageThread::cwiid_process_wiimote_status(quint8 battery) {
  setDeviceBatteryState(100.0 * double(battery)/double(CWIID_BATTERY_MAX));
}