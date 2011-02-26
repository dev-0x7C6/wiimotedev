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

#include <QFile>
#include <QSettings>

#include "classes/hashcompare.h"
#include "uinput/manager.h"

extern QMap < QString, quint64> devicebuttons;
extern QMap < QString, quint16> scancodes;

const QRegExp deviceEventRegExp(".*(\\[.*(\\d+)\\])");

UInputProfileManager::UInputProfileManager(QObject *parent) :QObject(parent),
  dbusDeviceEventsIface(new DBusDeviceEventsInterface(
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
  virtualEvent(new UInputEvent())
{
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SLOT(dbusWiimoteButtons(quint32,quint64)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteAcc(quint32,accdata)), this, SLOT(dbusWiimoteAcc(quint32,accdata)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SLOT(dbusNunchukButtons(quint32,quint64)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukStick(quint32,stickdata)), this, SLOT(dbusNunchukStick(quint32,stickdata)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukAcc(quint32,accdata)), this, SLOT(dbusNunchukAcc(quint32,accdata)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SLOT(dbusClassicControllerButtons(quint32,quint64)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerLStick(quint32,stickdata)), this, SLOT(dbusClassicControllerLStick(quint32,stickdata)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerRStick(quint32,stickdata)), this, SLOT(dbusClassicControllerRStick(quint32,stickdata)));

  virtualEvent->uinput_open();

  initializeCommandEvents();

  QDBusConnection::systemBus().registerService("org.wiimotedev.uinput");
  dbusWiimoteGeneralButtons(1, 0);
}

QHash < quint32, quint64> UInputProfileManager::extractDeviceEvent(QString input)
{
  QStringList list = input.remove(QRegExp("[ ]")).toLower().split('+');
  QHash < quint32, quint64> values;
  bool result = false;

  quint32 index;
  quint64 value;

  foreach (const QString &item, list) {
    deviceEventRegExp.exactMatch(item);
    index = deviceEventRegExp.cap(2).toUInt();
    value = devicebuttons.value(deviceEventRegExp.cap(0).remove(deviceEventRegExp.cap(1)), 0);
    values.insert(index, values.value(index, 0) | value);
    result = result || (value);
  }

  if (!result)
    return (QHash < quint32, quint64>()); else
    return values;
}

QList < quint32> UInputProfileManager::extractScancodes(QStringList list)
{
  QList < quint32> values;
  for (register int i = 0; i < list.count(); ++i)
    if (scancodes.value(list.at(i), QString(list.at(i)).toUInt()))
      values << scancodes.value(list.at(i), QString(list.at(i)).toUInt());
  return values;
}


void UInputProfileManager::dbusWiimoteGeneralButtons(quint32 id, quint64 buttons) {
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

  QSettings settings(file, QSettings::IniFormat);

  loadCommandEvents(settings);
  loadGamepadEvents(settings);
  loadInfraredEvents(settings);
  loadKeyboardEvents(settings);

  return true;
}

bool UInputProfileManager::unloadProfile() {
  unloadCommandEvents();
  unloadGamepadEvents();
  unloadInfraredEvents();
  unloadKeyboardEvents();

  return true;
}


UInputProfileManager::~UInputProfileManager()
{
  unloadProfile();

  virtualEvent->uinput_close(false);

  delete virtualEvent;
}
