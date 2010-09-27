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

#include <QSettings>
#include <QDebug>

#include "uinput/manager.h"

extern QMap < QString, quint64> devicebuttons;
extern QMap < QString, quint16> scancodes;

const QRegExp deviceEventRegExp(".*(\\[.*(\\d+)\\])");

QHash < quint32, quint64> UInputProfileManager::extractDeviceEvent(QString input)
{
  QStringList list = input.remove(QRegExp("[ ]")).toLower().split('+');
  QHash < quint32, quint64> values;
  bool result = false;

  quint32 index;
  quint64 value;

  foreach (const QString &item, list) {
    deviceEventRegExp.exactMatch(item);
    index = deviceEventRegExp.cap(2).toUInt();
    value = devicebuttons.value(deviceEventRegExp.cap(0).remove(deviceEventRegExp.cap(1)), 0);
    values.insert(index, values.value(index, 0) | value);
    result = result || (value);
  }

  if (!result)
    return (QHash < quint32, quint64>()); else
    return values;
}

QList < quint16> UInputProfileManager::extractScancodes(QStringList list)
{
  QList < quint16> values;
  for (register int i = 0; i < list.count(); ++i)
    values << scancodes.value(list.at(i), 0);
  return values;
}


UInputProfileManager::UInputProfileManager(QObject *parent) :QObject(parent),
  dbusDeviceEventsIface(new DBusDeviceEventsInterface(
      WIIMOTEDEV_DBUS_SERVICE_NAME,
      WIIMOTEDEV_DBUS_OBJECT_EVENTS,
      QDBusConnection::systemBus(), this)),
  dbusProfileManager(new DBusProfileManagerAdaptorWrapper(this, QDBusConnection::systemBus())),
  dbusService(new DBusServiceAdaptorWrapper(this, QDBusConnection::systemBus())),
  dbusCustomJobs(new DBusCustomJobsAdaptorWrapper(this, QDBusConnection::systemBus())),
  disableNunchukExtShift(false),
  disableNunchukExtShake(false),
  disableNunchukExtTilt(false),
  disableWiiremoteShift(false),
  disableWiiremoteShake(false),
  disableWiiremoteTilt(false),
  enableWiiremoteInfraredMouse(false),
  virtualEvent(new UInputEvent())
{
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));
  connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteInfrared(quint32,QList<irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList<irpoint>)));

  virtualEvent->uinput_open();

  this->loadProfile("/root/uinput.ini");
}

void UInputProfileManager::dbusWiimoteGeneralButtons(quint32 id, quint64 buttons) {
  if (disableNunchukExtShift) buttons &= ~NUNCHUK_SHIFT_MASK;
  if (disableNunchukExtShake) buttons &= ~NUNCHUK_BTN_SHIFT_SHAKE;
  if (disableNunchukExtTilt) buttons &= ~NUNCHUK_TILT_MASK;
  if (disableWiiremoteShift) buttons &= ~WIIMOTE_SHIFT_MASK;
  if (disableWiiremoteShake) buttons &= ~WIIMOTE_BTN_SHIFT_SHAKE;
  if (disableWiiremoteTilt) buttons &= ~WIIMOTE_TILT_MASK;

  if (lastWiiremoteButtons.value(id, 0) == buttons)
    return;

  lastWiiremoteButtons[id] = buttons;

  processKeyboardEvents();
}

void UInputProfileManager::dbusWiimoteInfrared(quint32 id, QList< irpoint> table) {
  qDebug() << table.count();
}

void UInputProfileManager::processKeyboardEvents() {
  if (keyboardActions.isEmpty())
    return;

  foreach (KeyboardAction *action, keyboardActions) {
    if (action->event.isEmpty())
      continue;

    bool matched = true;

    QHashIterator < quint32, quint64> map(action->event);
    while (map.hasNext()) {
      map.next();
      if (!(matched &= ((map.value() & lastWiiremoteButtons[map.key()]) == map.value())))
        break;
    }

    if (matched && !action->pushed) {
      action->pushed = true;
      qDebug() << "pushed";
    }

    if (!matched && action->pushed) {
      action->pushed = false;
      qDebug() << "released";
    }
  }
}

