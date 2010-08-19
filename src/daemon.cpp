/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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

#define DAEMON_NAME "wiimotedev"
#define DAEMON_VERSION "1.2" // 1.1.99
#define PID_FILE "/var/run/wiimotedev-daemon.pid"
#define PID_MODE 0644

//Wiimotedev-daemon arguments

// --debug -> for additional debug output
// --force-dbus -> enable dbus protocol
// --force-tcp -> enable tcp protocol
// --help -> print help page
// --no-daemon -> do not run in daemon mode
// --no-quiet -> do not block stdout messages

#include <stdlib.h>

#ifdef DAEMON_SUPPORT
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <unistd.h>
    #include <string.h>
#endif

#include <signal.h>

#include <QCoreApplication>
#include <QStringList>

#include "wiimotedev/manager.h"

void *app_pointer = 0;

bool additional_debug = false;
bool force_dbus = false;
bool force_tcp = false;

void signal_handler(int sig) {
    switch(sig) {
        case SIGHUP:
        case SIGTERM:
        case SIGINT:
        case SIGQUIT:
            static_cast< QCoreApplication*>( app_pointer)->quit();
            break;
        case SIGPIPE: // Ignore this signal
            signal(SIGPIPE, signal_handler);
            break;
    }
}

int main(int argc, char *argv[])
{        
    QCoreApplication application(argc, argv);
    application.setApplicationName(DAEMON_NAME);
    application.setApplicationVersion(DAEMON_VERSION);

    if (application.arguments().indexOf("--help") != -1)
    {
        qDebug() << "Wiimotedev-daemon argument list\n";
        qDebug() << "  --debug\t\tfor additional debug output";
        qDebug() << "  --force-dbus\t\tenable dbus protocol";
        qDebug() << "  --force-tcp\t\tenable tcp protocol";
        qDebug() << "  --help\t\tprint help page";
        qDebug() << "  --no-daemon\t\tdo not run in background";
        qDebug() << "  --no-quiet\t\tdo not block stdout messages";
        qDebug() << "";
        exit(EXIT_SUCCESS);
    }

    app_pointer = &application;

    additional_debug = (application.arguments().indexOf("--debug") != -1);
    force_dbus = (application.arguments().indexOf("--force-dbus") != -1);
    force_tcp = (application.arguments().indexOf("--force-tcp") != -1);

#ifdef DAEMON_SUPPORT
    if (application.arguments().indexOf("--no-daemon") == -1) {
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
#else
#endif

    if (application.arguments().indexOf("--no-quiet") == -1) {
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    syslog_open(DAEMON_NAME);
    syslog_message("system service started");

    if (additional_debug)
        syslog_message("additional debug mode switch-on");

    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGPIPE, signal_handler);

    ConnectionManager *manager_thread = new ConnectionManager();

    manager_thread->start();
    application.exec();
    manager_thread->terminateRequest();
    manager_thread->wait();

    delete manager_thread;

    syslog_message("system service closed");
    syslog_close();

    exit(EXIT_SUCCESS);
}
