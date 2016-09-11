#pragma once

#include <QObject>
#include <QSettings>
#include <QTimer>

#include <memory>
#include <list>

#include "dbus/customjobs.h"
#include "dbus/profilemanager.h"
#include "dbus/uinputservice.h"
#include "eioinfrared/eioinfraredmouse.h"
#include "eiokeyboard/eioremotekeyboard.h"
#include "emulation/event-device.h"
#include "interfaces/igamepad.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "linux/usr/include/wiimotedev/deviceevents.h"

class UInputProfileManager : public QObject {
	Q_OBJECT
public:
	explicit UInputProfileManager(QObject *parent = nullptr);
	virtual ~UInputProfileManager();

private:
	QHash<uint32_t, uint64_t> extractDeviceEvent(QString);
	QList<uint> extractScancodes(QStringList);

	QMap<QString, uint> commandIds;

	void initializeCommandEvents();

	void freeKeyboardEvents();

	bool setup(const io::emulation::gamepad::IGamepad::Type type, const std::string &name, uint32_t id);

	void assignKeyboardEvents(const QString &key, QSettings &settings);
	void assignJoystickEvents(const QString &key, QSettings &settings);

	void loadCommandEvents(QSettings &);
	void unloadCommandEvents();

	void loadInfraredEvents(QSettings &);
	void unloadInfraredEvents();

	void processCommandEvents();

	void activeCommandEvent(QStringList &);
	void deactiveCommandEvent(QStringList &);

private:
	void gamepad_iterator(const io::emulation::gamepad::IGamepad::Type type, const uint32_t id, std::function<void(const std::unique_ptr<io::emulation::gamepad::IGamepad> &)> &&function);

private slots:
	void dbusClassicControllerButtons(uint32_t, uint64_t);
	void dbusClassicControllerLStick(uint32_t, stickdata);
	void dbusClassicControllerRStick(uint32_t, stickdata);
	void dbusNunchukButtons(uint32_t, uint64_t);
	void dbusNunchukStick(uint32_t, stickdata);
	void dbusWiimoteButtons(uint32_t, uint64_t);
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

public slots:
	bool loadProfile(QString);
	bool unloadProfile();

private:
	struct CommandAction {
		QHash<uint32_t, uint64_t> event;
		QStringList params;
		bool actived;
		uint8_t alghoritm;
	};

	enum CommandList {
		executeAction = 1,
		rumbleAction,
		hwheelAction,
		vwheelAction
	};

	std::unique_ptr<WiimotedevDeviceEvents> dbusDeviceEventsIface;
	std::unique_ptr<DBusProfileManagerAdaptorWrapper> m_dbusProfileManager;
	std::unique_ptr<DBusServiceAdaptorWrapper> m_dbusService;
	std::unique_ptr<DBusCustomJobsAdaptorWrapper> m_dbusCustomJobs;

	std::list<std::unique_ptr<io::emulation::gamepad::IGamepad>> m_gamepads;
	std::list<std::unique_ptr<EIORemoteKeyboard>> m_keyboards;
	std::list<std::unique_ptr<EIOInfraredMouse>> m_mouses;
	std::list<std::unique_ptr<CommandAction>> commandActions;
	QHash<uint32_t, uint64_t> lastWiiremoteButtons;
	EventDevice m_eventDevice;

signals:
	void executeRequest(QStringList);
};
