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

#include "devices/wiimotegamepad.h"

WiimoteGamepadDevice::WiimoteGamepadDevice(QString deviceName, QObject *parent) :
  UInputObject(parent),
  deviceName(deviceName)
{
  if (deviceName.isEmpty())
    deviceName = "noname";
}


bool WiimoteGamepadDevice::uinput_open() {
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

  linux_register_evbit(EV_KEY);
  linux_register_evbit(EV_MSC);
  linux_register_evbit(EV_ABS);

  linux_register_keybit(BTN_0);
  linux_register_keybit(BTN_1);
  linux_register_keybit(BTN_2);
  linux_register_keybit(BTN_3);

  linux_register_keybit(BTN_A);
  linux_register_keybit(BTN_B);
  linux_register_keybit(BTN_X);
  linux_register_keybit(BTN_Y);
  linux_register_keybit(BTN_SELECT);
  linux_register_keybit(BTN_MODE);
  linux_register_keybit(BTN_START);

  linux_register_keybit(BTN_C);
  linux_register_keybit(BTN_Z);

  linux_register_absbit(ABS_X);
  linux_register_absbit(ABS_Y);
  linux_register_absbit(ABS_RX);
  linux_register_absbit(ABS_RY);
  linux_register_absbit(ABS_HAT0X);
  linux_register_absbit(ABS_HAT0Y);
  linux_register_absbit(ABS_HAT1X);
  linux_register_absbit(ABS_HAT1Y);

  linux_abs_set_range(ABS_X, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
  linux_abs_set_range(ABS_Y, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
  linux_abs_set_range(ABS_RX, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
  linux_abs_set_range(ABS_RY, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);

  linux_abs_set_range(ABS_HAT0X, WIIMOTE_PITCH_MAX, WIIMOTE_PITCH_MIN);
  linux_abs_set_range(ABS_HAT0Y, WIIMOTE_ROLL_MAX, WIIMOTE_ROLL_MIN);
  linux_abs_set_range(ABS_HAT1X, NUNCHUK_PITCH_MAX, NUNCHUK_PITCH_MIN);
  linux_abs_set_range(ABS_HAT1Y, NUNCHUK_ROLL_MAX, NUNCHUK_ROLL_MIN);

  write(uinput_fd, &dev, sizeof(dev));
  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    qWarning("%s: Unable to create virtual input device", staticInterfaceName());
    uinput_close();
    return false;
  }

  return (alreadyOpened = true);
}

void WiimoteGamepadDevice::setWiimoteButtons(quint64 buttons) {
  sendEvent(EV_KEY, BTN_A, (buttons & WIIMOTE_BTN_A) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_B, (buttons & WIIMOTE_BTN_B) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_X, (buttons & WIIMOTE_BTN_1) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_Y, (buttons & WIIMOTE_BTN_2) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_START, (buttons & WIIMOTE_BTN_PLUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_MODE, (buttons & WIIMOTE_BTN_HOME) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_SELECT, (buttons & WIIMOTE_BTN_MINUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_0, (buttons & WIIMOTE_BTN_RIGHT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_1, (buttons & WIIMOTE_BTN_LEFT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_2, (buttons & WIIMOTE_BTN_DOWN) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_3, (buttons & WIIMOTE_BTN_UP) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

  register qint8 x = 0;
  register qint8 y = 0;

  if (buttons & WIIMOTE_BTN_RIGHT) x = WIIMOTE_DPAD_MAX; else
  if (buttons & WIIMOTE_BTN_LEFT) x = WIIMOTE_DPAD_MIN;
  if (buttons & WIIMOTE_BTN_DOWN) y = WIIMOTE_DPAD_MAX; else
  if (buttons & WIIMOTE_BTN_UP) y = WIIMOTE_DPAD_MIN;

  sendEvent(EV_ABS, ABS_RX, x);
  sendEvent(EV_ABS, ABS_RX, y);
  sendEventSync();
}

void WiimoteGamepadDevice::setWiimoteTilts(double pitch, double roll) {
  register qint32 x = (pitch * 100);
  register qint32 y = (roll * 100);

  sendEvent(EV_ABS, ABS_HAT0X, x);
  sendEvent(EV_ABS, ABS_HAT0Y, y);
  sendEventSync();
}

void WiimoteGamepadDevice::setNunchukButtons(quint64 buttons) {
  sendEvent(EV_KEY, BTN_C, (buttons & NUNCHUK_BTN_C) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_Z, (buttons & NUNCHUK_BTN_Z) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
  sendEventSync();
}

void WiimoteGamepadDevice::setNunchukStick(qint32 x, qint32 y) {
  sendEvent(EV_ABS, ABS_X, x);
  sendEvent(EV_ABS, ABS_Y, y);
  sendEventSync();
}

void WiimoteGamepadDevice::setNunchukTilts(double pitch, double roll) {
  register qint32 x = (pitch * 100);
  register qint32 y = (roll * 100);

  sendEvent(EV_ABS, ABS_HAT1X, x);
  sendEvent(EV_ABS, ABS_HAT1Y, y);
  sendEventSync();
}
