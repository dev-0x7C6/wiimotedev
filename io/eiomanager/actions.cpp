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

#include "eiomanager/manager.h"
#include "helper/hashcompare.h"

const QRegExp deviceEventRegExp(".*(\\(.*(\\d+)\\))");

void UInputProfileManager::loadCommandEvents(QSettings &settings) {
	unloadCommandEvents();
	QMap<QString, uint8_t> actionConfigList;
	actionConfigList["actions-bit"] = HashCompare<QString, uint8_t>::BitCompare;
	actionConfigList["actions"] = HashCompare<QString, uint8_t>::BitCompare;
	actionConfigList["actions-equal"] = HashCompare<QString, uint8_t>::EqualCompare;
	actionConfigList["actions-notequal"] = HashCompare<QString, uint8_t>::NotEqualCompare;
	QMapIterator<QString, uint8_t> map(actionConfigList);

	while (map.hasNext()) {
		map.next();
		settings.beginGroup(map.key());
		foreach (const QString &string, settings.allKeys()) {
			QStringList params = settings.value(string, QString()).toString().split(QChar(' '));
			int32_t index;

			while ((index = params.indexOf("")) != -1)
				params.removeAt(index);

			if (params.count() < 1)
				continue;

			CommandAction *action = new CommandAction;
			action->event = extractDeviceEvent(string);
			action->params = params;
			action->alghoritm = actionConfigList[map.key()];
			action->actived = false;
			commandActions << action;
		}
		settings.endGroup();
	}
}

void UInputProfileManager::unloadCommandEvents() {
	foreach (CommandAction *action, commandActions)
		delete action;
	commandActions.clear();
}

void UInputProfileManager::initializeCommandEvents() {
	commandIds["exec"] = executeAction;
	commandIds["execute"] = executeAction;
	commandIds["rumble"] = rumbleAction;
	commandIds["hwheel"] = hwheelAction;
	commandIds["vwheel"] = vwheelAction;
}

void UInputProfileManager::processCommandEvents() {
	if (commandActions.isEmpty())
		return;

	HashCompare<uint32_t, uint64_t> compare;
	foreach (CommandAction *action, commandActions) {
		if (action->event.isEmpty())
			continue;

		bool matched = compare.compare(&action->event, &lastWiiremoteButtons, action->alghoritm);

		if (matched && !action->actived) {
			action->actived = !action->actived;
			activeCommandEvent(action->params);
		} else if (!matched && action->actived) {
			action->actived = !action->actived;
			deactiveCommandEvent(action->params);
		}
	}
}

void UInputProfileManager::activeCommandEvent(QStringList &params) {
	switch (commandIds.value(params.at(0))) {
		case rumbleAction:
			dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(QString(params.value(1, "1")).toUInt(), true);
			break;

		case executeAction:
			emit executeRequest(params);
			break;

		case hwheelAction:
			virtualEvent->moveMouseHWheel(QString(params.value(1, "0")).toInt());
			break;

		case vwheelAction:
			virtualEvent->moveMouseVWheel(QString(params.value(1, "0")).toInt());
			break;
	}
}

void UInputProfileManager::deactiveCommandEvent(QStringList &params) {
	switch (commandIds.value(params.at(0))) {
		case rumbleAction:
			dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(QString(params.value(1, "1")).toUInt(), false);
	}
}