bool UInputProfileManager::loadProfile(QString file) {
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("keyboard");

  foreach (const QString &string, settings.allKeys()) {
    KeyboardAction *action = new KeyboardAction;
    action->event = extractDeviceEvent(string);
    action->keys = extractScancodes(settings.value(string, QStringList()).toStringList());
    action->pushed = false;
    keyboardActions << action;
  }

  settings.endGroup();
}

bool UInputProfileManager::unloadProfile() {

}


ProfileManager::ProfileManager(QObject *object) : QObject(object)
{
    QWIIMOTEDEV_REGISTER_META_TYPES;

    connect(&infraredTimeout, SIGNAL(timeout()), this, SLOT(infraredTimeoutSection()));
    connect(&infraredTimer, SIGNAL(timeout()), this, SLOT(infraredAccSection()));


    dbusDeviceEventsIface =
            new DBusDeviceEventsInterface(WIIMOTEDEV_DBUS_SERVICE_NAME,
                                          WIIMOTEDEV_DBUS_OBJECT_EVENTS,
                                          QDBusConnection::systemBus(), this);
    virtualClassicGamepad = new UInputClassicGamepad();
    virtualWiimoteGamepad = new UInputWiimoteGamepad();

    virtualEvent = new UInputEvent();
    virtualAbsoluteMouse = new UInputMouse();

    virtualWiimoteGamepad->uinput_open();
    virtualClassicGamepad->uinput_open();
    virtualEvent->uinput_open();

    QDBusConnection connection = QDBusConnection::systemBus();

    dbusProfileManager = new DBusProfileManagerAdaptorWrapper(this, connection);
    dbusService = new DBusServiceAdaptorWrapper(this, connection);
    dbusCustomJobs = new DBusCustomJobsAdaptorWrapper(this, connection);

    connection.registerService("org.wiimotedev.uinput");

    profileName = QString::fromUtf8("");
}

ProfileManager::~ProfileManager()
{
    unloadProfile();
    disconnect(&infraredTimeout, 0, 0, 0);
    disconnect(&infraredTimer, 0, 0, 0);

    virtualClassicGamepad->uinput_close(false);
    virtualWiimoteGamepad->uinput_close(false);
    virtualAbsoluteMouse->uinput_close(false);
    virtualEvent->uinput_close(false);

    delete virtualClassicGamepad;
    delete virtualWiimoteGamepad;
    delete virtualEvent;
    delete virtualAbsoluteMouse;

    delete dbusProfileManager;
    delete dbusService;
    delete dbusCustomJobs;
}

