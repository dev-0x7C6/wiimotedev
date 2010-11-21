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

#include "core/wiiremote.h"

WiimoteDevice::WiimoteDevice(QObject *parent):
  QObject(parent),
  device(0),
  lock(new QReadWriteLock()),
  haveWiimoteCallibration(false),
  haveNunchukCallibration(false),
  isRumble(false),
  switchOnLeds(0),
  reportMode(0)
{
  memset(&bdaddr, 0, sizeof(bdaddr_t));
  memset(&wiimote_acc_cal, 0, sizeof(acc_cal));
  memset(&nunchuk_acc_cal, 0, sizeof(acc_cal));
}

WiimoteDevice::~WiimoteDevice()
{
  if (isConnected())
    disconnectFromDevice(true);
  delete lock;
}

bool WiimoteDevice::connectToDevice(const quint32 timeout)
{
  memset(&bdaddr, 0, sizeof(bdaddr_t));

  if ((device = cwiid_open_timeout(&bdaddr, CWIID_FLAG_MESG_IFC, timeout)))
  {
   // memcpy(&wiimotebdaddr, &bdaddr, sizeof(bdaddr_t));
    id = cwiid_get_id(device);

    return setReportMode(reportMode = defaultReportFlags);
  }

  device = 0;
  return false;
}

bool WiimoteDevice::disconnectFromDevice(const bool switchOfReport) {
  if (isDisconnected())
    return false;

  if (switchOfReport) cwiid_set_rpt_mode(device, 0);

  cwiid_disconnect(device);
  device = 0;
  isRumble = false;
  switchOnLeds = 0;
  reportMode = 0;
  haveWiimoteCallibration = false;
  haveNunchukCallibration = false;
  return true;
}

bool WiimoteDevice::getMesgStruct(int *count, union cwiid_mesg *mesg[], struct timespec *time) {
  if (isDisconnected())
    return false;

  if (cwiid_get_mesg(device, count, mesg, time)) {
    disconnectFromDevice(false);
    return false;
  }

  return true;
}

bool WiimoteDevice::setLedStatus(quint8 led) {
  if (isDisconnected()) return false;

  if (cwiid_set_led(device, switchOnLeds = led)) {
    disconnectFromDevice(false);
    return false;
  }

  return true;
}

bool WiimoteDevice::setRumbleStatus(bool rumble) {
  if (isDisconnected()) return false;
  cwiid_set_rumble(device, rumble);
  return true;
}

bool WiimoteDevice::setReportMode(quint8 mode) {
  if (isDisconnected()) return false;

  if (cwiid_set_rpt_mode(device, reportMode = mode)) {
    disconnectFromDevice(false);
    return false;
  }

  return true;
}

quint8 WiimoteDevice::getLedStatus() {
  if (isDisconnected()) return 0;

  return switchOnLeds;
}

bool WiimoteDevice::getRumbleStatus() {
  if (isDisconnected())
    return false;

  return isRumble;
}

quint8 WiimoteDevice::getReportMode() {
  if (isDisconnected())
    return 0;

  return reportMode;
}

QString WiimoteDevice::getWiimoteSAddr() {
  char addr[17];
  ba2str(&bdaddr, addr);
  return QString::fromAscii(addr, 17);
}

bdaddr_t WiimoteDevice::getWiimoteAddr() {
  return bdaddr;
}

bool WiimoteDevice::getDeviceCallibration(enum cwiid_ext_type ext_type, struct acc_cal *acc_cal) {
  if (isDisconnected())
    return false;

  if (cwiid_get_acc_cal(device, ext_type, acc_cal))
    return false;

  switch (ext_type) {
  case CWIID_EXT_NONE:
    wiimote_acc_cal = *acc_cal;
    haveWiimoteCallibration = true;
    break;
  case CWIID_EXT_NUNCHUK:
    nunchuk_acc_cal = *acc_cal;
    haveWiimoteCallibration = true;
    break;
  }

  return true;
}

struct acc_cal WiimoteDevice::getLastWiimoteCallibration(bool &valid) {
  lock->lockForRead();
  valid = haveWiimoteCallibration;
  struct acc_cal copy;
  memcpy(&copy, &wiimote_acc_cal, sizeof(acc_cal));
  lock->unlock();

  return copy;
}

struct acc_cal WiimoteDevice::getLastNunchukCallibration(bool &valid) {
  lock->lockForRead();
  valid = haveNunchukCallibration;
  struct acc_cal copy;
  memcpy(&copy, &nunchuk_acc_cal, sizeof(acc_cal));
  lock->unlock();

  return copy;
}


bool WiimoteDevice::getWiimoteState(struct cwiid_state &state) {
  if (isDisconnected()) return false;

  if (cwiid_get_state(device, &state)){
    disconnectFromDevice(false);
    return false;
  }

  return true;
}
