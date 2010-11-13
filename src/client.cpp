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

#define DAEMON_NAME "wiimotedev[client]"
#define PID_FILE "/var/run/wiimotedev-client.pid"
#define PID_MODE 0644

#include "config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

#include "syslog/syslog.h"
#include "network/clientmanager.h"

#include <QCoreApplication>
#include <QFileInfo>

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
  pointer = &application;

  application.setApplicationName(DAEMON_NAME);
  application.setApplicationVersion(
    QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
    QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
    QString::number(WIIMOTEDEV_VERSION_PATCH));

  if (application.arguments().indexOf("--help") != -1) {
    qDebug("Wiimotedev-client argument list\n");
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

  if (application.arguments().indexOf("--no-daemon") == -1)
  {
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

  systemlog::open(DAEMON_NAME);
  systemlog::information("system service started");

  if (additional_debug)
    systemlog::debug("additional debug mode switch-on");

  qRegisterMetaType< QList< irpoint> >("QList< irpoint>");
  qRegisterMetaType< QList< accdata> >("QList< accdata>");
  qRegisterMetaType< QList< stickdata> >("QList< stickdata>");

  qRegisterMetaType< irpoint>("irpoint");
  qRegisterMetaType< accdata>("accdata");
  qRegisterMetaType< stickdata>("stickdata");

  signal(SIGHUP, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGPIPE, signal_handler);

  ConnectionManager manager;
  manager.start();
  application.exec();
  manager.setTerminateRequest(true);
  manager.wait();
  int result = manager.result;

  systemlog::information("system service closed");
  systemlog::close();

  exit(result);
}