bool ProfileManager::loadProfile(QString file)
{
/* General section -------------------------------------------------- */
    if (!QFile::exists(file))
        return false;
    else
        unloadProfile();

    QSettings settings((profileName = file), QSettings::IniFormat);
    QMap < qint32, quint64> event;
    QList < quint16> scancodes;

/* Infrared section ------------------------------------------------ */

    settings.beginGroup(profiles::infrared::section);

    moveX = 0;
    moveY = 0;
    timeout = false;

    irWiimoteId = settings.value(profiles::infrared::wiimoteid, 0).toInt();
    irMode = settings.value(profiles::infrared::mode, 0).toInt();
    irAlghoritm = settings.value(profiles::infrared::alghoritm, 0).toInt();
    irXSensitivity = settings.value(profiles::infrared::sensx, 0).toDouble();
    irYSensitivity = settings.value(profiles::infrared::sensy, 0).toDouble();
    irXFreeZone = settings.value(profiles::infrared::freezonex, 0).toInt();
    irYFreeZone = settings.value(profiles::infrared::freezoney, 0).toInt();
    irTimeout = settings.value(profiles::infrared::timeout, 2000).toInt();
    irLatency = settings.value(profiles::infrared::latency, 8).toInt();
    irRange = settings.value(profiles::infrared::range, QRect(-512, -384, 1024, 768)).toRect();

    if (irWiimoteId) {
        infraredTimer.setInterval(irLatency);
        infraredTimer.start();
        infraredTimeout.setInterval(irTimeout);
        infraredTimeout.start();
        virtualAbsoluteMouse->uinput_open(irRange, true);
    }

    settings.endGroup();

/* Action section -------------------------------------------------- */

    QString action;

    settings.beginGroup("actions");

    for (register qint32 i = 0; i < settings.allKeys().count(); ++i) {
        event = extractDeviceEvent(settings.allKeys().at(i));
        action = settings.value(settings.allKeys().at(i), QString()).toString();

        if (event.count() > 0 && action.count() > 0) {
            actionItem *act = new actionItem();
            act->event = event;
            act->action = action;
            act->setActivedValue(false);
            actions << act;
        }
    }

    settings.endGroup();

    settings.beginGroup("actions-released");

    for (register qint32 i = 0; i < settings.allKeys().count(); ++i) {
        event = extractDeviceEvent(settings.allKeys().at(i));
        action = settings.value(settings.allKeys().at(i), QString()).toString();

        if (event.count() > 0 && action.count() > 0) {
            actionItem *act = new actionItem();
            act->event = event;
            act->action = action;
            act->setActivedValue(false);
            actionsRelease << act;
        }
    }

    settings.endGroup();


/* Events section -------------------------------------------------- */

    settings.beginGroup("events");

    for (register qint32 i = 0; i < settings.allKeys().count(); ++i) {
        event = extractDeviceEvent(settings.allKeys().at(i));
        scancodes = extractScancodes(settings.value(settings.allKeys().at(i), QStringList()).toStringList());

        if (event.count() > 0 && scancodes.count() > 0) {
            eventItem *ev = new eventItem();
            ev->event = event;
            ev->scancodes = scancodes;
            ev->setActivedValue(false);
            events << ev;
        }
    }

    settings.endGroup();

/* EventsOnce section ---------------------------------------------- */
    settings.beginGroup("events-once");

    for (register qint32 i = 0; i < settings.allKeys().count(); ++i) {
        event = extractDeviceEvent(settings.allKeys().at(i));
        scancodes = extractScancodes(settings.value(settings.allKeys().at(i), QStringList()).toStringList());

        if (event.count() > 0 && scancodes.count() > 0) {
            eventItem *ev = new eventItem();
            ev->event = event;
            ev->scancodes = scancodes;
            ev->setActivedValue(false);
            eventsOnce << ev;
        }
    }

    settings.endGroup();

/* Exec section ---------------------------------------------------- */

    settings.beginGroup("exec");

    for (register qint32 i = 0; i < settings.allKeys().count(); ++i) {
        event =  extractDeviceEvent(settings.allKeys().at(i));

        if (event.count() > 0){
            execItem *ex = new execItem();
            ex->event = event;
            ex->exec = settings.value(settings.allKeys().at(i), QString()).toString();
            ex->setActivedValue(false);
            execs << ex;
        }
    }

    settings.endGroup();

/* Exec-released section ------------------------------------------- */

    settings.beginGroup("exec-released");

    for (register qint32 i = 0; i < settings.allKeys().count(); ++i)
    {
        event = extractDeviceEvent(settings.allKeys().at(i));

        if (event.count() > 0){
            execItem *ex = new execItem();
            ex->event = event;
            ex->exec = settings.value(settings.allKeys().at(i), QString()).toString();
            ex->setActivedValue(false);
            execsReleased << ex;
        }
    }

    settings.endGroup();

/* Connect section ------------------------------------------------- */    
    connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));
    connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteInfrared(quint32,QList<irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList<irpoint>)));

    connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SLOT(slotDBusWiimoteButtons(quint32,quint64)));
    connect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteAcc(quint32,accdata)), this, SLOT(slotDBusWiimoteAcc(quint32,accdata)));

    connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SLOT(slotDBusNunchukButtons(quint32,quint64)));
    connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukStick(quint32,stickdata)), this, SLOT(slotDBusNunchukStick(quint32,stickdata)));
    connect(dbusDeviceEventsIface, SIGNAL(dbusNunchukAcc(quint32,accdata)), this, SLOT(slotDBusNunchukAcc(quint32,accdata)));

    connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SLOT(slotDBusClassicControllerButtons(quint32,quint64)));
    connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerLStick(quint32,stickdata)), this, SLOT(slotDBusClassicControllerLStick(quint32,stickdata)));
    connect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerRStick(quint32,stickdata)), this, SLOT(slotDBusClassicControllerRStick(quint32,stickdata)));

    return true;
}

