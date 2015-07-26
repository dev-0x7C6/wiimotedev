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

#define DAEMON_NAME "wiimotedev-daemon"
#define PID_FILE "/var/run/wiimotedev-daemon.pid"
#define PID_MODE 0644

#include "../config.h"
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFileInfo>

#include "syslog/syslog.h"
#include "wiimotedevcore.h"

void signal_handler(int sig) {
  switch (sig) {
    case SIGHUP:
    case SIGTERM:
    case SIGINT:
    case SIGALRM:
    case SIGQUIT:
      qApp->quit();
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

  QCommandLineParser parser;
  parser.setApplicationDescription("Wiimotedev general service");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption optionNoQuiet({"q", "no-quiet"}, "do not block stdout messages");
  QCommandLineOption optionNoDaemon({"d", "no-daemon"}, "do not run in background");

  parser.addOption(optionNoDaemon);
  parser.addOption(optionNoQuiet);
  parser.process(application);

  if (getuid()) {
    std::cout << "root privilages needed." << std::endl;
    exit(EXIT_FAILURE);
  }

  pid_t pid;

  if (!parser.isSet(optionNoDaemon)) {
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

  if (parser.isSet(optionNoQuiet)) {
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

  std::unique_ptr<WiimotedevCore> core = std::make_unique<WiimotedevCore>();
  core->start();
  application.exec();
  core->interrupt();
  core->wait();

  systemlog::information("service quited");
  systemlog::close();

  if (getpid() == pid)
    unlink(PID_FILE);

  exit(core->result);
}
