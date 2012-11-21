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

#ifndef WIIMOTEMESSAGETHREAD_H
#define WIIMOTEMESSAGETHREAD_H

#include <QThread>
#include "adaptors/deviceevents.h"
#include "3rdparty/libcwiid/cwiid.h"

#include "virtualcursor.h"

class QMutex;
class QElapsedTimer;
class WiimoteDevice;

const quint8 classicLToleranceValue = (0x3F >> 2);
const quint8 classicRToleranceValue = (0x1F >> 2);
const quint8 classicLStickMaxX = (0x3F >> 1) + classicLToleranceValue;
const quint8 classicLStickMinX = (0x3F >> 1) - classicLToleranceValue;
const quint8 classicLStickMaxY = (0x3F >> 1) + classicLToleranceValue;
const quint8 classicLStickMinY = (0x3F >> 1) - classicLToleranceValue;
const quint8 classicRStickMaxX = (0x1F >> 1) + classicRToleranceValue;
const quint8 classicRStickMinX = (0x1F >> 1) - classicRToleranceValue;
const quint8 classicRStickMaxY = (0x1F >> 1) + classicRToleranceValue;
const quint8 classicRStickMinY = (0x1F >> 1) - classicRToleranceValue;
const quint8 nunchukToleranceValue = (0xFF >> 2);
const quint8 nunchukStickMaxX = (0xFF >> 1) + nunchukToleranceValue;
const quint8 nunchukStickMinX = (0xFF >> 1) - nunchukToleranceValue;
const quint8 nunchukStickMaxY = (0xFF >> 1) + nunchukToleranceValue;
const quint8 nunchukStickMinY = (0xFF >> 1) - nunchukToleranceValue;

class WiimoteMessageThread : public QThread
{
  Q_OBJECT
private:
  WiimoteDevice *m_device;
  QMutex *m_mutex;
  bool m_threadQuit;
  double m_batteryLife;
  qint64 m_powerSaveTimeout;

  enum DeviceType {
    ix_wiimote_device = 0,
    ix_nunchuk_device,
    ix_classic_device,
    ix_general_device,
    ix_all_devices
  };

  enum StickIndex {
    ix_classic_lstick = 0,
    ix_classic_rstick,
    ix_nunchuk_stick,
    ix_all_sticks
  };

  enum AxisIndex {
    ix_x_axis = 0,
    ix_y_axis
  };

  enum StateIndex {
    ix_max = 0,
    ix_min
  };

  VirtualCursor *m_virtualCursor;

  struct stickdata stick[ix_all_sticks];
  quint64 cstate[ix_all_devices];
  quint64 lstate[ix_all_devices];
  bool m_available[ix_general_device];
  bool m_nunchukConnected;
  bool m_classicConnected;

  struct acc_cal calibration[ix_all_devices - 1];
  struct accdata acc[ix_all_devices - 1];

  int m_id;
  struct acc_cal nunchukAcc;
  struct acc_cal wiimoteAcc;

  QList < struct irpoint > current_ir_table;
  QList < struct irpoint > last_ir_table;

  double m_currentLatency;
  double m_averageLatency;
  double m_bufferLatency;
  qint32 m_bufferCounter;

private:
  void cwiid_process_wiimote_init();
  void cwiid_process_wiimote_done();
  void cwiid_process_wiimote_clear();

  void cwiid_process_wiimote_buttons(quint16 buttons);
  void cwiid_process_wiimote_acc(quint8 cwiid_acc[3]);
  void cwiid_process_wiimote_ir(cwiid_ir_src ir[4]);
  void cwiid_process_wiimote_status(quint8 battery);
  void cwiid_process_wiimote_error();

  void cwiid_process_classic_init();
  void cwiid_process_classic_done();
  void cwiid_process_classic_clear();

  void cwiid_process_classic_lstick(quint8 cwiid_stick[2]);
  void cwiid_process_classic_rstick(quint8 cwiid_stick[2]);
  void cwiid_process_classic_buttons(quint16 cwiid_buttons);
  void cwiid_process_classic_status(cwiid_ext_type type);

  void cwiid_process_nunchuk_init();
  void cwiid_process_nunchuk_done();
  void cwiid_process_nunchuk_clear();

  void cwiid_process_nunchuk_buttons(quint8 cwiid_buttons);
  void cwiid_process_nunchuk_stick(quint8 cwiid_stick[2]);
  void cwiid_process_nunchuk_acc(quint8 cwiid_acc[3]);
  void cwiid_process_nunchuk_status(cwiid_ext_type type);

  double calcVirtualCursorDiff(double c1[], double c2[]);
  void calcAccelerometerValues(quint8 acc[3], acc_cal &cal, accdata &out);

  void connect_animation();
  void disconnect_animation();

public:
  explicit WiimoteMessageThread(WiimoteDevice *device, int id, QObject *parent = 0);

  quint32 id() { return m_id; }

  void setThreadQuitState(bool quit = true);
  bool threadQuitState();

  void setDeviceAvailable(DeviceType dev, bool available);
  bool deviceAvailable(DeviceType dev);

  void setDeviceBatteryState(double state);
  double deviceBatteryState();

  void setDeviceCurrentLatency(quint32 latency);
  quint32 deviceCurrentLatency();

  void setDeviceAverageLatency(quint32 latency);
  quint32 deviceAverageLatency();

  void setPowerSafeTimeout(qint64 timeout);
  qint64 powerSafeTimeout();

public:
  bool dbusIsClassicConnected();
  bool dbusIsNunchukConnected();
  bool dbusIsWiimoteConnected();

  QList< uint> dbusNunchukGetAccelerometrCalibration();
  QList< uint> dbusWiimoteGetAccelerometrCalibration();

  quint32 dbusWiimoteGetAverageLatency();
  quint32 dbusWiimoteGetBatteryLife();
  quint32 dbusWiimoteGetCurrentLatency();
  QString dbusWiimoteGetMacAddress();

  quint8 dbusWiimoteGetLedStatus();

  bool dbusWiimoteGetRumbleStatus();
  bool dbusWiimoteSetLedStatus(quint32 status);
  bool dbusWiimoteSetRumbleStatus(bool status);
  quint8 dbusWiimoteGetStatus();

protected:
  void run();

signals:
  void dbusVirtualCursorPosition(quint32, double, double, double, double);
  void dbusVirtualCursorFound(quint32);
  void dbusVirtualCursorLost(quint32);
  void dbusWiimoteGeneralButtons(quint32, quint64);
  void dbusWiimoteConnected(quint32);
  void dbusWiimoteDisconnected(quint32);
  void dbusWiimoteBatteryLife(quint32, quint8);
  void dbusWiimoteButtons(quint32, quint64);
  void dbusWiimoteInfrared(quint32, QList< struct irpoint>);
  void dbusWiimoteAcc(quint32, struct accdata);
  void dbusNunchukPlugged(quint32);
  void dbusNunchukUnplugged(quint32);
  void dbusNunchukButtons(quint32, quint64);
  void dbusNunchukStick(quint32, struct stickdata);
  void dbusNunchukAcc(quint32, struct accdata);
  void dbusClassicControllerPlugged(quint32);
  void dbusClassicControllerUnplugged(quint32);
  void dbusClassicControllerButtons(quint32, quint64);
  void dbusClassicControllerLStick(quint32, struct stickdata);
  void dbusClassicControllerRStick(quint32, struct stickdata);
};

#endif // WIIMOTEMESSAGETHREAD_H
