#include "eiomanager/manager.h"
#include <io/functionals/hash-compare.h>

const QRegExp deviceEventRegExp(".*(\\(.*(\\d+)\\))");

void UInputProfileManager::loadCommandEvents(QSettings &settings) {
	unloadCommandEvents();
	QMap<QString, u8> actionConfigList;
	actionConfigList["actions-bit"] = HashCompare<QString, u8>::BitCompare;
	actionConfigList["actions"] = HashCompare<QString, u8>::BitCompare;
	actionConfigList["actions-equal"] = HashCompare<QString, u8>::EqualCompare;
	actionConfigList["actions-notequal"] = HashCompare<QString, u8>::NotEqualCompare;
	QMapIterator<QString, u8> map(actionConfigList);

	while (map.hasNext()) {
		map.next();
		settings.beginGroup(map.key());
		for (const auto &string : settings.allKeys()) {
			QStringList params = settings.value(string, QString()).toString().split(QChar(' '));
			i32 index;

			while ((index = params.indexOf("")) != -1)
				params.removeAt(index);

			if (params.count() < 1)
				continue;

			auto action = std::make_unique<CommandAction>();
			action->event = extractDeviceEvent(string);
			action->params = params;
			action->alghoritm = actionConfigList[map.key()];
			action->actived = false;
			commandActions.emplace_back(std::move(action));
		}
		settings.endGroup();
	}
}

void UInputProfileManager::unloadCommandEvents() {
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
	if (commandActions.empty())
		return;

	HashCompare<u32, u64> compare;
	for (const auto &action : commandActions) {
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
			//			dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(QString(params.value(1, "1")).toUInt(), true);
			break;

		case executeAction:
			emit executeRequest(params);
			break;

		case hwheelAction:
			m_eventDevice.moveMouseHWheel(QString(params.value(1, "0")).toInt());
			break;

		case vwheelAction:
			m_eventDevice.moveMouseVWheel(QString(params.value(1, "0")).toInt());
			break;
	}
}

void UInputProfileManager::deactiveCommandEvent(QStringList &params) {
	//	switch (commandIds.value(params.at(0))) {
	//		case rumbleAction:
	////			dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(QString(params.value(1, "1")).toUInt(), false);
	//	}
}
