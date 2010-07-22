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

#ifndef SYSLOG_H
#define SYSLOG_H

#include <QString>

#ifdef SYSLOG_SUPPORT

    #include <syslog.h>

    #define syslog_open(x) setlogmask(LOG_UPTO(LOG_INFO)); openlog(x, LOG_CONS, LOG_USER);
    #define syslog_close() closelog();
    #define syslog_message(x) syslog(LOG_INFO, "%s", x);

#else

    #define syslog_open(x)      ;
    #define syslog_close()      ;
    #define syslog_message(x)   ;

#endif

inline void syslog(const QString message)
{
    syslog_message(message.toAscii().constData());
};

#endif // SYSLOG_H
