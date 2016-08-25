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

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QPoint>

#include <QScopedPointer>
#include <QSettings>

#include "dbus/customjobs.h"
#include "dbus/profilemanager.h"
#include "dbus/uinputservice.h"
#include "eiobase/eioeventdevice.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "linux/usr/include/wiimotedev/deviceevents.h"
#include "eiokeyboard/eioremotekeyboard.h"
#include "eioinfrared/eioinfraredmouse.h"

#include "eiojoystick/eioclassicjoystick.h"
#include "eiojoystick/eionunchukjoystick.h"
#include "eiojoystick/eiowiimotejoystick.h"

enum {
	mouseEmulationModeNone = 0,
	mouseEmulationModeAbs,
	mouseEmulationModeAcc
};

enum {
	mouseEmulationAlghoritm1point = 1,
	mouseEmulationAlghoritm2points,
	mouseEmulationAlghoritm3points,
	mouseEmulationAlghoritm4points
};

struct mouseEmulationStruct {
	uint8_t mode;
	uint8_t alghoritm;
};

struct InfraredConfigurationStruct {
	uint8_t mode;
	uint8_t alghoritm;
};

namespace profiles {
namespace infrared {
const QString section("infrared/");
const QString wiimoteid("wiimoteid");
const QString mode("mode");
const QString alghoritm("alghoritm");
const QString sensx("sensitivity.x");
const QString sensy("sensitivity.y");
const QString freezonex("freezone.x");
const QString freezoney("freezone.y");
const QString timeout("timeout");
const QString latency("latency");
const QString range("range");
}
}

enum KeyboardActionAlghortim {
	keyboardBitCompare,
	keyboardEqual,
	keyboardNotEqual
};

class UInputProfileManager : public QObject {
	Q_OBJECT
private:
	//Interfaces
	WiimotedevDeviceEvents *dbusDeviceEventsIface;

	//Adaptors
	DBusProfileManagerAdaptorWrapper *dbusProfileManager;
	DBusServiceAdaptorWrapper *dbusService;
	DBusCustomJobsAdaptorWrapper *dbusCustomJobs;

	//Profile section
	QString author;
	QString email;
	QString name;
	QString path;
	QString version;

	//Settings

	bool disableNunchukExtShift;
	bool disableNunchukExtShake;
	bool disableNunchukExtTilt;
	bool disableWiiremoteShift;
	bool disableWiiremoteShake;
	bool disableWiiremoteTilt;
	bool disableKeyboardModule;
	bool enableWiiremoteInfraredMouse;
	bool rumbleStatus;

	//Keyboard section
	const static char *keyboardSection;

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

	QList<CommandAction *> commandActions;
	QHash<uint32_t, uint64_t> lastWiiremoteButtons;

	QList<EIOClassicJoystick *> EIOClassicJoysticks;
	QList<EIONunchukJoystick *> EIONunchukJoysticks;
	QList<EIOWiimoteJoystick *> EIOWiimoteJoysticks;
	QList<EIORemoteKeyboard *> EIORemoteKeyboards;
	QList<EIOInfraredMouse *> EIOInfraredMouses;

	EventDevice *virtualEvent;

	/* General variables --------------------------------------------- */

	QString profileName;
	QPoint cursor;

public:
	UInputProfileManager(QObject *parent = 0);
	~UInputProfileManager();

private:
	QHash<uint32_t, uint64_t> extractDeviceEvent(QString);
	QList<uint> extractScancodes(QStringList);

	QMap<QString, uint> commandIds;

	void initializeCommandEvents();

	void freeKeyboardEvents();

	void setupClassicJoystick(uint32_t assign, const QString &name, QSettings &settings);
	void setupWiimoteJoystick(uint32_t assign, const QString &name, QSettings &settings);
	void setupNunchukJoystick(uint32_t assign, const QString &name, QSettings &settings);
	void setupInfraredMouse(uint32_t assing, const QString &name, QSettings &settings);

	void assignInfraredEvents(const QString &key, QSettings &settings);
	void assignKeyboardEvents(const QString &key, QSettings &settings);
	void assignJoystickEvents(const QString &key, QSettings &settings);

	void freeJoystickEvents();

	//void assignCommandEvents(const QString &key, QSettings &settings);

	void loadCommandEvents(QSettings &);
	void unloadCommandEvents();

	void loadInfraredEvents(QSettings &);
	void unloadInfraredEvents();

	void processCommandEvents();

	void activeCommandEvent(QStringList &);
	void deactiveCommandEvent(QStringList &);

private Q_SLOTS:
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

	void dbusClassicControllerButtons(uint32_t, uint64_t);
	void dbusWiimoteAcc(uint32_t, accdata);
	void dbusWiimoteButtons(uint32_t, uint64_t);
	void dbusNunchukAcc(uint32_t, accdata);
	void dbusNunchukButtons(uint32_t, uint64_t);
	void dbusNunchukStick(uint32_t, stickdata);
	void dbusClassicControllerLStick(uint32_t, stickdata);
	void dbusClassicControllerRStick(uint32_t, stickdata);

public Q_SLOTS:
	inline bool isWiimotedevServiceAvailable() {
		return dbusDeviceEventsIface->isValid();
	}

	bool loadProfile(QString);
	bool unloadProfile();

Q_SIGNALS:
	void executeRequest(QStringList);
};

#endif // PROFILEMANAGER_H
