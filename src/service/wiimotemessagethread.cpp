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
#include <QElapsedTimer>
#include "3rdparty/libcwiid/cwiid.h"
#include <qmath.h>

WiimoteMessageThread::WiimoteMessageThread(WiimoteDevice *device, int id, QObject *parent) :
  QThread(parent),
  m_device(device),
  m_mutex(new QMutex()),
  m_threadQuit(false),
  m_nunchukConnected(false),
  m_classicConnected(false),
  m_id(id),
  m_currentLatency(0),
  m_averageLatency(0),
  m_bufferLatency(0),
  m_bufferCounter(0)
{
  setTerminationEnabled(true);
  m_device->moveToThread(this);
}

void WiimoteMessageThread::run() {
  lock();
  msleep(1000);
  m_device->setRumbleStatus(true);

  for (register int i = 0; i < 2; ++i) {
    switch (i%2) {
    case 0:
      for (register int j = 0; j < 4; ++j) {
        m_device->setLedStatus(1 << j);
        msleep(20);
      }
      break;
    case 1:
      for (register int j = 3; j >= 0; --j) {
        m_device->setLedStatus(1 << j);
        msleep(20);
      }
      break;
    }
  }
  m_device->setLedStatus(m_id);
  m_device->setRumbleStatus(false);
  bool v;
  m_device->getDeviceCallibration(CWIID_EXT_NONE, &calibration[ix_wiimote_device]);
  m_device->setReportMode();


  unlock();

  int count;
  union cwiid_mesg *mesg;
  struct timespec time;

  cstate[ix_general_device] = 0x00;
  lstate[ix_general_device] = 0x00;

  QElapsedTimer *m_elapsed = new QElapsedTimer();
  m_virtualCursor = new VirtualCursor;
  m_elapsed->start();
  m_mutex->lock();

  cwiid_process_wiimote_init();
  cwiid_process_classic_init();
  cwiid_process_nunchuk_init();

  do {
    setDeviceCurrentLatency(m_elapsed->elapsed());
    m_elapsed->restart();
    m_bufferLatency += m_currentLatency;
    m_bufferCounter++;

    if (m_bufferLatency >= 1000) {
      setDeviceAverageLatency(m_bufferLatency / m_bufferCounter);
      m_bufferLatency = 0;
      m_bufferCounter = 0;
    }

    m_device->getMesgStruct(&count, &mesg, &time);

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
      }

      if ((cstate[ix_general_device] = cstate[ix_wiimote_device] |
           cstate[ix_nunchuk_device] | cstate[ix_classic_device]) != lstate[ix_general_device])
        emit dbusWiimoteGeneralButtons(m_id, lstate[ix_general_device] = cstate[ix_general_device]);

      if (cstate[ix_wiimote_device] != lstate[ix_wiimote_device])
        emit dbusWiimoteButtons(m_id, lstate[ix_wiimote_device] = cstate[ix_wiimote_device]);

      if (cstate[ix_nunchuk_device] != lstate[ix_nunchuk_device])
        emit dbusNunchukButtons(m_id, lstate[ix_nunchuk_device] = cstate[ix_nunchuk_device]);

      if (cstate[ix_classic_device] != lstate[ix_classic_device])
        emit dbusClassicControllerButtons(m_id, lstate[ix_classic_device] = cstate[ix_classic_device]);
    }

    delete mesg;
  } while (!threadQuitState());

  cwiid_process_classic_done();
  cwiid_process_classic_done();
  cwiid_process_nunchuk_done();

  if (m_device->isConnected())
    m_device->disconnectFromDevice();

  delete m_device;
  delete m_elapsed;
  delete m_virtualCursor;
  m_device = 0;

  m_mutex->unlock();
}

double WiimoteMessageThread::calcVirtualCursorDiff(double c1[], double c2[]) {
  return (sqrt(pow(abs(c2[0] - c1[0]), 2) + pow(abs(c2[1] - c1[1]), 2)));
}

void WiimoteMessageThread::setThreadQuitState(bool quit) {
  m_mutex->lock();
  m_threadQuit = quit;
  m_mutex->unlock();
}

bool WiimoteMessageThread::threadQuitState() {
  m_mutex->lock();
  bool result = m_threadQuit;
  m_mutex->unlock();

  return result;
}

void WiimoteMessageThread::setDeviceAvailable(DeviceType dev, bool available) {
  m_mutex->lock();
  m_available[dev] = available;
  m_mutex->unlock();
}

bool WiimoteMessageThread::deviceAvailable(DeviceType dev) {
  m_mutex->lock();
  bool result = m_available[dev];
  m_mutex->unlock();

  return result;
}

void WiimoteMessageThread::setDeviceBatteryState(double state) {
  m_mutex->lock();
  m_batteryLife = state;
  m_mutex->unlock();
}

