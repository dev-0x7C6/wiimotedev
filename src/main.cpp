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

#define DAEMON_NAME "Wiimotedev"
#define DAEMON_VERSION "1.0"
#define PID_FILE "/var/run/wiimotedev-daemon.pid"

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

#ifdef SYSLOG_SUPPORT
    #include <syslog.h>
#endif

#include <signal.h>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#include "connectionmanager.h"

const QString confFile("/etc/wiimotedev/wiimotedev.conf");
const QString scancodeFile("/etc/wiimotedev/scancode.ini");

QString filePathWiimotedev = "/etc/wiimotedev/wiimotedev.conf";
QString filePathScancode = "/etc/wiimotedev/scancode.ini";
QString filePathLogfile = "/var/log/wiimotedev.log";

QCoreApplication *application = 0;

void signal_handler(int sig) {
    switch(sig) {
        case SIGHUP:
        case SIGTERM:
        case SIGINT:
        case SIGQUIT:
            application->quit();
            break;
    }
}

int main(int argc, char *argv[])
{    
#ifdef DAEMON_SUPPORT
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);
    if (chdir("/") < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    QFile file(PID_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << qint64(sid);
        file.close();
    } else
        exit(EXIT_FAILURE);
#else
    qDebug("Daemon functions will be disabled");
#endif

    application = new QCoreApplication(argc, argv);
    application->setApplicationName(DAEMON_NAME);
    application->setApplicationVersion(DAEMON_VERSION);

#ifdef SYSLOG_SUPPORT
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(DAEMON_NAME, LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "daemon started");
#endif

    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);

    ConnectionManager manager;
    manager.start();

    application->exec();
    delete application;

#ifdef SYSLOG_SUPPORT
    syslog(LOG_INFO, "daemon closed");
#endif

    exit(EXIT_SUCCESS);
}
