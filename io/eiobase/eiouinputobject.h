/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#ifndef UINPUT_GENERAL_H
#define UINPUT_GENERAL_H

#include <QObject>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "linux/usr/include/wiimotedev/consts.h"

#define linux_register_evbit(x) ioctl(uinput_fd, UI_SET_EVBIT, x);
#define linux_register_keybit(x) ioctl(uinput_fd, UI_SET_KEYBIT, x);
#define linux_register_relbit(x) ioctl(uinput_fd, UI_SET_RELBIT, x);
#define linux_register_absbit(x) ioctl(uinput_fd, UI_SET_ABSBIT, x);

#define linux_abs_set_range(abs, max, min) dev.absmax[abs] = max; \
                                           dev.absmin[abs] = min; \
                                           dev.absflat[abs] = 0; \
                                           dev.absfuzz[abs] = 0;

#define UINPUT_PRODUCT_ID 0x01
#define UINPUT_VENDOR_ID 0x01
#define UINPUT_VERSION_ID 0x01
#define UINPUT_BUSTYPE_ID BUS_USB

class EIO_UInputObject
{
protected:
  QString uinputFile;

  int uinput_fd;
  struct uinput_user_dev dev;

public:
  explicit EIO_UInputObject();
  virtual ~EIO_UInputObject();

  bool alreadyOpened;
  virtual void uinput_close(bool force = true);

  QString path(){ return uinputFile; }

  void sendEvent(uint16 type, uint16 code, int32 value);
  void sendEventSync();

};

#endif // UINPUT_GENERAL_H
