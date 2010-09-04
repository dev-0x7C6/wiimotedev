/**********************************************************************************
 * Wiimotedev client, wiiremote system service                                    *
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
#define DAEMON_VERSION "1.1.80" // 1.1.80
#define PID_FILE "/var/run/wiimotedev-client.pid"
#define PID_MODE 0644

//*Wiimotedev-client arguments

// --debug -> for additional debug output
// --help -> print help page
// --no-daemon -> do not run in daemon mode
// --no-quiet -> do not block stdout messages

#include <stdlib.h>

#ifdef DAEMON_MODE
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <stdio.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <unistd.h>
  #include <string.h>
#endif

#include <boost/scoped_ptr.hpp>
#include <signal.h>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#include "network/manager.h"

boost::scoped_ptr<QCoreApplication> application;

bool additional_debug = false;

void signal_handler(int sig) {
  switch(sig) {
    case SIGHUP:
    case SIGTERM:
    case SIGINT:
    case SIGQUIT: application.get()->quit(); break;
    case SIGPIPE: signal(SIGPIPE, signal_handler); break;
  }
}

int main(int argc, char *argv[])
{    
  application.reset(new QCoreApplication(argc, argv));
  application.get()->setApplicationName(DAEMON_NAME);
  application.get()->setApplicationVersion(DAEMON_VERSION);

  if (application.get()->arguments().indexOf("--help") != -1) {
    qDebug() << "Wiimotedev-client argument list\n";
    qDebug() << "  --debug\t\tfor additional debug output";
    qDebug() << "  --help\t\tprint help page";
    qDebug() << "  --no-daemon\t\tdo not run in background";
    qDebug() << "  --no-quiet\t\tdo not block stdout messages";
    qDebug() << "";
    exit(EXIT_SUCCESS);
  }

  additional_debug = (application.get()->arguments().indexOf("--debug") != -1);

#ifdef DAEMON_MODE
  if (application.arguments().indexOf("--no-daemon") == -1)
  {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    pid_t sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);
    if (chdir("/") < 0) exit(EXIT_FAILURE);

    int fd = open(PID_FILE, O_CREAT | O_WRONLY | O_SYNC, PID_MODE);

    if (!fd) exit(EXIT_FAILURE);

    QString out = QString::number(sid, 10);
    write(fd, out.toAscii().constData(), out.length());
    close(fd);
  }
#endif
  if (application.get()->arguments().indexOf("--no-quiet") == -1) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  }

#ifdef USE_SYSLOG
  systemlog::open(DAEMON_NAME);
  systemlog::information("system service started");

  if (additional_debug)
    systemlog::debug("additional debug mode switch-on");
#endif
  signal(SIGHUP, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGPIPE, signal_handler);

  ConnectionManager manager;
  manager.start();
  application.get()->exec();
  manager.terminateRequest();
  manager.wait();

#ifdef USE_SYSLOG
  systemlog::information("system service closed");
  systemlog::close();
#endif

  application.reset();

  exit(EXIT_SUCCESS);
}
