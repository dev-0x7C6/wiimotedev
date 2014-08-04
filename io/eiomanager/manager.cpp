/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2014  Bartłomiej Burdukiewicz                               *
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

#include <QFile>
#include <QSettings>

#include "helper/hashcompare.h"
#include "eiomanager/manager.h"

extern QMap <QString, uint64> devicebuttons;
extern QMap <QString, uint> scancodes;

const QRegExp deviceEventRegExp(".*(\\[.*(\\d+)\\])");

UInputProfileManager::UInputProfileManager(QObject *parent) : QObject(parent),
  dbusDeviceEventsIface(new WiimotedevDeviceEvents(
                          WIIMOTEDEV_DBUS_SERVICE_NAME,
                          WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                          QDBusConnection::systemBus(), this)),
  dbusProfileManager(new DBusProfileManagerAdaptorWrapper(this, QDBusConnection::systemBus())),
  dbusService(new DBusServiceAdaptorWrapper(this, QDBusConnection::systemBus())),
  dbusCustomJobs(new DBusCustomJobsAdaptorWrapper(this, QDBusConnection::systemBus())),
  disableNunchukExtShift(false),
  disableNunchukExtShake(false),
  disableNunchukExtTilt(false),
  disableWiiremoteShift(false),
  disableWiiremoteShake(false),
  disableWiiremoteTilt(false),
  disableKeyboardModule(true),
  enableWiiremoteInfraredMouse(false),
  rumbleStatus(false),
  virtualEvent(new EIO_EventDevice()) {
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteGeneralButtons(uint, uint64)), this, SLOT(dbusWiimoteGeneralButtons(uint, uint64)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteButtons(uint, uint64)), this, SLOT(dbusWiimoteButtons(uint, uint64)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteAcc(uint, accdata)), this, SLOT(dbusWiimoteAcc(uint, accdata)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukButtons(uint, uint64)), this, SLOT(dbusNunchukButtons(uint, uint64)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukStick(uint, stickdata)), this, SLOT(dbusNunchukStick(uint, stickdata)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukAcc(uint, accdata)), this, SLOT(dbusNunchukAcc(uint, accdata)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerButtons(uint, uint64)), this, SLOT(dbusClassicControllerButtons(uint, uint64)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerLStick(uint, stickdata)), this, SLOT(dbusClassicControllerLStick(uint, stickdata)), Qt::DirectConnection);
  connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerRStick(uint, stickdata)), this, SLOT(dbusClassicControllerRStick(uint, stickdata)), Qt::DirectConnection);
  virtualEvent->uinput_open();
  initializeCommandEvents();
  QDBusConnection::systemBus().registerService("org.wiimotedev.io");
  dbusWiimoteGeneralButtons(1, 0);
}

QHash <uint, uint64> UInputProfileManager::extractDeviceEvent(QString input) {
  QStringList list = input.remove(QRegExp("[ ]")).toLower().split('+');
  QHash <uint, uint64> values;
  bool result = false;
  uint index;
  uint64 value;
  foreach(const QString & item, list) {
    deviceEventRegExp.exactMatch(item);
    index = deviceEventRegExp.cap(2).toUInt();
    value = devicebuttons.value(deviceEventRegExp.cap(0).remove(deviceEventRegExp.cap(1)), 0);
    values.insert(index, values.value(index, 0) | value);
    result = result || (value);
  }

  if (!result)
    return (QHash <uint, uint64>());
  else
    return values;
}

QList <uint> UInputProfileManager::extractScancodes(QStringList list) {
  QList <uint> values;

  for (register int i = 0; i < list.count(); ++i)
    if (scancodes.value(list.at(i), QString(list.at(i)).toUInt()))
      values << scancodes.value(list.at(i), QString(list.at(i)).toUInt());

  return values;
}


void UInputProfileManager::dbusWiimoteGeneralButtons(uint id, uint64 buttons) {
  if (disableNunchukExtShift) buttons &= ~NUNCHUK_SHIFT_MASK;

  if (disableNunchukExtShake) buttons &= ~NUNCHUK_BTN_SHIFT_SHAKE;

  if (disableNunchukExtTilt) buttons &= ~NUNCHUK_TILT_MASK;

  if (disableWiiremoteShift) buttons &= ~WIIMOTE_SHIFT_MASK;

  if (disableWiiremoteShake) buttons &= ~WIIMOTE_BTN_SHIFT_SHAKE;

  if (disableWiiremoteTilt) buttons &= ~WIIMOTE_TILT_MASK;

  if (lastWiiremoteButtons.value(id, -1) == buttons)
    return;

  lastWiiremoteButtons[id] = buttons;
  processCommandEvents();
}

bool UInputProfileManager::loadProfile(QString file) {
  if (!QFile::exists(file))
    return false;

  freeJoystickEvents();
  QSettings settings(file, QSettings::IniFormat);
  foreach(const QString & key, settings.childGroups()) {
    const QString &module = settings.value(key + "/module", QString()).toString();

    if (module.isEmpty())
      continue;

    module.toLower();

    if (module == QString::fromUtf8("joystick"))
      assignJoystickEvents(key, settings);
    else if (module == QString::fromUtf8("keyboard"))
      assignKeyboardEvents(key, settings);
    else if (module == QString::fromUtf8("infrared"))
      assignInfraredEvents(key, settings);
  }
  loadCommandEvents(settings);
  //  loadGamepadEvents(settings);
  loadInfraredEvents(settings);
  return true;
}

bool UInputProfileManager::unloadProfile() {
  unloadCommandEvents();
  freeJoystickEvents();
  unloadInfraredEvents();
  freeKeyboardEvents();
  return true;
}


UInputProfileManager::~UInputProfileManager() {
  unloadProfile();
  virtualEvent->uinput_close(false);
  delete virtualEvent;
}
