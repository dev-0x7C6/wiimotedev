/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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

#define DAEMON_NAME "wiimotedev"
#define DAEMON_VERSION "1.2" // 1.1.95
#define PID_FILE "/var/run/wiimotedev-daemon.pid"

//*Wiimotedev-daemon arguments

// --debug -> for additional debug output
// --help -> print help page
// --no-daemon -> do not run in daemon mode


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
        qDebug() << "  --help\t\tprint help page";
        qDebug() << "  --no-daemon\t\tdo not run in background";
        qDebug() << "";
        exit(EXIT_SUCCESS);
    }

    app_pointer = &application;

#ifdef DAEMON_SUPPORT
    if (application.arguments().indexOf("--no-daemon") == -1)
    {
        pid_t pid = fork();
        if (pid < 0) exit(EXIT_FAILURE);
        if (pid > 0) exit(EXIT_SUCCESS);

        pid_t sid = setsid();
        if (sid < 0) exit(EXIT_FAILURE);
        if (chdir("/") < 0) exit(EXIT_FAILURE);

        int fd = open(PID_FILE, O_CREAT | O_WRONLY | O_SYNC);

        if (!fd) exit(EXIT_FAILURE);

        QString out = QString::number(sid, 10);
        write(fd, out.toAscii().constData(), out.length());
        close(fd);

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }
#else
#endif

    syslog_open(DAEMON_NAME);
    syslog_message("system service started");

    additional_debug = (application.arguments().indexOf("--debug") != -1);

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
