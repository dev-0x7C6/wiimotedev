/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include "src/connectionmanager.h"
#include "src/profilemanager.h"

#include "datalogger.h"

// verbose levels
// 0 - nothing
// 1 - errors
// 2 - warnings
// 3 - hints
// 4 - connection informations
// 5 - developer informations
// 6 - advenced developer informations
// 7 - everything

int verboseLevel = 7;

QString filePathWiimotedev = "/etc/wiimotedev/wiimotedev.conf";
QString filePathScancode = "/etc/wiimotedev/scancode.conf";
QString filePathLogfile = "/var/log/wiimotedev.log";

DataLogger dataLogger;

int main(int argc, char *argv[])
{
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

    QFile file(QString::fromUtf8("/var/run/wiimotedev.pid"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << qint64(sid);
        file.close();
    } else
        exit(EXIT_FAILURE);

    QCoreApplication application(argc, argv);
    application.setApplicationName("wiimotedev daemon");
    application.setApplicationVersion("0.10");


    dataLogger.openLog(filePathLogfile);
    dataLogger.setVerboseLevel(4);
    dataLogger.addLine("");
    dataLogger.addInfoLine("wiimotedev logging started");

    ProfileManager::Initialization();

    ProfileManager profileManager;


    ConnectionManager manager(&profileManager);
    manager.start();


    return application.exec();

    ProfileManager::Finalization();

    dataLogger.addInfoLine("wiimotedev logging finished");
    dataLogger.closeLog();
}
