/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2010  Bartłomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
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

#include <QScopedPointer>
#include <QSettings>

#include "adaptors/customjobs.h"
#include "adaptors/profilemanager.h"
#include "adaptors/uinputservice.h"
#include "adaptors/adaptors.h"
#include "devices/classicgamepad.h"
#include "devices/eventdevice.h"
#include "devices/mouse.h"
#include "devices/touchscreen.h"
#include "devices/wiimotegamepad.h"
#include "headers/consts.h"
#include "interfaces/deviceevents.h"
#include "virtual/event/keyboard.h"
#include "virtual/mouse/infrared.h"

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
  quint8 mode;
  quint8 alghoritm;
};


struct InfraredConfigurationStruct {
  quint8 mode;
  quint8 alghoritm;
};

namespace profiles
{
    namespace infrared
    {
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


class UInputProfileManager :public QObject
{
  Q_OBJECT
private:
//Interfaces
  DBusDeviceEventsInterface *dbusDeviceEventsIface;

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

  QList < EventVirtualKeyboard*> virtualKeyboards;
  QList < InfraredVirtualMouse*> virtualMouses;

//Keyboard section
  const static char *keyboardSection;

  struct CommandAction {
    QHash< quint32, quint64> event;
    QStringList params;
    bool actived;
    quint8 alghoritm;
  };

  enum CommandList {
    executeAction = 1,
    rumbleAction,
    hwheelAction,
    vwheelAction
  };

  QList < CommandAction*> commandActions;
  QHash< quint32, quint64> lastWiiremoteButtons;


  QHash < quint32, ClassicGamepadDevice*> classicGamepads;
  QHash < quint32, WiimoteGamepadDevice*> wiimoteGamepads;



  UInputEvent *virtualEvent;

/* General variables --------------------------------------------- */

  QString profileName;
  QPoint cursor;

public:
  UInputProfileManager(QObject *parent = 0);
 ~UInputProfileManager();

private:
  QHash < quint32, quint64> extractDeviceEvent(QString);
  QList < quint32> extractScancodes(QStringList);

  QMap < QString, quint32> commandIds;

  void initializeCommandEvents();

  void loadGamepadEvents(QSettings&);
  void unloadGamepadEvents();

  void loadKeyboardEvents(QSettings&);
  void unloadKeyboardEvents();

  void loadCommandEvents(QSettings&);
  void unloadCommandEvents();

  void loadInfraredEvents(QSettings&);
  void unloadInfraredEvents();

  void processCommandEvents();

  void activeCommandEvent(QStringList&);
  void deactiveCommandEvent(QStringList&);

private Q_SLOTS:
  void dbusWiimoteGeneralButtons(quint32, quint64);

  void dbusClassicControllerButtons(quint32, quint64);
  void dbusWiimoteAcc(quint32, accdata);
  void dbusWiimoteButtons(quint32, quint64);
  void dbusNunchukAcc(quint32, accdata);
  void dbusNunchukButtons(quint32, quint64);
  void dbusNunchukStick(quint32, stickdata);
  void dbusClassicControllerLStick(quint32, stickdata);
  void dbusClassicControllerRStick(quint32, stickdata);

public Q_SLOTS:
  inline bool isWiimotedevServiceAvailable(){ return dbusDeviceEventsIface->isValid(); }

  bool loadProfile(QString);
  bool unloadProfile();

Q_SIGNALS:
  void executeRequest(QStringList);

};

#endif // PROFILEMANAGER_H
