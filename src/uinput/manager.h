/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QTimer>

#include <QScopedPointer>

#include "adaptors/customjobs.h"
#include "adaptors/profilemanager.h"
#include "adaptors/uinputservice.h"
#include "adaptors/adaptors.h"
#include "headers/consts.h"
#include "headers/interface.h"
#include "devices/keyboard.h"





/*
class VirtualDeviceManager :public QObject
{
    Q_OBJECT
private:
// Standard devices -----------------------------------------------
    UInputKeyboard *keyboard;
    UInputMouse *relativeMouse;
    UInputMouse *absoluteMouse;

// Gamepads devices -----------------------------------------------
    QMap < quint32, *UInputClassicGamepad> virtualClassicGamepads;
    QMap < quint32, *UInputWiimoteGamepad> virtualWiimoteGamepads;

public:
    VirtualDeviceManager(QObject *parent = 0) :QObject(parent){
        keyboard = new UInputKeyboard;
        relativeMouse = new UInputMouse();
        absoluteMouse = new UInputMouse();

        keyboard->uinput_open();
        relativeMouse->uinput_open();
        absoluteMouse->uinput_open(QRect(-512, -384, 1024, 768), true);
    };

    ~VirtualDeviceManager(){
        disconnect(this, 0, 0, 0);

        keyboard->uinput_close();
        relativeMouse->uinput_close();
        absoluteMouse->uinput_close();

        QMapIterator < quint32, *UInputClassicGamepad> classicIterator(virtualClassicGamepads);
        QMapIterator < quint32, *UInputWiimoteGamepad> wiimoteIterator(virtualWiimoteGamepads);

        while (classicIterator.hasNext()){
            static_cast< UInputClassicGamepad*>( classicIterator.value())->uinput_close();
            delete classicIterator.value();
        }

        while (wiimoteIterator.hasNext()){
            static_cast< UInputWiimoteGamepad*>( wiimoteIterator.value())->uinput_close();
            delete wiimoteIterator.value();
        }

        delete keyboard;
        delete relativeMouse;
        delete absoluteMouse;
    }

    void initVirtualClassicGamepad(QList < quint32> &devices);
    void freeVirtualClassicGamepad(QList < quint32> &devices);
    void freeAllVirtualClassicGamepad();

    void initVirtualWiimoteGamepad(QList < quint32> &devices);
    void freeVirtualWiimoteGamepad(QList < quint32> &devices);
    void freeAllVirtualWiimoteGamepad();

};
*/

class itemTemplate
{
private:
    bool actived;

public:
    inline void setActivedValue(bool status) { actived = status; };
    inline bool getActivedValue() { return actived; };
};

class eventItem: public itemTemplate
{  
public:
    QMap < qint32, quint64> event;
    QList < quint16> scancodes;
};

class execItem: public itemTemplate
{
public:
    QMap < qint32, quint64> event;
    QString exec;
};

class actionItem: public itemTemplate
{
public:
    QMap < qint32, quint64> event;
    QString action;
};



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
  bool enableWiiremoteInfraredMouse;

//Keyboard section
  const static char *keyboardSection;

  struct KeyboardAction {
    QHash< quint32, quint64> event;
    QList< uint16> keys;
    bool pushed;
  };

  QList < KeyboardAction*> keyboardActions;

  QHash< quint32, quint64> lastWiiremoteButtons;
  UInputEvent *virtualEvent;


public:
  UInputProfileManager(QObject *parent = 0);

private:
  QHash < quint32, quint64> extractDeviceEvent(QString);
  QList < quint16> extractScancodes(QStringList);

  void processKeyboardEvents();

private Q_SLOTS:
  void dbusWiimoteGeneralButtons(quint32, quint64);
  void dbusWiimoteInfrared(quint32, QList< irpoint>);

public Q_SLOTS:
  inline bool isWiimotedevServiceAvailable(){ return dbusDeviceEventsIface->isValid(); }

  bool loadProfile(QString);
  bool unloadProfile();


};


