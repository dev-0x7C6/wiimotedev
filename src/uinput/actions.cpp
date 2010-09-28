/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "classes/hashcompare.h"
#include "uinput/manager.h"

const QRegExp deviceEventRegExp(".*(\\(.*(\\d+)\\))");

void UInputProfileManager::loadCommandEvents(QSettings &settings) {
  unloadCommandEvents();

  QMap < QString, quint8> actionConfigList;
  actionConfigList["actions-bit"] = HashCompare<QString, quint8>::BitCompare;
  actionConfigList["actions"] = HashCompare<QString, quint8>::BitCompare;
  actionConfigList["actions-equal"] = HashCompare<QString, quint8>::EqualCompare;
  actionConfigList["actions-notequal"] = HashCompare<QString, quint8>::NotEqualCompare;

  QMapIterator < QString, quint8> map(actionConfigList);

  while (map.hasNext()) {
    map.next();
    settings.beginGroup(map.key());
    foreach (const QString &string, settings.allKeys()) {
      QStringList params = settings.value(string, QString()).toString().split(QChar(' '));

      qint32 index;

      while ((index = params.indexOf("")) != -1)
        params.removeAt(index);

      if (params.count() < 1)
        continue;

      CommandAction *action = new CommandAction;
      action->event = extractDeviceEvent(string);
      action->params = params;
      action->alghoritm  = actionConfigList[map.key()];
      action->actived = false;
      commandActions << action;
    }
    settings.endGroup();
  }
}

void UInputProfileManager::unloadCommandEvents() {

}

void UInputProfileManager::initializeCommandEvents() {
  commandIds["exec"] = executeAction;
  commandIds["execute"] = executeAction;
  commandIds["rumble"] = rumbleAction;
}

void UInputProfileManager::processCommandEvents() {
  if (commandActions.isEmpty())
    return;

  HashCompare<quint32, quint64> compare;

  foreach (CommandAction *action, commandActions) {
    if (action->event.isEmpty())
      continue;

    bool matched = compare.compare(&action->event, &lastWiiremoteButtons, action->alghoritm);

    if (matched && !action->actived) {
      action->actived = !action->actived;
      activeCommandEvent(action->params);
    } else
    if (!matched && action->actived) {
      action->actived = !action->actived;
      deactiveCommandEvent(action->params);
    }

  }
}


void UInputProfileManager::activeCommandEvent(QStringList &params) {
  switch (commandIds.value(params.at(0))) {
  case rumbleAction:
    dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(QString(params.value(1, "1")).toUInt(), true);
  }
}

void UInputProfileManager::deactiveCommandEvent(QStringList &params) {
  switch (commandIds.value(params.at(0))) {
  case rumbleAction:
    dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(QString(params.value(1, "1")).toUInt(), false);
  }
}
