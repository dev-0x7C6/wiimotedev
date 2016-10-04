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
#include "linux/usr/include/wiimotedev/wiimotedev"

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

	void assignKeyboardEvents(const QString &key, QSettings &settings);

	void loadCommandEvents(QSettings &);
	void unloadCommandEvents();

	void loadInfraredEvents(QSettings &);
	void unloadInfraredEvents();

	void processCommandEvents();

	void activeCommandEvent(QStringList &);
	void deactiveCommandEvent(QStringList &);

private slots:
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

	std::unique_ptr<io::functional::Profile> m_profile;

	std::list<std::unique_ptr<EIORemoteKeyboard>> m_keyboards;
	std::list<std::unique_ptr<EIOInfraredMouse>> m_mouses;
	std::list<std::unique_ptr<CommandAction>> commandActions;
	QHash<uint32_t, uint64_t> lastWiiremoteButtons;
	io::emulation::EventDevice m_eventDevice;

signals:
	void executeRequest(QStringList);
};
