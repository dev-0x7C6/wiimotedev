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

#include "devices/classicgamepad.h"

ClassicGamepadDevice::ClassicGamepadDevice(QString deviceName) :
  UInputObject(),
  m_deviceName(deviceName)
{
  if (m_deviceName.isEmpty())
    m_deviceName = QString::fromUtf8("Classic Gamepad Device (undefined)");

  centerStick(ClassicGamepadDevice::LeftStick);
  centerStick(ClassicGamepadDevice::RightStick);
  centerStick(ClassicGamepadDevice::DpadStick);
}

bool ClassicGamepadDevice::uinput_open() {
  if (alreadyOpened)
    uinput_close();

  if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
    qWarning("%s: Unable to open %s", m_deviceName.toAscii().constData(), uinputFile.toAscii().constData());
    return false;
  }

  memset(&dev, 0, sizeof(dev));

  strncpy(dev.name, m_deviceName.toAscii().constData(), m_deviceName.length());

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
  linux_register_keybit(BTN_MODE);
  linux_register_keybit(BTN_START);

  linux_register_absbit(CLASSIC_LEFT_STICK_LINUX_AXIS_X);
  linux_register_absbit(CLASSIC_LEFT_STICK_LINUX_AXIS_Y);
  linux_register_absbit(CLASSIC_RIGHT_STICK_LINUX_AXIS_X);
  linux_register_absbit(CLASSIC_RIGHT_STICK_LINUX_AXIS_Y);
  linux_register_absbit(CLASSIC_DPAD_LINUX_AXIS_X);
  linux_register_absbit(CLASSIC_DPAD_LINUX_AXIS_Y);

  linux_abs_set_range(CLASSIC_LEFT_STICK_LINUX_AXIS_X, CLASSIC_LEFT_STICK_MAX, CLASSIC_LEFT_STICK_MIN);
  linux_abs_set_range(CLASSIC_LEFT_STICK_LINUX_AXIS_Y, CLASSIC_LEFT_STICK_MAX, CLASSIC_LEFT_STICK_MIN);
  linux_abs_set_range(CLASSIC_RIGHT_STICK_LINUX_AXIS_X, CLASSIC_RIGHT_STICK_MAX, CLASSIC_RIGHT_STICK_MIN);
  linux_abs_set_range(CLASSIC_RIGHT_STICK_LINUX_AXIS_Y, CLASSIC_RIGHT_STICK_MAX, CLASSIC_RIGHT_STICK_MIN);
  linux_abs_set_range(CLASSIC_DPAD_LINUX_AXIS_X, CLASSIC_DPAD_MAX, CLASSIC_DPAD_MIN);
  linux_abs_set_range(CLASSIC_DPAD_LINUX_AXIS_Y, CLASSIC_DPAD_MAX, CLASSIC_DPAD_MIN);

  write(uinput_fd, &dev, sizeof(dev));
  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    qWarning("%s: Unable to create virtual input device", m_deviceName.toAscii().constData());
    uinput_close();
    return false;
  }

  centerStick(ClassicGamepadDevice::LeftStick);
  centerStick(ClassicGamepadDevice::RightStick);
  centerStick(ClassicGamepadDevice::DpadStick);
  syncSticks();

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
  sendEvent(EV_KEY, BTN_MODE, (buttons & CLASSIC_BTN_HOME) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_START, (buttons & CLASSIC_BTN_PLUS) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_0, (buttons & CLASSIC_BTN_RIGHT) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_1, (buttons & CLASSIC_BTN_LEFT) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_2, (buttons & CLASSIC_BTN_DOWN) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_3, (buttons & CLASSIC_BTN_UP) ? CLASSIC_BUTTON_PUSHED : CLASSIC_BUTTON_RELEASED);
  sendEventSync();


  centerStick(ClassicGamepadDevice::DpadStick);
  if (buttons & CLASSIC_BTN_RIGHT) m_last_dpad_x = CLASSIC_DPAD_MAX; else
  if (buttons & CLASSIC_BTN_LEFT) m_last_dpad_x = CLASSIC_DPAD_MIN;
  if (buttons & CLASSIC_BTN_DOWN) m_last_dpad_y = CLASSIC_DPAD_MAX; else
  if (buttons & CLASSIC_BTN_UP) m_last_dpad_y = CLASSIC_DPAD_MIN;

  syncSticks();
}

void ClassicGamepadDevice::centerStick(Sticks stick) {
  switch (stick) {
  case ClassicGamepadDevice::LeftStick:
    m_last_l_stick_x = (CLASSIC_LEFT_STICK_MIN + CLASSIC_LEFT_STICK_MAX) / 2;
    m_last_l_stick_y = (CLASSIC_LEFT_STICK_MIN + CLASSIC_LEFT_STICK_MAX) / 2;
    break;
  case ClassicGamepadDevice::RightStick:
    m_last_r_stick_x = (CLASSIC_RIGHT_STICK_MIN + CLASSIC_RIGHT_STICK_MAX) / 2;
    m_last_r_stick_y = (CLASSIC_RIGHT_STICK_MIN + CLASSIC_RIGHT_STICK_MAX) / 2;
    break;
  case ClassicGamepadDevice::DpadStick:
    m_last_dpad_x = 0;
    m_last_dpad_y = 0;
  }
}


void ClassicGamepadDevice::setStick(Sticks stick, qint32 x, qint32 y) {
  switch (stick) {
  case ClassicGamepadDevice::LeftStick:
    y = 0x41 - y;
    if (CLASSIC_LEFT_STICK_MAX < x) x = CLASSIC_LEFT_STICK_MAX; else
    if (CLASSIC_LEFT_STICK_MIN > x) x = CLASSIC_LEFT_STICK_MIN;
    if (CLASSIC_LEFT_STICK_MAX < y) y = CLASSIC_LEFT_STICK_MAX; else
    if (CLASSIC_LEFT_STICK_MIN > y) y = CLASSIC_LEFT_STICK_MIN;
    m_last_l_stick_x = x;
    m_last_l_stick_y = y;
    break;

  case ClassicGamepadDevice::RightStick:
    y = 0x1F - y;
    if (CLASSIC_RIGHT_STICK_MAX < x) x = CLASSIC_RIGHT_STICK_MAX; else
    if (CLASSIC_RIGHT_STICK_MIN > x) x = CLASSIC_RIGHT_STICK_MIN;
    if (CLASSIC_RIGHT_STICK_MAX < y) y = CLASSIC_RIGHT_STICK_MAX; else
    if (CLASSIC_RIGHT_STICK_MIN > y) y = CLASSIC_RIGHT_STICK_MIN;
    m_last_r_stick_x = x;
    m_last_r_stick_y = y;
    break;

  case ClassicGamepadDevice::DpadStick:
    break;
  }

  syncSticks();
}

void ClassicGamepadDevice::syncSticks() {
  sendEvent(EV_ABS, CLASSIC_DPAD_LINUX_AXIS_X, m_last_dpad_x);
  sendEvent(EV_ABS, CLASSIC_DPAD_LINUX_AXIS_Y, m_last_dpad_y);
  sendEvent(EV_ABS, CLASSIC_RIGHT_STICK_LINUX_AXIS_X, m_last_r_stick_x);
  sendEvent(EV_ABS, CLASSIC_RIGHT_STICK_LINUX_AXIS_Y, m_last_r_stick_y);
  sendEvent(EV_ABS, CLASSIC_LEFT_STICK_LINUX_AXIS_X, m_last_l_stick_x);
  sendEvent(EV_ABS, CLASSIC_LEFT_STICK_LINUX_AXIS_Y, m_last_l_stick_y);
  sendEventSync();
}


