/**********************************************************************************
 * Wiimotedev uinput, virtual devices system service                              *
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

#include "devices/general.h"

#include <QStringList>
#include <QFile>

const QStringList UInputLocation(QStringList() << "/dev/uinput" << "/dev/input/uinput" << "/dev/misc/uinput");

UInputObject::UInputObject() {
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

void UInputObject::uinput_close(bool force)
{
  if (!force && !alreadyOpened)
    return;

  ioctl(uinput_fd, UI_DEV_DESTROY);
  close(uinput_fd);
  uinput_fd = 0;
  alreadyOpened = false;
}

void UInputObject::sendEvent(quint16 type, quint16 code, qint32 value) {
  if (!uinput_fd)
      return;

  struct input_event event;
  memset(&event.time, 0, sizeof(event.time));
  event.code = code;
  event.type = type;
  event.value = value;
  write(uinput_fd, &event, sizeof(event));
}

void UInputObject::sendEventSync() {
  sendEvent(EV_SYN, SYN_REPORT, 0);
};
