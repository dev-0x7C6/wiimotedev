#include <io/functionals/hash-compare.h>
#include "eiomanager/manager.h"

extern QMap<QString, uint> scancodes;

void UInputProfileManager::assignKeyboardEvents(const QString &key, QSettings &settings) {
	freeKeyboardEvents();
	settings.beginGroup(key);

	auto device = std::make_unique<EIORemoteKeyboard>(m_eventDevice);

	foreach (const QString &string, settings.allKeys()) {
		if (string.toLower() == "module")
			continue;

		KeyboardAction action;
		action.event = extractDeviceEvent(string);

		if (action.event.isEmpty())
			continue;

		action.keys = extractScancodes(settings.value(string, QStringList()).toStringList());

		if (action.keys.isEmpty())
			continue;

		action.pushed = false;
		device->addKeyboardAction(action);
	}
	// FIXME: removed dbusDeviceEventsIface
	// connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusWiimoteGeneralButtons, device.get(), &EIORemoteKeyboard::dbusWiimoteGeneralButtons);
	settings.endGroup();

	m_keyboards.emplace_back(std::move(device));
}

void UInputProfileManager::freeKeyboardEvents() {
	m_keyboards.clear();
}