bool ProfileManager::unloadProfile()
{

    virtualAbsoluteMouse->uinput_close(false);

/* Disconnect section ---------------------------------------------- */
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(dbusWiimoteGeneralButtons(quint32,quint64)));
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteInfrared(quint32,QList<irpoint>)), this, SLOT(dbusWiimoteInfrared(quint32,QList<irpoint>)));

    disconnect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteButtons(quint32,quint64)), this, SLOT(slotDBusWiimoteButtons(quint32,quint64)));
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusWiimoteAcc(quint32,accdata)), this, SLOT(slotDBusWiimoteAcc(quint32,accdata)));

    disconnect(dbusDeviceEventsIface, SIGNAL(dbusNunchukButtons(quint32,quint64)), this, SLOT(slotDBusNunchukButtons(quint32,quint64)));
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusNunchukStick(quint32,stickdata)), this, SLOT(slotDBusNunchukStick(quint32,stickdata)));
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusNunchukAcc(quint32,accdata)), this, SLOT(slotDBusNunchukAcc(quint32,accdata)));

    disconnect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerButtons(quint32,quint64)), this, SLOT(slotDBusClassicControllerButtons(quint32,quint64)));
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerLStick(quint32,stickdata)), this, SLOT(slotDBusClassicControllerLStick(quint32,stickdata)));
    disconnect(dbusDeviceEventsIface, SIGNAL(dbusClassicControllerRStick(quint32,stickdata)), this, SLOT(slotDBusClassicControllerRStick(quint32,stickdata)));

/* Release all actions Hack --------------------------------------- */

    for (register int i = 0; i < events.count(); ++i) events.at(i)->setActivedValue(true);
    for (register int i = 0; i < execs.count(); ++i) execs.at(i)->setActivedValue(true);
    for (register int i = 0; i < execsReleased.count(); ++i) execsReleased.at(i)->setActivedValue(true);

    QMapIterator < quint32, quint64> i(buttons);

    while (i.hasNext()) {
        i.next();
        dbusWiimoteGeneralButtons(i.key(), 0x00);
    }

 /* Cleanup section ------------------------------------------------- */

    buttons.clear();

    for (register int i = 0; i < events.count(); ++i) delete events.at(i);
    for (register int i = 0; i < execs.count(); ++i) delete execs.at(i);
    for (register int i = 0; i < execsReleased.count(); ++i) delete execsReleased.at(i);

    events.clear();
    eventsOnce.clear();
    execs.clear();
    execsReleased.clear();

    profileName = QString();
}

