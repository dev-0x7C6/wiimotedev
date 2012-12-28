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

#ifndef ADAPTORS_DEVICEEVENTS_H
#define ADAPTORS_DEVICEEVENTS_H

#include "adaptors.h"

class DBusDeviceEventsAdaptor : public QDBusAbstractAdaptor
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.deviceEvents")
  Q_CLASSINFO("D-Bus Introspection", ""
" <interface name=\"org.wiimotedev.deviceEvents\">\n"
"    <method name=\"dbusGetWiimoteList\">\n"
"      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
"      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
"    </method>\n"
"    <method name=\"dbusIsClassicConnected\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusIsNunchukConnected\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusIsWiimoteConnected\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </method>\n"
"    <method name=\"dbusNunchukGetAccelerometrCalibration\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
"      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetAccelerometrCalibration\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <annotation value=\"QList&lt;uint>\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\" />\n"
"      <arg direction=\"out\" type=\"au\" name=\"id_list\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetAverageLatency\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetBatteryLife\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetCurrentLatency\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"u\" name=\"latency\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetMacAddress\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"s\" name=\"mac\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetLedStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"led\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteGetRumbleStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"b\" name=\"rumble\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteSetLedStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"in\" type=\"u\" name=\"status\" />\n"
"      <arg direction=\"out\" type=\"b\" name=\"led\" />\n"
"    </method>\n"
"    <method name=\"dbusWiimoteSetRumbleStatus\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"in\" type=\"b\" name=\"status\" />\n"
"      <arg direction=\"out\" type=\"b\" name=\"rumble\" />\n"
"    </method>\n"
"    <signal name=\"dbusClassicControllerButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerLStick\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerPlugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerRStick\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusClassicControllerUnplugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukAcc\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yyydd)\" name=\"acc\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukPlugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukStick\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yy)\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusNunchukUnplugged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteAcc\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"(yyydd)\" name=\"acc\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteBatteryLife\">\n"
"     <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"     <arg direction=\"out\" type=\"y\" name=\"life\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteConnected\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteDisconnected\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteGeneralButtons\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"t\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteInfrared\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"a(nqq)\" name=\"table\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteLedStatusChanged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteRumbleStatusChanged\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"value\" />\n"
"    </signal>\n"
"    <signal name=\"dbusWiimoteStatus\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"y\" name=\"status\" />\n"
"    </signal>\n"
"    <signal name=\"dbusVirtualCursorPosition\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"      <arg direction=\"out\" type=\"d\" name=\"x\" />\n"
"      <arg direction=\"out\" type=\"d\" name=\"y\" />\n"
"      <arg direction=\"out\" type=\"d\" name=\"length\" />\n"
"      <arg direction=\"out\" type=\"d\" name=\"angle\" />\n"
"    </signal>\n"
"    <signal name=\"dbusVirtualCursorFound\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusVirtualCursorLost\">\n"
"      <arg direction=\"out\" type=\"u\" name=\"id\" />\n"
"    </signal>\n"
"    <signal name=\"dbusReportUnregistredWiimote\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"mac\" />\n"
"    </signal>\n"
"  </interface>");

public:
  DBusDeviceEventsAdaptor (QObject *parent);

public Q_SLOTS:
  bool dbusIsClassicConnected(uint id);
  bool dbusIsNunchukConnected(uint id);
  bool dbusIsWiimoteConnected(uint id);

  QList< uint> dbusNunchukGetAccelerometrCalibration(uint id);
  QList< uint> dbusWiimoteGetAccelerometrCalibration(uint id);

  uint dbusWiimoteGetAverageLatency(uint id);
  uint dbusWiimoteGetBatteryLife(uint id);
  uint dbusWiimoteGetCurrentLatency(uint id);
  QString dbusWiimoteGetMacAddress(uint id);

  uint8 dbusWiimoteGetLedStatus(uint id);

  QList < uint> dbusGetWiimoteList();
  bool dbusWiimoteGetRumbleStatus(uint id);
  bool dbusWiimoteSetLedStatus(uint id, uint status);
  bool dbusWiimoteSetRumbleStatus(uint id, bool status);

