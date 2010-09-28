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

#include "devices/classicgamepad.h"

ClassicGamepadDevice::ClassicGamepadDevice(QString deviceName) :
  UInputObject(),
  deviceName(deviceName)
{
  if (deviceName.isEmpty())
    deviceName = "noname";
}

bool ClassicGamepadDevice::uinput_open() {
  if (alreadyOpened)
    uinput_close();

  if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
    qWarning("%s: Unable to open %s", deviceName.toAscii().constData(), uinputFile.toAscii().constData());
    return false;
  }

  memset(&dev, 0, sizeof(dev));

  strncpy(dev.name, deviceName.toAscii().constData(), deviceName.length());

  dev.id.product = UINPUT_PRODUCT_ID;
  dev.id.version = UINPUT_VERSION_ID;
  dev.id.vendor = UINPUT_VENDOR_ID;
  dev.id.bustype = UINPUT_BUSTYPE_ID;

/* Register events ---------------------------------------------- */
  linux_register_evbit(EV_KEY);
  linux_register_evbit(EV_MSC);
  linux_register_evbit(EV_ABS);

/* Joystick events ---------------------------------------------- */
  linux_register_keybit(BTN_0); // arrows
  linux_register_keybit(BTN_1); // arrows
  linux_register_keybit(BTN_2); // arrows
  linux_register_keybit(BTN_3); // arrows

  linux_register_keybit(BTN_A);
  linux_register_keybit(BTN_B);
  linux_register_keybit(BTN_X);
  linux_register_keybit(BTN_Y);
  linux_register_keybit(BTN_TL);
  linux_register_keybit(BTN_TR);
  linux_register_keybit(BTN_TL2);
  linux_register_keybit(BTN_TR2);
  linux_register_keybit(BTN_SELECT);
  linux_register_keybit(BTN_START);

  linux_register_absbit(ABS_X);
  linux_register_absbit(ABS_Y);
  linux_register_absbit(ABS_RX);
  linux_register_absbit(ABS_RY);

  linux_abs_set_range(ABS_X, CLASSIC_LEFT_STICK_MAX, CLASSIC_LEFT_STICK_MIN);
  linux_abs_set_range(ABS_Y, CLASSIC_LEFT_STICK_MAX, CLASSIC_LEFT_STICK_MIN);
  linux_abs_set_range(ABS_RX, CLASSIC_RIGHT_STICK_MAX, CLASSIC_RIGHT_STICK_MIN);
  linux_abs_set_range(ABS_RY, CLASSIC_RIGHT_STICK_MAX, CLASSIC_RIGHT_STICK_MIN);

  linux_register_absbit(ABS_HAT0X);
  linux_register_absbit(ABS_HAT0Y);
  linux_abs_set_range(ABS_HAT0X, CLASSIC_DPAD_MAX, CLASSIC_DPAD_MIN);
  linux_abs_set_range(ABS_HAT0Y, CLASSIC_DPAD_MAX, CLASSIC_DPAD_MIN);

  write(uinput_fd, &dev, sizeof(dev));
  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    qWarning("%s: Unable to create virtual input device", deviceName.toAscii().constData());
    uinput_close();
    return false;
  }
  return (alreadyOpened = true);
}

void ClassicGamepadDevice::setButtons(quint64 buttons) {
  sendEvent(EV_KEY, BTN_A, (buttons & CLASSIC_BTN_A) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_B, (buttons & CLASSIC_BTN_B) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_X, (buttons & CLASSIC_BTN_X) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_Y, (buttons & CLASSIC_BTN_Y) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_TL, (buttons & CLASSIC_BTN_L) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_TR, (buttons & CLASSIC_BTN_R) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_TL2, (buttons & CLASSIC_BTN_ZL) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_TR2, (buttons & CLASSIC_BTN_ZR) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_SELECT, (buttons & CLASSIC_BTN_MINUS) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_START, (buttons & CLASSIC_BTN_PLUS) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_0, (buttons & CLASSIC_BTN_RIGHT) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_1, (buttons & CLASSIC_BTN_LEFT) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_2, (buttons & CLASSIC_BTN_DOWN) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_3, (buttons & CLASSIC_BTN_UP) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);

  register qint8 x = 0;
  register qint8 y = 0;

  if (buttons & CLASSIC_BTN_RIGHT) x = CLASSIC_DPAD_MAX; else
  if (buttons & CLASSIC_BTN_LEFT) x = CLASSIC_DPAD_MIN;
  if (buttons & CLASSIC_BTN_DOWN) y = CLASSIC_DPAD_MAX; else
  if (buttons & CLASSIC_BTN_UP) y = CLASSIC_DPAD_MIN;

  sendEvent(EV_ABS, ABS_HAT0X, x);
  sendEvent(EV_ABS, ABS_HAT0X, y);

  sendEventSync();
}

void ClassicGamepadDevice::setLeftStick(qint32 x, qint32 y) {
  sendEvent(EV_ABS, ABS_X, x);
  sendEvent(EV_ABS, ABS_Y, y);
  sendEventSync();
}

void ClassicGamepadDevice::setRightStick(qint32 x, qint32 y) {
  sendEvent(EV_ABS, ABS_RX, x);
  sendEvent(EV_ABS, ABS_RY, y);
  sendEventSync();
}