void ProfileManager::dbusWiimoteGeneralButtons(quint32 id, quint64 value)
{
#ifdef deviceButtonStatus
    quint64 x = ((quint64)1 << 63) | value;
    qDebug(QString("device[%1] flags = %2").arg(QString::number(id), QString::number(x, 2).mid(1, 63)).toAscii());
#endif

     if (buttons.value(id, 0x00) == value)
        return;

     bool matched;
     buttons[id] = value;

     for (register qint32 i = 0; i < actions.count(); ++i)
     {
         QMapIterator < qint32, quint64> map(actions.at(i)->event);
         matched = true;

         while(map.hasNext()) {
             map.next();
             matched = matched && ((buttons.value(map.key(), 0) & map.value()) == map.value());
         }

         if (actions.at(i)->getActivedValue() && !matched)
             actions.at(i)->setActivedValue(false);

         if (!actions.at(i)->getActivedValue() && matched) {
             actions.at(i)->setActivedValue(true);
             qDebug("there %s",  actions.at(i)->action.toAscii().constData());

             if (actions.at(i)->action == QString::fromUtf8("rumbleon")) dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(1, true); else
             if (actions.at(i)->action == QString::fromUtf8("rumbleoff")) dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(1, false);
         }
     }

     for (register qint32 i = 0; i < actionsRelease.count(); ++i)
     {
         QMapIterator < qint32, quint64> map(actionsRelease.at(i)->event);
         matched = true;

         while(map.hasNext()) {
             map.next();
             matched = matched && ((buttons.value(map.key(), 0) & map.value()) == map.value());
         }

         if (actionsRelease.at(i)->getActivedValue() && !matched) {
             actionsRelease.at(i)->setActivedValue(false);
             qDebug("there %s",  actionsRelease.at(i)->action.toAscii().constData());
             if (actionsRelease.at(i)->action == QString::fromUtf8("rumbleon")) dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(1, true); else
             if (actionsRelease.at(i)->action == QString::fromUtf8("rumbleoff")) dbusDeviceEventsIface->dbusWiimoteSetRumbleStatus(1, false);
         }

         if (!actionsRelease.at(i)->getActivedValue() && matched) {
             actionsRelease.at(i)->setActivedValue(true);
         }
     }


     for (register qint32 i = 0; i < events.count(); ++i)
     {
         QMapIterator < qint32, quint64> map(events.at(i)->event);
         matched = true;

         while(map.hasNext()) {
             map.next();
             matched = matched && ((buttons.value(map.key(), 0) & map.value()) == map.value());
         }

         if (events.at(i)->getActivedValue() && !matched) {
             events.at(i)->setActivedValue(false);
             for (register int l = 0; l < events.at(i)->scancodes.count(); ++l)
                 virtualEvent->releaseKeyboardButton(events.at(i)->scancodes.at(l));
         }

         if (!events.at(i)->getActivedValue() && matched) {
             events.at(i)->setActivedValue(true);
             for (register int l = 0; l < events.at(i)->scancodes.count(); ++l)
                 virtualEvent->pressKeyboardButton(events.at(i)->scancodes.at(l));
         }
     }

     for (register qint32 i = 0; i < eventsOnce.count(); ++i)
     {
         QMapIterator < qint32, quint64> map(eventsOnce.at(i)->event);
         matched = true;

         while(map.hasNext()) {
             map.next();
             matched = matched && ((buttons.value(map.key(), 0) & map.value()) == map.value());
         }
         if (eventsOnce.at(i)->getActivedValue() && !matched)
             eventsOnce.at(i)->setActivedValue(false);

         if (!eventsOnce.at(i)->getActivedValue() && matched) {
             eventsOnce.at(i)->setActivedValue(true);
             for (register int l = 0; l < eventsOnce.at(i)->scancodes.count(); ++l)
                 virtualEvent->pressKeyboardButtonOnce(eventsOnce.at(i)->scancodes.at(l));
         }
     }

     for (register qint32 i = 0; i < execs.count(); ++i)
     {
         QMapIterator < qint32, quint64> map(execs.at(i)->event);
         matched = true;

         while(map.hasNext()) {
             map.next();
             matched = matched && ((buttons.value(map.key(), 0) & map.value()) == map.value());
         }

         if (!execs.at(i)->getActivedValue() && matched) {
             execs.at(i)->setActivedValue(true);
             dbusCustomJobs->slotExecuteRequest(execs.at(i)->exec.split(QChar(' ')));
         }

         if (execs.at(i)->getActivedValue() && !matched)
             execs.at(i)->setActivedValue(false);
     }

     for (register qint32 i = 0; i < execsReleased.count(); ++i)
     {
         QMapIterator < qint32, quint64> map(execsReleased.at(i)->event);
         matched = true;

         while(map.hasNext()) {
             map.next();
             matched = matched && ((buttons.value(map.key(), 0) & map.value()) == map.value());
         }

         if (!execsReleased.at(i)->getActivedValue() && matched)
             execsReleased.at(i)->setActivedValue(true);

         if (execsReleased.at(i)->getActivedValue() && !matched) {
             dbusCustomJobs->slotExecuteRequest(execsReleased.at(i)->exec.split(QChar(' ')));
             execsReleased.at(i)->setActivedValue(false);
         }
     }
}

