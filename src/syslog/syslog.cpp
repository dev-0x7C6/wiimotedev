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

#include "syslog.h"
#include <syslog.h>

void systemlog::open(const char *name) {
  openlog(name, LOG_PID, LOG_DAEMON);
  setlogmask(LOG_UPTO(LOG_DEBUG));
}

void systemlog::close() {
  closelog();
}

void systemlog::critical(const QString message) {
  syslog(LOG_CRIT, "%s", message.toAscii().constData());
}

void systemlog::debug(const QString message) {
  syslog(LOG_DEBUG, "%s", message.toAscii().constData());
}

void systemlog::error(const QString message) {
  syslog(LOG_ERR, "%s", message.toAscii().constData());
}

void systemlog::information(const QString message) {
  syslog(LOG_INFO, "%s", message.toAscii().constData());
}

void systemlog::notice(const QString message) {
  syslog(LOG_NOTICE, "%s", message.toAscii().constData());
}

void systemlog::warning(const QString message) {
  syslog(LOG_WARNING, "%s", message.toAscii().constData());
}
