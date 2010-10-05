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

#include "devices/eventdevice.h"

bool UInputEvent::uinput_open(bool replay){
  if (alreadyOpened) uinput_close();
  if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
    qWarning("event device: Unable to open %s", uinputFile.toAscii().constData());
    return false;
  }

  memset(&dev, 0, sizeof(dev));
  strncpy(dev.name, "event device", 12);

  dev.id.product = UINPUT_PRODUCT_ID;
  dev.id.version = UINPUT_VERSION_ID;
  dev.id.vendor = UINPUT_VENDOR_ID;
  dev.id.bustype = UINPUT_BUSTYPE_ID;

/* Register events ---------------------------------------------- */
  linux_register_evbit(EV_KEY);
  if (replay) linux_register_evbit(EV_REP);
  linux_register_evbit(EV_REL);

/* Keyboard events ---------------------------------------------- */
  for (register quint16 i = 0; i < 0xFF; ++i)
      linux_register_keybit(i);

/* Mouse events ------------------------------------------------- */
  for (register quint16 i = BTN_MOUSE; i < BTN_JOYSTICK; ++i)
      linux_register_keybit(i);

  linux_register_relbit(REL_X);
  linux_register_relbit(REL_Y);
  linux_register_relbit(REL_HWHEEL);
  linux_register_relbit(REL_WHEEL);

  write(uinput_fd, &dev, sizeof(dev));
  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
      qWarning("%s: Unable to create virtual input device", "event device");
      uinput_close();
      return false;
  }
  return (alreadyOpened = true);
}


void UInputEvent::pressKeyboardButton(quint16 button) {
  sendEvent(EV_KEY, button, 1);
  sendEventSync();
}

void UInputEvent::releaseKeyboardButton(quint16 button) {
  sendEvent(EV_KEY, button, 0);
  sendEventSync();
}

void UInputEvent::pressKeyboardButtonOnce(quint16 button) {
  sendEvent(EV_KEY, button, true);
  sendEventSync();
  sendEvent(EV_KEY, button, false);
  sendEventSync();
}

void UInputEvent::moveMouseVWheel(qint32 direction) {
  if (direction)
      return;

  sendEvent(EV_REL, REL_WHEEL, direction);
  sendEventSync();
}

void UInputEvent::moveMouseHWheel(qint32 direction) {
  if (direction)
      return;

  sendEvent(EV_REL, REL_HWHEEL, direction);
  sendEventSync();
}

void UInputEvent::moveMousePointerRel(qint32 x, qint32 y)
{
  if (x) sendEvent(EV_REL, REL_X, x);
  if (y) sendEvent(EV_REL, REL_Y, y);
  sendEventSync();
}
