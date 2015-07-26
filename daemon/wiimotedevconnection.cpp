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

#include <QElapsedTimer>

#include <memory>
#include <qmath.h>
#include <sched.h>

#include "wiimotedevconnection.h"
#include "wiimotedevdevice.h"

WiimotedevConnection::WiimotedevConnection(WiimotedevDevice *device, int id, QObject *parent) :
  QThread(parent),
  m_device(device),
  m_interrupted(false),
  m_id(id),
  m_currentLatency(0),
  m_averageLatency(0),
  m_bufferLatency(0),
  m_bufferCounter(0),
  m_virtualCursorVisible(false) {
  setTerminationEnabled(true);
  m_device->moveToThread(this);
}

WiimotedevConnection::~WiimotedevConnection() {
}

uint WiimotedevConnection::id() const {
  return m_id;
}

void WiimotedevConnection::interrupt() {
  m_interrupted = true;
}

void WiimotedevConnection::run() {
  int timeout = 0;

  do {
    msleep(10);
    timeout += 10;
  } while (!m_interrupted && timeout < 1000);

  connect_animation();
  wcounter = 0;
  ncounter = 0;
  calibration[ix_wiimote_device].one[0x00] = 0x00;
  calibration[ix_wiimote_device].one[0x01] = 0x00;
  calibration[ix_wiimote_device].one[0x02] = 0x00;
  calibration[ix_wiimote_device].zero[0x00] = 0x00;
  calibration[ix_wiimote_device].zero[0x01] = 0x00;
  calibration[ix_wiimote_device].zero[0x02] = 0x00;
  m_device->requestCallibration(CWIID_EXT_NONE, &calibration[ix_wiimote_device]);
  m_device->setReportMode();
  int count;
  union cwiid_mesg *mesg;
  struct timespec time;
  cstate[ix_general_device] = 0x00;
  lstate[ix_general_device] = 0x00;
  std::unique_ptr<QElapsedTimer> m_elapsed = std::make_unique<QElapsedTimer>();
  std::unique_ptr<QElapsedTimer> m_powersave = std::make_unique<QElapsedTimer>();
  std::unique_ptr<QElapsedTimer> m_updateState = std::make_unique<QElapsedTimer>();
  m_virtualCursor = std::make_unique<VirtualCursor>();
  m_elapsed->start();
  m_powersave->start();
  m_updateState->start();
  cwiid_process_wiimote_init();
  cwiid_process_classic_init();
  cwiid_process_nunchuk_init();
  m_device->requestStatus();
  emit dbusWiimoteConnected(m_id);

  do {
    m_currentLatency = m_elapsed->elapsed();
    m_elapsed->restart();
    m_bufferLatency += m_currentLatency;
    m_bufferCounter++;

    if (m_bufferLatency >= 1000) {
      m_averageLatency = (m_bufferLatency / m_bufferCounter);
      m_bufferLatency = m_currentLatency;
      m_bufferCounter = 1;
    }

    m_device->fetchMessage(&count, &mesg, &time);

    if (m_updateState->hasExpired(1000)) {
      m_device->requestStatus();
      m_updateState->restart();
    }

    for (register int i = 0; i < count; ++i) {
      switch (mesg[i].type) {
        case CWIID_MESG_ERROR:
          cwiid_process_wiimote_error();
          break;

        case CWIID_MESG_STATUS:
          cwiid_process_wiimote_status(mesg[i].status_mesg.battery);
          cwiid_process_classic_status(mesg[i].status_mesg.ext_type);
          cwiid_process_nunchuk_status(mesg[i].status_mesg.ext_type);
          cwiid_process_motionplus_status(mesg[i].status_mesg.ext_type);
          break;

        case CWIID_MESG_IR:
          if (m_available[ix_wiimote_device])
            cwiid_process_wiimote_ir(mesg[i].ir_mesg.src);

          break;

        case CWIID_MESG_BTN:
          if (m_available[ix_wiimote_device])
            cwiid_process_wiimote_buttons(mesg[i].btn_mesg.buttons);

          break;

        case CWIID_MESG_MOTIONPLUS:
          cwiid_process_motionplus(mesg[i].motionplus_mesg.angle_rate, mesg[i].motionplus_mesg.low_speed);
          break;

        case CWIID_MESG_ACC:
          if (m_available[ix_wiimote_device])
            cwiid_process_wiimote_acc(mesg[i].acc_mesg.acc);

          break;

        case CWIID_MESG_CLASSIC:
          if (m_available[ix_classic_device]) {
            cwiid_process_classic_buttons(mesg[i].classic_mesg.buttons);
            cwiid_process_classic_lstick(mesg[i].classic_mesg.l_stick);
            cwiid_process_classic_rstick(mesg[i].classic_mesg.r_stick);
          }

          break;

        case CWIID_MESG_NUNCHUK:
          if (m_available[ix_nunchuk_device]) {
            cwiid_process_nunchuk_buttons(mesg[i].nunchuk_mesg.buttons);
            cwiid_process_nunchuk_stick(mesg[i].nunchuk_mesg.stick);
            cwiid_process_nunchuk_acc(mesg[i].nunchuk_mesg.acc);
          }

          break;

        default:
          break;
      }

      if ((cstate[ix_general_device] = cstate[ix_wiimote_device] |
                                       cstate[ix_nunchuk_device] | cstate[ix_classic_device]) != lstate[ix_general_device]) {
        emit dbusWiimoteGeneralButtons(m_id, lstate[ix_general_device] = cstate[ix_general_device]);
        m_powersave->restart();
      }

      if (cstate[ix_wiimote_device] != lstate[ix_wiimote_device])
        emit dbusWiimoteButtons(m_id, lstate[ix_wiimote_device] = cstate[ix_wiimote_device]);

      if (cstate[ix_nunchuk_device] != lstate[ix_nunchuk_device])
        emit dbusNunchukButtons(m_id, lstate[ix_nunchuk_device] = cstate[ix_nunchuk_device]);

      if (cstate[ix_classic_device] != lstate[ix_classic_device])
        emit dbusClassicControllerButtons(m_id, lstate[ix_classic_device] = cstate[ix_classic_device]);
    }

    delete mesg;

    if (m_powersave->elapsed() > powerSafeTimeout())
      break;

    sched_yield();
  } while (!m_interrupted);

  cwiid_process_wiimote_done();
  cwiid_process_classic_done();
  cwiid_process_nunchuk_done();

  if (m_device->isConnected()) {
    if (!m_interrupted)
      disconnect_animation();

    m_device->disconnectFromDevice();
  }

  emit dbusWiimoteDisconnected(m_id);
}

