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

#ifndef WIIMOTE_DEVICE_H
#define WIIMOTE_DEVICE_H

// This is wrapper class, for changes in cwiid API or when wiimotedev-daemon will use
// other library like *libwiimotedev (when is done)

#include "config.h"

#include <QObject>
#include <QReadWriteLock>

#ifdef USE_STATIC_CWIID
#include "3rdparty/libcwiid/cwiid.h"
#else
#include <cwiid.h>
#endif

#include "headers/consts.h"

#define defaultReportFlags \
  CWIID_RPT_STATUS | \
  CWIID_RPT_BTN | \
  CWIID_RPT_IR | \
  CWIID_RPT_NUNCHUK | \
  CWIID_RPT_CLASSIC | \
  CWIID_RPT_ACC

class WiimoteDevice : public QObject
{
private:
  bdaddr_t bdaddr;
  cwiid_wiimote_t *device;
  qint32 id;
  QReadWriteLock *lock;
  bool haveWiimoteCallibration;
  bool haveNunchukCallibration;

private:
  bool isRumble;
  quint8 switchOnLeds;
  quint8 reportMode;
  struct acc_cal wiimote_acc_cal;
  struct acc_cal nunchuk_acc_cal;

public:
  explicit WiimoteDevice(QObject *parent = 0);
  virtual ~WiimoteDevice();

  bool connectToDevice(const quint32 timeout = 3);
  bool disconnectFromDevice(const bool switchOfReport = true);

  bool getMesgStruct(int *count, union cwiid_mesg *mesg[], struct timespec *time);

  inline bool isConnected() { return (device != 0); }
  inline bool isDisconnected() { return (device == 0); }

  bool setLedStatus(quint8 led);
  bool setRumbleStatus(bool rumble);
  bool setReportMode(quint8 mode = defaultReportFlags);

  void requestStatus();

  quint8 getLedStatus();
  bool getRumbleStatus();
  quint8 getReportMode();
  bool getWiimoteState(struct cwiid_state &state);

  bool getDeviceCallibration(enum cwiid_ext_type ext_type, struct acc_cal *acc_cal);
  struct acc_cal getLastWiimoteCallibration(bool &valid);
  struct acc_cal getLastNunchukCallibration(bool &valid);

  QString getWiimoteSAddr();
  bdaddr_t getWiimoteAddr();

};

#endif // WIIMOTE_DEVICE_H
