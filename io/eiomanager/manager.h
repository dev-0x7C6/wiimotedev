#pragma once

#include <QObject>
#include <QSettings>
#include <QTimer>

#include <memory>
#include <list>

#include "eioinfrared/eioinfraredmouse.h"
#include "eiokeyboard/eioremotekeyboard.h"
#include "emulation/event-device.h"
#include "functionals/profile.h"
#include "interfaces/igamepad.h"
#include "include/wiimotedev/wiimotedev"

class UInputProfileManager : public QObject {
	Q_OBJECT
public:
	explicit UInputProfileManager(QObject *parent = nullptr);
	virtual ~UInputProfileManager();

private:
	QHash<u32, u64> extractDeviceEvent(QString);
	QList<uint> extractScancodes(QStringList);

	QMap<QString, uint> commandIds;

	void initializeCommandEvents();

	void freeKeyboardEvents();

	void assignKeyboardEvents(const QString &key, QSettings &settings);

	void loadCommandEvents(QSettings &);
	void unloadCommandEvents();

	void loadInfraredEvents(QSettings &);
	void unloadInfraredEvents();

	void processCommandEvents();

	void activeCommandEvent(QStringList &);
	void deactiveCommandEvent(QStringList &);

private slots:
	void dbusWiimoteGeneralButtons(u32, u64);

public slots:
	bool loadProfile(QString);
	bool unloadProfile();

private:
	struct CommandAction {
		QHash<u32, u64> event;
		QStringList params;
		bool actived;
		u8 alghoritm;
	};

	enum CommandList {
		executeAction = 1,
		rumbleAction,
		hwheelAction,
		vwheelAction
	};

	std::unique_ptr<io::functional::Profile> m_profile;

	std::list<std::unique_ptr<EIORemoteKeyboard>> m_keyboards;
	std::list<std::unique_ptr<io::EIOInfraredMouse>> m_mouses;
	std::list<std::unique_ptr<CommandAction>> commandActions;
	QHash<u32, u64> lastWiiremoteButtons;
	io::emulation::EventDevice m_eventDevice;

signals:
	void executeRequest(QStringList);
};
