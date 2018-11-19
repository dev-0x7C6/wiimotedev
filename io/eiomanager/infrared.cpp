#include "eiomanager/manager.h"

#include <iostream>

void UInputProfileManager::loadInfraredEvents(QSettings &settings) {
	unloadInfraredEvents();
	for (const auto &key : settings.childGroups()) {
		if (settings.value(QString("%1/module").arg(key), QString()).toString().toLower() == "infrared" ||
			settings.value(QString("%1/module").arg(key), QString()).toString().toLower() == "mouse") {
			settings.beginGroup(key);
			auto device = std::make_unique<io::EIOInfraredMouse>(m_eventDevice);
			device->setId(settings.value("device", uint(1)).toLongLong());
			std::cout << settings.value("acc", false).toBool() << std::endl;
			device->config().setAccEnabled(settings.value("acc", false).toBool());
			device->config().setAccMultiX(settings.value("acc.multi.x", 4).toDouble());
			device->config().setAccMultiY(settings.value("acc.multi.y", 4).toDouble());
			device->config().setAccPowX(settings.value("acc.pow.x").toDouble());
			device->config().setAccPowY(settings.value("acc.pow.y").toDouble());
			device->config().setAccTimeout(settings.value("acc.timeout", 2000).toLongLong());
			device->config().setDeadzoneX(settings.value("deadzone.x", 30).toULongLong());
			device->config().setDeadzoneY(settings.value("deadzone.y", 20).toULongLong());
			//			connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusVirtualCursorPosition, device.get(), &EIOInfraredMouse::dbusVirtualCursorPosition);
			//			connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusVirtualCursorLost, device.get(), &EIOInfraredMouse::dbusVirtualCursorLost);
			settings.endGroup();
			device->start();
			m_mouses.emplace_back(std::move(device));
		}
	}
}

void UInputProfileManager::unloadInfraredEvents() {
	m_mouses.clear();
}
