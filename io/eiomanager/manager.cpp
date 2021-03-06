#include <QFile>
#include <QSettings>

#include "eioinfrared/eioinfraredmouse.h"
#include "eiomanager/manager.h"
#include "io/functionals/hash-compare.h"

using namespace io::interface;
using namespace io::functional;

extern QMap<QString, u64> devicebuttons;
extern QMap<QString, uint> scancodes;

const QRegExp deviceEventRegExp(".*(\\[.*(\\d+)\\])");

UInputProfileManager::UInputProfileManager(QObject *parent)
		: QObject(parent)
		//		, dbusDeviceEventsIface(std::make_unique<WiimotedevDeviceEvents>(
		//			  WIIMOTEDEV_DBUS_SERVICE_NAME,
		//			  WIIMOTEDEV_DBUS_OBJECT_EVENTS,
		//			  QDBusConnection::systemBus(), this))
		, m_eventDevice("Virtual mouse and keyboard", 0) {
	//	connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusWiimoteGeneralButtons, this, &UInputProfileManager::dbusWiimoteGeneralButtons, Qt::DirectConnection);
	initializeCommandEvents();
	dbusWiimoteGeneralButtons(1, 0);
	m_eventDevice.open();
	m_eventDevice.configure();
	m_eventDevice.create();
}

QHash<u32, u64> UInputProfileManager::extractDeviceEvent(QString input) {
	QStringList list = input.remove(QRegExp("[ ]")).toLower().split('+');
	QHash<u32, u64> values;
	bool result = false;
	u32 index;
	u64 value;
	for (const auto &item : list) {
		deviceEventRegExp.exactMatch(item);
		index = deviceEventRegExp.cap(2).toUInt();
		value = devicebuttons.value(deviceEventRegExp.cap(0).remove(deviceEventRegExp.cap(1)), 0);
		values.insert(index, values.value(index, 0) | value);
		result = result || (value);
	}

	if (!result)
		return (QHash<u32, u64>());
	else
		return values;
}

QList<uint> UInputProfileManager::extractScancodes(QStringList list) {
	QList<uint> values;

	for (int i = 0; i < list.count(); ++i)
		if (scancodes.value(list.at(i), QString(list.at(i)).toUInt()))
			values << scancodes.value(list.at(i), QString(list.at(i)).toUInt());

	return values;
}

void UInputProfileManager::dbusWiimoteGeneralButtons(u32 id, u64 buttons) {
	if (lastWiiremoteButtons.value(id, -1) == buttons)
		return;

	lastWiiremoteButtons[id] = buttons;
	processCommandEvents();
}

bool UInputProfileManager::loadProfile(QString file) {
	if (!QFile::exists(file))
		return false;

	m_profile = std::make_unique<decltype(m_profile)::element_type>(file.toStdString());
	//connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusWiimoteButtons, m_profile.get(), &Profile::wiimoteButtons, Qt::DirectConnection);
	//connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusNunchukButtons, m_profile.get(), &Profile::nunchukButtons, Qt::DirectConnection);
	//connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusNunchukStick, m_profile.get(), &Profile::nunchukStick, Qt::DirectConnection);
	//connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusClassicControllerButtons, m_profile.get(), &Profile::classicControllerButtons, Qt::DirectConnection);
	//connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusClassicControllerLStick, m_profile.get(), &Profile::classicControllerLStick, Qt::DirectConnection);
	//connect(dbusDeviceEventsIface.get(), &WiimotedevDeviceEvents::dbusClassicControllerRStick, m_profile.get(), &Profile::classicControllerRStick, Qt::DirectConnection);

	QSettings settings(file, QSettings::IniFormat);
	for (auto &&key : settings.childGroups()) {
		const QString &module = settings.value(key + "/module", QString()).toString();

		if (module.isEmpty())
			continue;

		if (module.toLower() == QString::fromUtf8("keyboard"))
			assignKeyboardEvents(key, settings);
	}
	loadCommandEvents(settings);
	loadInfraredEvents(settings);
	return true;
}

bool UInputProfileManager::unloadProfile() {
	unloadCommandEvents();
	m_mouses.clear();
	m_keyboards.clear();
	return true;
}

UInputProfileManager::~UInputProfileManager() {
	unloadProfile();
}