double WiimoteMessageThread::deviceBatteryState() {
  m_mutex->lock();
  double result = m_batteryLife;
  m_mutex->unlock();

  return result;
}

void WiimoteMessageThread::setDeviceCurrentLatency(quint32 latency) {
  m_mutex->lock();
  m_currentLatency = latency;
  m_mutex->unlock();
}

quint32 WiimoteMessageThread::deviceCurrentLatency() {
  m_mutex->lock();
  quint32 result = m_batteryLife;
  m_mutex->unlock();

  return result;
}

void WiimoteMessageThread::setDeviceAverageLatency(quint32 latency) {
  m_mutex->lock();
  m_batteryLife = latency;
  m_mutex->unlock();
}

quint32 WiimoteMessageThread::deviceAverageLatency() {
  m_mutex->lock();
  double result = m_averageLatency;
  m_mutex->unlock();

  return result;
}

void WiimoteMessageThread::calcAccelerometerValues(quint8 acc[3], acc_cal &cal, accdata &out) {
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

bool WiimoteMessageThread::dbusIsClassicConnected() {
  bool result;
  m_mutex->lock();
  result = deviceAvailable(ix_classic_device);
  m_mutex->unlock();

  return result;
}

bool WiimoteMessageThread::dbusIsNunchukConnected() {
  bool result;
  m_mutex->lock();
  result = deviceAvailable(ix_nunchuk_device);
  m_mutex->unlock();

  return result;
}

bool WiimoteMessageThread::dbusIsWiimoteConnected() {
  bool result;

  m_mutex->lock();
  result = deviceAvailable(ix_wiimote_device);
  m_mutex->unlock();

  return result;
}

QList< uint> WiimoteMessageThread::dbusNunchukGetAccelerometrCalibration() {
  QList < uint> params;

  m_mutex->lock();
  for (register int i = 0; i < 3; ++i) {
    params << calibration[ix_nunchuk_device].one[i];
    params << calibration[ix_nunchuk_device].zero[i];
  }
  m_mutex->unlock();

  return params;
}

QList< uint> WiimoteMessageThread::dbusWiimoteGetAccelerometrCalibration() {
  QList < uint> params;

  m_mutex->lock();
  for (register int i = 0; i < 3; ++i) {
    params << calibration[ix_wiimote_device].one[i];
    params << calibration[ix_wiimote_device].zero[i];
  }
  m_mutex->unlock();

  return params;
}

quint32 WiimoteMessageThread::dbusWiimoteGetAverageLatency() {
  quint32 result;

  m_mutex->lock();
  result = m_averageLatency;
  m_mutex->unlock();

  return result;
}

quint32 WiimoteMessageThread::dbusWiimoteGetBatteryLife() {
  return deviceBatteryState();
}

quint32 WiimoteMessageThread::dbusWiimoteGetCurrentLatency() {
  quint32 result;

  m_mutex->lock();
  result = m_currentLatency;
  m_mutex->unlock();

  return result;
}

QString WiimoteMessageThread::dbusWiimoteGetMacAddress() {
  QString result;

  m_mutex->lock();
  result = m_device->getWiimoteSAddr();
  m_mutex->unlock();

  return result;
}

quint8 WiimoteMessageThread::dbusWiimoteGetLedStatus() {
  quint8 result;

  m_mutex->lock();
  result = m_device->getLedStatus();
  m_mutex->unlock();

  return result;
}

bool WiimoteMessageThread::dbusWiimoteGetRumbleStatus() {
  bool result;

  m_mutex->lock();
  result = m_device->getRumbleStatus();
  m_mutex->unlock();

  return result;
}

quint8 WiimoteMessageThread::dbusWiimoteGetStatus() {
  bool result = 0;
  if (deviceAvailable(ix_wiimote_device))
    result |= STATUS_WIIMOTE_CONNECTED;
  if (deviceAvailable(ix_nunchuk_device))
    result |= STATUS_NUNCHUK_CONNECTED;
  if (deviceAvailable(ix_classic_device))
    result |= STATUS_CLASSIC_CONNECTED;

  return result;
}

bool WiimoteMessageThread::dbusWiimoteSetLedStatus(quint32 status) {
  bool result;

  m_mutex->lock();
  result = m_device->setLedStatus(status);
  m_mutex->unlock();

  return result;
}

bool WiimoteMessageThread::dbusWiimoteSetRumbleStatus(bool status) {
  bool result;

  m_mutex->lock();
  result = m_device->setRumbleStatus(status);
  m_mutex->unlock();

  return result;
}

void WiimoteMessageThread::lock() {
  m_mutex->lock();
}

void WiimoteMessageThread::unlock() {
  m_mutex->unlock();
}

WiimoteDevice* WiimoteMessageThread::device() {
  return m_device;
}