QMap < qint32, quint64> ProfileManager::extractDeviceEvent(QString str)
{
    QStringList list = str.remove(QRegExp("[ ]")).toLower().split('+');
    QMap < qint32, quint64> values;
    bool result = false;

    qint32 index;
    quint64 value;

    for (register int i = 0; i < list.count(); ++i)
    {
        deviceEventRegExp.exactMatch(list.at(i));
        index = deviceEventRegExp.cap(2).toInt();
        value = devicebuttons.value(deviceEventRegExp.cap(0).remove(deviceEventRegExp.cap(1)), 0);
        values.insert(index, values.value(index, 0) | value);
        result = result || (value);
    }

    if (!result)
        return (QMap < qint32, quint64>()); else
        return values;
}

QList < quint16> ProfileManager::extractScancodes(QStringList list)
{
    QList < quint16> values;
    for (register int i = 0; i < list.count(); ++i)
        values << scancodes.value(list.at(i), 0);
    return values;
}

double acc_plus(int min, int max, int value, double sensitivy)
{
    if ((value <= min) || (value >= max))
        return 0.0;
    return ((value - min) / (double)(max - min)) * sensitivy;
}


double acc_minus(int max, int min, int value, double sensitivy)
{
    if ((value <= min) || (value >= max))
        return 0.0;
    return ((value - max) / (double)(min - max)) * -sensitivy;
}

void ProfileManager::dbusWiimoteInfrared(quint32 id, QList< struct irpoint> points)
{
    if (id != irWiimoteId)
        return;

    timeout = false;

    if (irAlghoritm == mouseEmulationAlghoritm2points) {

        switch (points.count()) {
        case 4:
        case 3:
        case 2:
            x = (points.at(0).x + points.at(1).x) >> 1;
            y = (points.at(0).y + points.at(1).y) >> 1;
            break;
        case 1:
            break;

        }
    } else
        if (irAlghoritm == mouseEmulationAlghoritm1point) {
            x = points.at(0).x;
            y = points.at(0).y;
        }


    switch (irMode)
    {
    case mouseEmulationModeAbs:
        virtualAbsoluteMouse->moveMousePointerAbs(-(x - 0x0200), y - 0x0180);
        break;

    case mouseEmulationModeAcc:
        if (cursor.x() == 0) cursor.setX(x);
        if (cursor.y() == 0) cursor.setY(y);

        moveX = -((x) - cursor.x());
        moveY = (y) - cursor.y();

        cursor.setX(x);
        cursor.setY(y);

        moveX -= acc_plus(512 + irXFreeZone, 1024, cursor.x(), irXSensitivity);
        moveX -= acc_minus(512 - irXFreeZone, 0, cursor.x(), irXSensitivity);
        moveY += acc_plus(384 + irYFreeZone, 768, cursor.y(), irYSensitivity);
        moveY += acc_minus(384 - irYFreeZone, 0, cursor.y(), irYSensitivity);
    }
}

void ProfileManager::infraredAccSection()
{
    if (timeout)
        return;

    virtualEvent->moveMousePointerRel(moveX, moveY);
}
