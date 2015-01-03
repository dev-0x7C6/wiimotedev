/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2014  Bartłomiej Burdukiewicz                               *
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

#define DAEMON_NAME "wiimotedev-daemon"
#define PID_FILE "/var/run/wiimotedev-daemon.pid"
#define PID_MODE 0644

#include "../config.h"
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "wiimotedevcore.h"
#include "syslog/syslog.h"

#include <QCoreApplication>
#include <QFileInfo>

QCoreApplication *pointer;

void signal_handler(int sig) {
  switch (sig) {
    case SIGHUP:
    case SIGTERM:
    case SIGINT:
    case SIGALRM:
    case SIGQUIT:
      pointer->quit();
      break;

    case SIGPIPE:
      signal(SIGPIPE, signal_handler);
      break;
  }
}

int main(int argc, char *argv[]) {
  QCoreApplication application(argc, argv);
  application.setApplicationName(DAEMON_NAME);
  application.setApplicationVersion(
    QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
    QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
    QString::number(WIIMOTEDEV_VERSION_PATCH));
  pointer = &application;

  if (application.arguments().indexOf("--help") != -1) {
    printf("Wiimotedev-daemon argument list\n\n" \
           "  --help\t\tprint help page\n" \
           "  --no-daemon\t\tdo not run in background\n" \
           "  --no-quiet\t\tdo not block stdout messages\n" \
           "  --version\t\tprint version\n\n");
    exit(EXIT_SUCCESS);
  }

  if (application.arguments().indexOf("--version") != -1) {
    printf("Version: %d.%d.%d",
           WIIMOTEDEV_VERSION_MAJOR,
           WIIMOTEDEV_VERSION_MINOR,
           WIIMOTEDEV_VERSION_PATCH);
    exit(EXIT_SUCCESS);
  }

  if (getuid()) {
    printf("root privilages needed.\n");
    exit(EXIT_FAILURE);
  }

  pid_t pid;

  if (application.arguments().indexOf("--no-daemon") == -1) {
    pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    if (pid > 0) exit(EXIT_SUCCESS);

    pid_t sid = setsid();

    if (sid < 0) exit(EXIT_FAILURE);

    if (chdir("/") < 0) exit(EXIT_FAILURE);

    int fd = open(PID_FILE, O_CREAT | O_WRONLY | O_SYNC, PID_MODE);

    if (!fd) exit(EXIT_FAILURE);

    write(fd, QString::number(sid).toLocal8Bit().data(), QString::number(sid).length());
    close(fd);
    pid = sid;
  }

  if (application.arguments().indexOf("--no-quiet") == -1) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  }

  signal(SIGHUP, signal_handler);
  signal(SIGALRM, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGPIPE, signal_handler);
  systemlog::open(DAEMON_NAME);
  systemlog::information("service started");
  WiimotedevCore *manager_thread = new WiimotedevCore();
  manager_thread->start();
  application.exec();
  manager_thread->setThreadQuitStatus();
  manager_thread->wait();
  int result = manager_thread->result;
  delete manager_thread;
  systemlog::information("service quited");
  systemlog::close();

  if (getpid() == pid)
    unlink(PID_FILE);

  exit(result);
}
