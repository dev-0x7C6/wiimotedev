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

#include <QReadWriteLock>
#include <QElapsedTimer>

#include <qmath.h>

WiimotedevConnection::WiimotedevConnection(WiimotedevDevice *device, int id, QObject *parent) :
  QThread(parent),
  m_device(device),
  m_variable_locker(new QReadWriteLock()),
  m_device_locker(new QReadWriteLock()),
  m_threadQuit(false),
  m_id(id),
  m_currentLatency(0),
  m_averageLatency(0),
  m_bufferLatency(0),
  m_bufferCounter(0),
  m_virtualCursorVisible(false)
{
  setTerminationEnabled(true);
  m_device->moveToThread(this);
}

WiimotedevConnection::~WiimotedevConnection() {
  delete m_device;
  delete m_variable_locker;
  delete m_device_locker;
}

void WiimotedevConnection::run() {
  int timeout = 0;
  do {
    msleep(10);
    timeout += 10;
  } while (!threadQuitState() && timeout < 1000);

  connect_animation();


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

  QElapsedTimer *m_elapsed = new QElapsedTimer();
  QElapsedTimer *m_powersave = new QElapsedTimer();
  QElapsedTimer *m_updateState = new QElapsedTimer();
  m_virtualCursor = new VirtualCursor;
  m_elapsed->start();
  m_powersave->start();
  m_updateState->start();

  cwiid_process_wiimote_init();
  cwiid_process_classic_init();
  cwiid_process_nunchuk_init();

  m_device_locker->lockForRead();
  m_device->requestStatus();
  m_device_locker->unlock();

  emit dbusWiimoteConnected(m_id);

  do {
    setDeviceCurrentLatency(m_elapsed->elapsed());
    m_elapsed->restart();
    m_bufferLatency += deviceCurrentLatency();
    m_bufferCounter++;

    if (m_bufferLatency >= 1000) {
      setDeviceAverageLatency(m_bufferLatency / m_bufferCounter);
      m_bufferLatency = deviceCurrentLatency();
      m_bufferCounter = 1;
    }

    m_device_locker->lockForRead();
    m_device->fetchMessage(&count, &mesg, &time);

    if (m_updateState->hasExpired(60000)) {
      m_device->requestStatus();
      m_updateState->restart();
    }
    m_device_locker->unlock();

    for (register int i = 0; i < count; ++i) {
      switch (mesg[i].type) {
        case CWIID_MESG_ERROR:
          cwiid_process_wiimote_error();
          break;

        case CWIID_MESG_STATUS:
          cwiid_process_wiimote_status(mesg[i].status_mesg.battery);
          cwiid_process_classic_status(mesg[i].status_mesg.ext_type);
          cwiid_process_nunchuk_status(mesg[i].status_mesg.ext_type);
          break;

        case CWIID_MESG_IR:
          if (deviceAvailable(ix_wiimote_device))
            cwiid_process_wiimote_ir(mesg[i].ir_mesg.src);
          break;
        case CWIID_MESG_BTN:
          if (deviceAvailable(ix_wiimote_device))
            cwiid_process_wiimote_buttons(mesg[i].btn_mesg.buttons);
          break;

        case CWIID_MESG_ACC:
          if (deviceAvailable(ix_wiimote_device))
            cwiid_process_wiimote_acc(mesg[i].acc_mesg.acc);
          break;

        case CWIID_MESG_CLASSIC:
          if (deviceAvailable(ix_classic_device)) {
            cwiid_process_classic_buttons(mesg[i].classic_mesg.buttons);
            cwiid_process_classic_lstick(mesg[i].classic_mesg.l_stick);
            cwiid_process_classic_rstick(mesg[i].classic_mesg.r_stick);
          }
          break;

        case CWIID_MESG_NUNCHUK:
          if (deviceAvailable(ix_nunchuk_device)) {
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

  } while (!threadQuitState());

  cwiid_process_wiimote_done();
  cwiid_process_classic_done();
  cwiid_process_nunchuk_done();

  if (m_device->isConnected()) {
//FIXME: Animation sometimes freeze wiiremote firmware
//    if (!threadQuitState())
//      disconnect_animation();
    m_device->disconnectFromDevice();
  }

  delete m_elapsed;
  delete m_updateState;
  delete m_virtualCursor;

  emit dbusWiimoteDisconnected(m_id);
}

double WiimotedevConnection::calcVirtualCursorDiff(double c1[], double c2[]) {
  return (sqrt(pow(abs(c2[0] - c1[0]), 2) + pow(abs(c2[1] - c1[1]), 2)));
}

void WiimotedevConnection::setThreadQuitState(bool quit) {
  QWriteLocker locker(m_variable_locker);
  m_threadQuit = quit;
}

bool WiimotedevConnection::threadQuitState() {
  QReadLocker locker(m_variable_locker);
  return m_threadQuit;
}

void WiimotedevConnection::setDeviceAvailable(DeviceType dev, bool available) {
  QWriteLocker locker(m_variable_locker);
  m_available[dev] = available;
}

bool WiimotedevConnection::deviceAvailable(DeviceType dev) {
  QReadLocker locker(m_variable_locker);
  return m_available[dev];
}

void WiimotedevConnection::setDeviceBatteryState(double state) {
  QWriteLocker locker(m_variable_locker);
  m_batteryLife = state;
}

double WiimotedevConnection::deviceBatteryState() {
  QReadLocker locker(m_variable_locker);
  return m_batteryLife;
}

void WiimotedevConnection::setDeviceCurrentLatency(uint latency) {
  QWriteLocker locker(m_variable_locker);
  m_currentLatency = latency;
}

uint WiimotedevConnection::deviceCurrentLatency() {
  QReadLocker locker(m_variable_locker);
  return m_currentLatency;
}

void WiimotedevConnection::setDeviceAverageLatency(uint latency) {
  QWriteLocker locker(m_variable_locker);
  m_averageLatency = latency;
}

uint WiimotedevConnection::deviceAverageLatency() {
  QReadLocker locker(m_variable_locker);
  return m_averageLatency;
}

void WiimotedevConnection::setPowerSafeTimeout(int64 timeout) {
  QWriteLocker locker(m_variable_locker);
  m_powerSaveTimeout = timeout;
}

int64 WiimotedevConnection::powerSafeTimeout() {
  QReadLocker locker(m_variable_locker);
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

    out.pitch = atan(-y / -z * cos(-out.roll))*-59.0;
    out.roll *= 58.8;
  }
}

bool WiimotedevConnection::dbusIsClassicConnected() {
  QReadLocker locker(m_variable_locker);
  return m_available[ix_classic_device];
}

bool WiimotedevConnection::dbusIsNunchukConnected() {
  QReadLocker locker(m_variable_locker);
  return m_available[ix_nunchuk_device];
}

bool WiimotedevConnection::dbusIsWiimoteConnected() {
  QReadLocker locker(m_variable_locker);
  return m_available[ix_wiimote_device];
}

QList< uint> WiimotedevConnection::dbusNunchukGetAccelerometrCalibration() {
  QReadLocker locker(m_variable_locker);
  QList < uint> params;
  for (register int i = 0; i < 3; ++i) {
    params << calibration[ix_nunchuk_device].one[i];
    params << calibration[ix_nunchuk_device].zero[i];
  }

  return params;
}

QList< uint> WiimotedevConnection::dbusWiimoteGetAccelerometrCalibration() {
  QReadLocker locker(m_variable_locker);
  QList < uint> params;
  for (register int i = 0; i < 3; ++i) {
    params << calibration[ix_wiimote_device].one[i];
    params << calibration[ix_wiimote_device].zero[i];
  }

  return params;
}

uint WiimotedevConnection::dbusWiimoteGetAverageLatency() {
  QReadLocker locker(m_variable_locker);
  return m_averageLatency;
}

uint WiimotedevConnection::dbusWiimoteGetBatteryLife() {
  QReadLocker locker(m_variable_locker);
  return m_batteryLife;
}

uint WiimotedevConnection::dbusWiimoteGetCurrentLatency() {
  QReadLocker locker(m_variable_locker);
  return m_currentLatency;
}

QString WiimotedevConnection::dbusWiimoteGetMacAddress() {
  QWriteLocker locker(m_device_locker);
  return m_device->getWiimoteSAddr();
}

uint8 WiimotedevConnection::dbusWiimoteGetLedStatus() {
  QWriteLocker locker(m_device_locker);
  return m_device->getLedStatus();
}

bool WiimotedevConnection::dbusWiimoteGetRumbleStatus() {
  QWriteLocker locker(m_device_locker);
  return m_device->getRumbleStatus();
}

uint8 WiimotedevConnection::dbusWiimoteGetStatus() {
  bool result = 0;
  if (deviceAvailable(ix_wiimote_device))
    result |= STATUS_WIIMOTE_CONNECTED;
  if (deviceAvailable(ix_nunchuk_device))
    result |= STATUS_NUNCHUK_CONNECTED;
  if (deviceAvailable(ix_classic_device))
    result |= STATUS_CLASSIC_CONNECTED;

  return result;
}

bool WiimotedevConnection::dbusWiimoteSetLedStatus(uint status) {
  QWriteLocker locker(m_device_locker);
  return m_device->setLedStatus(status);
}

bool WiimotedevConnection::dbusWiimoteSetRumbleStatus(bool status) {
  QWriteLocker locker(m_device_locker);
  return m_device->setRumbleStatus(status);
}