Q_SIGNALS:
  void dbusReportUnregistredWiimote(QString);
  void dbusWiimoteGeneralButtons(uint, uint64);

  void dbusVirtualCursorPosition(uint, double, double, double, double);
  void dbusVirtualCursorFound(uint);
  void dbusVirtualCursorLost(uint);

  void dbusWiimoteAcc(uint, struct accdata);
  void dbusWiimoteBatteryLife(uint, uint8);
  void dbusWiimoteButtons(uint, uint64);
  void dbusWiimoteConnected(uint);
  void dbusWiimoteDisconnected(uint);
  void dbusWiimoteInfrared(uint, QList< struct irpoint>);
  void dbusWiimoteLedStatusChanged(uint, uint8);
  void dbusWiimoteRumbleStatusChanged(uint, uint8);
  void dbusWiimoteStatus(uint, uint8);

  void dbusNunchukPlugged(uint);
  void dbusNunchukUnplugged(uint);

  void dbusNunchukAcc(uint, struct accdata);
  void dbusNunchukButtons(uint, uint64);
  void dbusNunchukStick(uint, struct stickdata);

  void dbusClassicControllerPlugged(uint);
  void dbusClassicControllerUnplugged(uint);

  void dbusClassicControllerButtons(uint, uint64);
  void dbusClassicControllerLStick(uint, struct stickdata);
  void dbusClassicControllerRStick(uint, struct stickdata);
};

class DBusDeviceEventsAdaptorWrapper : public QObject
{
  Q_OBJECT
private:
  bool registred;

public:
  DBusDeviceEventsAdaptorWrapper(QObject *parent, QDBusConnection &connection);

  inline bool isRegistred() { return registred; }

public Q_SLOTS:
  bool dbusIsClassicConnected(uint id);
  bool dbusIsNunchukConnected(uint id);
  bool dbusIsWiimoteConnected(uint id);

  QList< uint> dbusNunchukGetAccelerometrCalibration(uint id);
  QList< uint> dbusWiimoteGetAccelerometrCalibration(uint id);

  uint dbusWiimoteGetAverageLatency(uint id);
  uint dbusWiimoteGetBatteryLife(uint id);
  uint dbusWiimoteGetCurrentLatency(uint id);
  QString dbusWiimoteGetMacAddress(uint id);

  uint8 dbusWiimoteGetLedStatus(uint id);

  QList < uint> dbusGetWiimoteList();
  QStringList dbusGetUnregistredWiimoteList();

  bool dbusWiimoteGetRumbleStatus(uint id);
  bool dbusWiimoteSetLedStatus(uint id, uint status);
  bool dbusWiimoteSetRumbleStatus(uint id, bool status);

Q_SIGNALS:
  void dbusReportUnregistredWiimote(QString);
  void dbusWiimoteGeneralButtons(uint, uint64);

  void dbusVirtualCursorPosition(uint, double, double, double, double);
  void dbusVirtualCursorFound(uint);
  void dbusVirtualCursorLost(uint);

  void dbusWiimoteAcc(uint, struct accdata);
  void dbusWiimoteBatteryLife(uint, uint8);
  void dbusWiimoteButtons(uint, uint64);
  void dbusWiimoteConnected(uint);
  void dbusWiimoteDisconnected(uint);
  void dbusWiimoteInfrared(uint, QList< struct irpoint>);
  void dbusWiimoteLedStatusChanged(uint, uint8);
  void dbusWiimoteRumbleStatusChanged(uint, uint8);
  void dbusWiimoteStatus(uint, uint8);

  void dbusNunchukPlugged(uint);
  void dbusNunchukUnplugged(uint);

  void dbusNunchukAcc(uint, struct accdata);
  void dbusNunchukButtons(uint, uint64);
  void dbusNunchukStick(uint, struct stickdata);

  void dbusClassicControllerPlugged(uint);
  void dbusClassicControllerUnplugged(uint);

  void dbusClassicControllerButtons(uint, uint64);
  void dbusClassicControllerLStick(uint, struct stickdata);
  void dbusClassicControllerRStick(uint, struct stickdata);
};

#endif // ADAPTORS_DEVICEEVENTS_H
