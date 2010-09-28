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

#include "devices/mouse.h"

bool UInputMouse::uinput_open(QRect absRect, bool abs){
  if (alreadyOpened) uinput_close();
  if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
      qWarning("%s: Unable to open %s", "mouse", uinputFile.toAscii().constData());
      return false;
  }

  memset(&dev, 0, sizeof(dev));
  strncpy(dev.name, "mouse", 5);

  dev.id.product = UINPUT_PRODUCT_ID;
  dev.id.version = UINPUT_VERSION_ID;
  dev.id.vendor = UINPUT_VENDOR_ID;
  dev.id.bustype = UINPUT_BUSTYPE_ID;

/* Register events ---------------------------------------------- */
  linux_register_evbit(EV_KEY);
  if (abs) linux_register_evbit(EV_ABS) else
           linux_register_evbit(EV_REL);

/* Mouse events ------------------------------------------------- */
  for (register quint16 i = BTN_MOUSE; i < BTN_JOYSTICK; ++i)
      linux_register_keybit(i);

  if (abs) {
      linux_register_absbit(ABS_X);
      linux_register_absbit(ABS_Y);
      linux_abs_set_range(ABS_X, absRect.right(), absRect.left());
      linux_abs_set_range(ABS_Y, absRect.bottom(), absRect.top());
  } else {
      linux_register_relbit(REL_X);
      linux_register_relbit(REL_Y);
      linux_register_relbit(REL_HWHEEL);
      linux_register_relbit(REL_WHEEL);
  }


  write(uinput_fd, &dev, sizeof(dev));
  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
      qWarning("%s: Unable to create virtual input device", "mouse");
      uinput_close();
      return false;
  }
  return (alreadyOpened = true);
}

void UInputMouse::moveMousePointerRel(qint32 x, qint32 y) {
  if (x) sendEvent(EV_REL, REL_X, x);
  if (y) sendEvent(EV_REL, REL_Y, y);
  sendEventSync();
}

void UInputMouse::pressMouseButton(quint16 button){
  if (button < BTN_MOUSE || button >= BTN_JOYSTICK)
      return;

  sendEvent(EV_KEY, button, true);
  sendEventSync();
}

void UInputMouse::releaseMouseButton(quint16 button){
  if (button < BTN_MOUSE || button >= BTN_JOYSTICK)
      return;

  sendEvent(EV_KEY, button, false);
  sendEventSync();
}

void UInputMouse::moveMouseVWheel(qint32 direction) {
  if (direction)
      return;

  sendEvent(EV_REL, REL_WHEEL, direction);
  sendEventSync();
}

void UInputMouse::moveMouseHWheel(qint32 direction) {
  if (direction)
      return;

  sendEvent(EV_REL, REL_WHEEL, direction);
  sendEventSync();
}

void UInputMouse::moveMousePointerAbs(qint32 x, qint32 y) {
  if (x) sendEvent(EV_ABS, ABS_X, x);
  if (y) sendEvent(EV_ABS, ABS_Y, y);
  sendEventSync();
}