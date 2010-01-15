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

#define __daemon

#ifdef __daemon
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <unistd.h>
    #include <syslog.h>
    #include <string.h>
#endif

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include "src/connectionmanager.h"

#include "datalogger.h"

const QString confFile("/etc/wiimotedev/wiimotedev.conf");
const QString scancodeFile("/etc/wiimotedev/scancode.ini");

const QString pidFile("/var/run/wiimotedev.pid");
const QString logFile("/var/log/wiimotedev.log");

int verboseLevel = 7;

QString filePathWiimotedev = "/etc/wiimotedev/wiimotedev.conf";
QString filePathScancode = "/etc/wiimotedev/scancode.ini";
QString filePathLogfile = "/var/log/wiimotedev.log";

DataLogger logfile;

QMap < QString, quint64> devicebuttons;
QMap < QString, quint16> scancodes;

// Wiimote filters
    quint64 wiimoteButtonFilter = 0;
    quint64 wiimoteShiftFilter = 0;
    quint64 wiimoteTiltFilter = 0;

// Nunchuk filters
    quint64 nunchukButtonFilter = 0;
    quint64 nunchukShiftFilter = 0;
    quint64 nunchukStickFilter = 0;
    quint64 nunchukTiltFilter = 0;

// Classic filters
    quint64 classicButtonFilter = 0;
    quint64 classicShiftFilter = 0;
    quint64 classicLStickFilter = 0;
    quint64 classicRStickFilter = 0;
    quint64 classicTiltFilter = 0;

int main(int argc, char *argv[])
{    
#ifdef __daemon
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

    QFile file(pidFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << qint64(sid);
        file.close();
    } else
        exit(EXIT_FAILURE);
#endif


    QCoreApplication application(argc, argv);
    application.setApplicationName("Wiimotedev daemon");
    application.setApplicationVersion("0.10");

    logfile.openLog(logFile);
    logfile.setVerboseLevel(4);
    logfile.addLine("");
    logfile.addInfoLine("wiimotedev logging started");

    devicebuttons.insert("wiimote.1", static_cast< quint64>(  true) << devicebuttons.count());
    devicebuttons.insert("wiimote.2", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.a", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.b", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.plus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.minus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.home", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.forward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.backward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.shift.shake", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.front", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.back", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("wiimote.tilt.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.z", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.c", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.stick.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.forward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.backward", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.shift.shake", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.front", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.back", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("nunchuk.tilt.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.x", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.y", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.a", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.b", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.l", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.r", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.zl", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.zr", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.minus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.plus", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.home", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.lstick.right", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.up", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.down", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.left", static_cast< quint64>( true) << devicebuttons.count());
    devicebuttons.insert("classic.rstick.right", static_cast< quint64>( true) << devicebuttons.count());

    wiimoteButtonFilter |= devicebuttons.value("wiimote.1");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.2");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.a");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.b");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.minus");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.plus");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.home");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.up");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.down");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.left");
    wiimoteButtonFilter |= devicebuttons.value("wiimote.right");

    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.up");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.down");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.left");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.right");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.forward");
    wiimoteShiftFilter |= devicebuttons.value("wiimote.shift.backward");

    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.front");
    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.back");
    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.left");
    wiimoteTiltFilter |= devicebuttons.value("wiimote.tilt.right");

    wiimoteButtonFilter = ~wiimoteButtonFilter;
    wiimoteShiftFilter = ~wiimoteShiftFilter;
    wiimoteTiltFilter = ~wiimoteTiltFilter;

    nunchukButtonFilter |= devicebuttons.value("nunchuk.c");
    nunchukButtonFilter |= devicebuttons.value("nunchuk.z");

    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.up");
    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.down");
    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.left");
    nunchukStickFilter |= devicebuttons.value("nunchuk.stick.right");

    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.up");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.down");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.left");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.right");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.forward");
    nunchukShiftFilter |= devicebuttons.value("nunchuk.shift.backward");

    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.front");
    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.back");
    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.left");
    nunchukTiltFilter |= devicebuttons.value("nunchuk.tilt.right");

    nunchukButtonFilter = ~nunchukButtonFilter;
    nunchukShiftFilter = ~nunchukShiftFilter;
    nunchukStickFilter = ~nunchukStickFilter;
    nunchukTiltFilter = ~nunchukTiltFilter;

    classicButtonFilter |= devicebuttons.value("classic.a");
    classicButtonFilter |= devicebuttons.value("classic.b");
    classicButtonFilter |= devicebuttons.value("classic.x");
    classicButtonFilter |= devicebuttons.value("classic.y");
    classicButtonFilter |= devicebuttons.value("classic.l");
    classicButtonFilter |= devicebuttons.value("classic.r");
    classicButtonFilter |= devicebuttons.value("classic.zl");
    classicButtonFilter |= devicebuttons.value("classic.zr");
    classicButtonFilter |= devicebuttons.value("classic.minus");
    classicButtonFilter |= devicebuttons.value("classic.plus");
    classicButtonFilter |= devicebuttons.value("classic.home");
    classicButtonFilter |= devicebuttons.value("classic.up");
    classicButtonFilter |= devicebuttons.value("classic.down");
    classicButtonFilter |= devicebuttons.value("classic.left");
    classicButtonFilter |= devicebuttons.value("classic.right");

    classicLStickFilter |= devicebuttons.value("classic.lstick.up");
    classicLStickFilter |= devicebuttons.value("classic.lstick.down");
    classicLStickFilter |= devicebuttons.value("classic.lstick.left");
    classicLStickFilter |= devicebuttons.value("classic.lstick.right");

    classicRStickFilter |= devicebuttons.value("classic.rstick.up");
    classicRStickFilter |= devicebuttons.value("classic.rstick.down");
    classicRStickFilter |= devicebuttons.value("classic.rstick.left");
    classicRStickFilter |= devicebuttons.value("classic.rstick.right");

    classicButtonFilter = ~classicButtonFilter;
    classicLStickFilter = ~classicLStickFilter;
    classicRStickFilter = ~classicRStickFilter;

    ConnectionManager manager;
    manager.start();


    return application.exec();

    logfile.addInfoLine("wiimotedev logging finished");
    logfile.closeLog();
}
