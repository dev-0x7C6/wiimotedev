/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#define DAEMON_NAME "wiimotedev[io]"
#define PID_FILE "/var/run/wiimotedev-io.pid"
#define PID_MODE 0644

#include "../config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

#include "adaptors/adaptors.h"
#include "headers/consts.h"
#include "syslog/syslog.h"
#include "eiomanager/manager.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

const QString scancodeFile("/etc/wiimotedev/scancode.conf");

QMap < QString, uint64> devicebuttons;
QMap < QString, uint> scancodes;

bool additional_debug = false;

QCoreApplication *pointer;

void signal_handler(int sig) {
  switch(sig) {
    case SIGHUP:
    case SIGTERM:
    case SIGINT:
    case SIGQUIT: pointer->quit(); break;
    case SIGPIPE: signal(SIGPIPE, signal_handler); break;
  }
}

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  qRegisterMetaType< uint8> ("uint8");
  qRegisterMetaType< uint16> ("uint16");
  qRegisterMetaType< uint32> ("uint32");
  qRegisterMetaType< uint64> ("uint64");
  qRegisterMetaType< int8> ("int8");
  qRegisterMetaType< int16> ("int16");
  qRegisterMetaType< int32> ("int32");
  qRegisterMetaType< int64> ("int64");
  pointer = &application;

  application.setApplicationName(DAEMON_NAME);
  application.setApplicationVersion(
    QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
    QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
    QString::number(WIIMOTEDEV_VERSION_PATCH));

  if (application.arguments().indexOf("--help") != -1) {
    qDebug("Wiimotedev-daemon argument list\n");
    qDebug("  --debug\t\tfor additional debug output");
    qDebug("  --help\t\tprint help page");
    qDebug("  --no-daemon\t\tdo not run in background");
    qDebug("  --no-quiet\t\tdo not block stdout messages");
    qDebug("  --version\t\tprint version");
    qDebug(" ");
    exit(EXIT_SUCCESS);
  }

  if (application.arguments().indexOf("--version") != -1) {
    qDebug("Version: %d.%d.%d",
           WIIMOTEDEV_VERSION_MAJOR,
           WIIMOTEDEV_VERSION_MINOR,
           WIIMOTEDEV_VERSION_PATCH);
    exit(EXIT_SUCCESS);
  }

  if (getuid()) {
    qDebug("root privilages needed.");
    exit(EXIT_FAILURE);
  }

  additional_debug = (application.arguments().indexOf("--debug") != -1);

  if (application.arguments().indexOf("--no-daemon") == -1) {
    QFileInfo info(PID_FILE);
    if (info.isFile())
      exit(EXIT_FAILURE);

    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    pid_t sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);
    if (chdir("/") < 0) exit(EXIT_FAILURE);

    int fd = open(PID_FILE, O_CREAT | O_WRONLY | O_SYNC, PID_MODE);

    if (!fd) exit(EXIT_FAILURE);

    write(fd, QString::number(sid).toAscii().constData(), QString::number(sid).length());
    close(fd);
  }

  if (application.arguments().indexOf("--no-quiet") == -1) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  }

  signal(SIGHUP, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGPIPE, signal_handler);

  systemlog::open(DAEMON_NAME);
  systemlog::information("system service started");

  if (additional_debug)
    systemlog::debug("additional debug mode switch-on");

  QSettings settings(scancodeFile, QSettings::IniFormat);
  settings.beginGroup("scancode");

  QStringList list = settings.allKeys();
  for (register int32 i = 0; i < settings.allKeys().count(); ++i)
      scancodes[QString(settings.allKeys().at(i)).toLower().remove(QChar(' '))] = settings.value(settings.allKeys().at(i), 0).toInt();

  settings.endGroup();

  devicebuttons.insert("wiimote.1", WIIMOTE_BTN_1);
  devicebuttons.insert("wiimote.2", WIIMOTE_BTN_2);
  devicebuttons.insert("wiimote.a", WIIMOTE_BTN_A);
  devicebuttons.insert("wiimote.b", WIIMOTE_BTN_B);
  devicebuttons.insert("wiimote.plus", WIIMOTE_BTN_PLUS);
  devicebuttons.insert("wiimote.minus", WIIMOTE_BTN_MINUS);
  devicebuttons.insert("wiimote.home", WIIMOTE_BTN_HOME);
  devicebuttons.insert("wiimote.up", WIIMOTE_BTN_UP);
  devicebuttons.insert("wiimote.down", WIIMOTE_BTN_DOWN);
  devicebuttons.insert("wiimote.left", WIIMOTE_BTN_LEFT);
  devicebuttons.insert("wiimote.right", WIIMOTE_BTN_RIGHT);
  devicebuttons.insert("wiimote.shift.up", WIIMOTE_BTN_SHIFT_UP);
  devicebuttons.insert("wiimote.shift.down", WIIMOTE_BTN_SHIFT_DOWN);
  devicebuttons.insert("wiimote.shift.left", WIIMOTE_BTN_SHIFT_LEFT);
  devicebuttons.insert("wiimote.shift.right", WIIMOTE_BTN_SHIFT_RIGHT);
  devicebuttons.insert("wiimote.shift.forward", WIIMOTE_BTN_SHIFT_FORWARD);
  devicebuttons.insert("wiimote.shift.backward", WIIMOTE_BTN_SHIFT_BACKWARD);
  devicebuttons.insert("wiimote.tilt.front", WIIMOTE_BTN_TILT_FRONT);
  devicebuttons.insert("wiimote.tilt.back", WIIMOTE_BTN_TILT_BACK);
  devicebuttons.insert("wiimote.tilt.left", WIIMOTE_BTN_TILT_LEFT);
  devicebuttons.insert("wiimote.tilt.right", WIIMOTE_BTN_TILT_RIGHT);
  devicebuttons.insert("nunchuk.z", NUNCHUK_BTN_Z);
  devicebuttons.insert("nunchuk.c", NUNCHUK_BTN_C);
  devicebuttons.insert("nunchuk.stick.up", NUNCHUK_BTN_STICK_UP);
  devicebuttons.insert("nunchuk.stick.down", NUNCHUK_BTN_STICK_DOWN);
  devicebuttons.insert("nunchuk.stick.left", NUNCHUK_BTN_STICK_LEFT);
  devicebuttons.insert("nunchuk.stick.right", NUNCHUK_BTN_STICK_RIGHT);
  devicebuttons.insert("nunchuk.shift.up", NUNCHUK_BTN_SHIFT_UP);
  devicebuttons.insert("nunchuk.shift.down", NUNCHUK_BTN_SHIFT_DOWN);
  devicebuttons.insert("nunchuk.shift.left", NUNCHUK_BTN_SHIFT_LEFT);
  devicebuttons.insert("nunchuk.shift.right", NUNCHUK_BTN_SHIFT_RIGHT);
  devicebuttons.insert("nunchuk.shift.forward", NUNCHUK_BTN_SHIFT_FORWARD);
  devicebuttons.insert("nunchuk.shift.backward", NUNCHUK_BTN_SHIFT_BACKWARD);
  devicebuttons.insert("nunchuk.tilt.front", NUNCHUK_BTN_TILT_FRONT);
  devicebuttons.insert("nunchuk.tilt.back", NUNCHUK_BTN_TILT_BACK);
  devicebuttons.insert("nunchuk.tilt.left", NUNCHUK_BTN_TILT_LEFT);
  devicebuttons.insert("nunchuk.tilt.right", NUNCHUK_BTN_TILT_RIGHT);
  devicebuttons.insert("classic.x", CLASSIC_BTN_X);
  devicebuttons.insert("classic.y", CLASSIC_BTN_Y);
  devicebuttons.insert("classic.a", CLASSIC_BTN_A);
  devicebuttons.insert("classic.b", CLASSIC_BTN_B);
  devicebuttons.insert("classic.l", CLASSIC_BTN_L);
  devicebuttons.insert("classic.r", CLASSIC_BTN_R);
  devicebuttons.insert("classic.zl", CLASSIC_BTN_ZL);
  devicebuttons.insert("classic.zr", CLASSIC_BTN_ZR);
  devicebuttons.insert("classic.minus", CLASSIC_BTN_MINUS);
  devicebuttons.insert("classic.plus", CLASSIC_BTN_PLUS);
  devicebuttons.insert("classic.home", CLASSIC_BTN_HOME);
  devicebuttons.insert("classic.up", CLASSIC_BTN_UP);
  devicebuttons.insert("classic.down", CLASSIC_BTN_DOWN);
  devicebuttons.insert("classic.left", CLASSIC_BTN_LEFT);
  devicebuttons.insert("classic.right", CLASSIC_BTN_RIGHT);
  devicebuttons.insert("classic.lstick.up", CLASSIC_BTN_LSTICK_UP);
  devicebuttons.insert("classic.lstick.down", CLASSIC_BTN_LSTICK_DOWN);
  devicebuttons.insert("classic.lstick.left", CLASSIC_BTN_LSTICK_LEFT);
  devicebuttons.insert("classic.lstick.right", CLASSIC_BTN_LSTICK_RIGHT);
  devicebuttons.insert("classic.rstick.up", CLASSIC_BTN_RSTICK_UP);
  devicebuttons.insert("classic.rstick.down", CLASSIC_BTN_RSTICK_DOWN);
  devicebuttons.insert("classic.rstick.left", CLASSIC_BTN_RSTICK_LEFT);
  devicebuttons.insert("classic.rstick.right", CLASSIC_BTN_RSTICK_RIGHT);
  devicebuttons.insert("wiimote.shift.shake", WIIMOTE_BTN_SHIFT_SHAKE);
  devicebuttons.insert("nunchuk.shift.shake", NUNCHUK_BTN_SHIFT_SHAKE);

  qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
  qRegisterMetaType< QList< accdata> >("QList< accdata>");
  qRegisterMetaType< QList< stickdata> >("QList< stickdata>");

  qRegisterMetaType< irpoint>("irpoint");
  qRegisterMetaType< accdata>("accdata");
  qRegisterMetaType< stickdata>("stickdata");

  UInputProfileManager profileManager;

  application.exec();

  systemlog::information("system service closed");
  systemlog::close();

  exit(EXIT_SUCCESS);
}
