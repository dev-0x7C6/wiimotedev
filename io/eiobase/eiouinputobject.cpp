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

#include "eiouinputobject.h"

#include <QStringList>
#include <QFile>

const QStringList UInputLocation(QStringList() << "/dev/uinput" << "/dev/input/uinput" << "/dev/misc/uinput");

EIO_UInputObject::EIO_UInputObject() {
  uinputFile = QString::fromUtf8("");
  uinput_fd = 0;
  alreadyOpened = false;

  foreach (const QString &file, UInputLocation) {
    if (QFile::exists(file)) {
      uinputFile = file;
      break;
    }
  }
}

EIO_UInputObject::~EIO_UInputObject() {

}

void EIO_UInputObject::uinput_close(bool force)
{
  if (!force && !alreadyOpened)
    return;

  ioctl(uinput_fd, UI_DEV_DESTROY);
  close(uinput_fd);
  uinput_fd = 0;
  alreadyOpened = false;
}

void EIO_UInputObject::sendEvent(uint16 type, uint16 code, int32 value) {
  if (!uinput_fd)
      return;

  struct input_event event;
  memset(&event.time, 0, sizeof(event.time));
  event.code = code;
  event.type = type;
  event.value = value;
  write(uinput_fd, &event, sizeof(event));
}

void EIO_UInputObject::sendEventSync() {
  sendEvent(EV_SYN, SYN_REPORT, 0);
}