double WiimotedevConnection::calcVirtualCursorDiff(double c1[], double c2[]) {
  return (sqrt(pow(abs(c2[0] - c1[0]), 2) + pow(abs(c2[1] - c1[1]), 2)));
}

void WiimotedevConnection::setPowerSafeTimeout(int64 timeout) {
  m_powerSaveTimeout = timeout;
}

int64 WiimotedevConnection::powerSafeTimeout() {
  return m_powerSaveTimeout;
}

void WiimotedevConnection::calcAccelerometerValues(uint8 acc[3], acc_cal &cal, accdata &out) {
  register double x = double((out.x = acc[0]) - cal.zero[0]) / double(cal.one[0] - cal.zero[0]);
  register double y = double((out.y = acc[1]) - cal.zero[1]) / double(cal.one[1] - cal.zero[1]);
  register double z = double((out.z = acc[2]) - cal.zero[2]) / double(cal.one[2] - cal.zero[2]);

  if (z != 0.0) {
    out.roll = atan(x / z);

    if (z < 0.0)
      out.roll += 3.14159265358979323 * ((x > 0.0) ? 1 : -1);

    out.pitch = atan(-y / -z * cos(-out.roll)) * -59.0;
    out.roll *= 58.8;
  }
}

bool WiimotedevConnection::dbusIsClassicConnected() {
  return m_available[ix_classic_device];
}

bool WiimotedevConnection::dbusIsNunchukConnected() {
  return m_available[ix_nunchuk_device];
}

bool WiimotedevConnection::dbusIsWiimoteConnected() {
  return m_available[ix_wiimote_device];
}

QList<uint> WiimotedevConnection::dbusNunchukGetAccelerometrCalibration() {
  QList <uint> params;

  for (register int i = 0; i < 3; ++i) {
    params << calibration[ix_nunchuk_device].one[i];
    params << calibration[ix_nunchuk_device].zero[i];
  }

  return params;
}

QList<uint> WiimotedevConnection::dbusWiimoteGetAccelerometrCalibration() {
  QList <uint> params;

  for (register int i = 0; i < 3; ++i) {
    params << calibration[ix_wiimote_device].one[i];
    params << calibration[ix_wiimote_device].zero[i];
  }

  return params;
}

uint WiimotedevConnection::dbusWiimoteGetAverageLatency() {
  return m_averageLatency;
}

uint WiimotedevConnection::dbusWiimoteGetBatteryLife() {
  return m_batteryLife;
}

uint WiimotedevConnection::dbusWiimoteGetCurrentLatency() {
  return m_currentLatency;
}

QString WiimotedevConnection::dbusWiimoteGetMacAddress() {
  return m_device->getWiimoteSAddr();
}

uint8 WiimotedevConnection::dbusWiimoteGetLedStatus() {
  return m_device->getLedStatus();
}

bool WiimotedevConnection::dbusWiimoteGetRumbleStatus() {
  return m_device->getRumbleStatus();
}

uint8 WiimotedevConnection::dbusWiimoteGetStatus() {
  bool result = 0;

  if (m_available[ix_wiimote_device])
    result |= STATUS_WIIMOTE_CONNECTED;

  if (m_available[ix_nunchuk_device])
    result |= STATUS_NUNCHUK_CONNECTED;

  if (m_available[ix_classic_device])
    result |= STATUS_CLASSIC_CONNECTED;

  return result;
}

bool WiimotedevConnection::dbusWiimoteSetLedStatus(uint status) {
  return m_device->setLedStatus(status);
}

bool WiimotedevConnection::dbusWiimoteSetRumbleStatus(bool status) {
  return m_device->setRumbleStatus(status);
}