class ProfileManager : public QObject
{
    Q_OBJECT
public:
    ProfileManager(QObject *parent = 0);
   ~ProfileManager();

private:          

/* Infrared section ---------------------------------------------- */

    qint32 x;
    qint32 y;

    quint32 irWiimoteId;
    quint8 irMode;
    quint8 irAlghoritm;
    double irXSensitivity;
    double irYSensitivity;
    quint32 irXFreeZone;
    quint32 irYFreeZone;
    quint32 irTimeout;
    quint32 irLatency;

    qint32 moveX;
    qint32 moveY;
    QTimer infraredTimer;
    QTimer infraredTimeout;
    QRect irRange;

    bool timeout;

/* DBus section -------------------------------------------------- */

    DBusProfileManagerAdaptorWrapper *dbusProfileManager;
    DBusServiceAdaptorWrapper *dbusService;
    DBusCustomJobsAdaptorWrapper *dbusCustomJobs;

    DBusDeviceEventsInterface *dbusDeviceEventsIface;

/* Uinput section ------------------------------------------------ */

    UInputClassicGamepad *virtualClassicGamepad;
    UInputWiimoteGamepad *virtualWiimoteGamepad;

    UInputEvent *virtualEvent;
    UInputMouse *virtualAbsoluteMouse;

/* Execute section ----------------------------------------------- */

    QList < eventItem*> events;
    QList < eventItem*> eventsOnce;
    QList < execItem*> execs;
    QList < execItem*> execsReleased;
    QList < actionItem*> actions;
    QList < actionItem*> actionsRelease;

/* General variables --------------------------------------------- */

    QString profileName;
    QMap < quint32, quint64> buttons;

    qint32 lastX;
    qint32 lastY;
    QPoint cursor;

private:
    QMap < qint32, quint64> extractDeviceEvent(QString str);
    QList < quint16> extractScancodes(QStringList list);

public slots:
    inline bool isWiimotedevServiceAvailable(){ return dbusDeviceEventsIface->isValid(); }
    inline QString currentProfile(){ return profileName; }

    bool loadProfile(QString file);
    bool unloadProfile();

private slots:
    inline void slotDBusClassicControllerButtons(quint32 id, quint64 buttons) {
        virtualClassicGamepad->classicButtons(id, buttons);
    }

    inline void slotDBusWiimoteAcc(quint32 id, struct accdata acc) {
        virtualWiimoteGamepad->wiimoteAcc(id, acc);
    }

    inline void slotDBusWiimoteButtons(quint32 id, quint64 buttons) {
        virtualWiimoteGamepad->wiimoteButtons(id, buttons);
    }

    inline void slotDBusNunchukAcc(quint32 id, struct accdata acc) {
        virtualWiimoteGamepad->nunchukAcc(id, acc);
    }

    inline void slotDBusNunchukButtons(quint32 id, quint64 buttons) {
        virtualWiimoteGamepad->nunchukButtons(id, buttons);
    }

    inline void slotDBusNunchukStick(quint32 id, struct stickdata stick) {
        virtualWiimoteGamepad->nunchukStick(id, stick.x, 0xFF - stick.y);
    }

    inline void slotDBusClassicControllerLStick(quint32 id, struct stickdata stick) {
        virtualClassicGamepad->classicLStick(id, stick.x, 0x3F - stick.y);
    }

    inline void slotDBusClassicControllerRStick(quint32 id, struct stickdata stick) {
        virtualClassicGamepad->classicRStick(id, stick.x, 0x1F - stick.y);
    }


    bool dbusWiimoteGetRumbleStatus(quint32 id){};
    bool dbusWiimoteSetLedStatus(quint32 id, quint8 status){};
    bool dbusWiimoteSetRumbleStatus(quint32 id, bool status){};
    quint8 dbusWiimoteGetLedStatus(quint32 id){};

    void dbusWiimoteGeneralButtons(quint32 id, quint64 value);
    void dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points);

    void infraredTimeoutSection(){ timeout = true; };
    void infraredAccSection();
};

#endif // PROFILEMANAGER_H
