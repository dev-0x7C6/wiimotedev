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

#include "devices/wiimotegamepad.h"

WiimoteGamepadDevice::WiimoteGamepadDevice(QString deviceName, Mode mode, Position horizontal) :
  UInputObject(),
  m_deviceName(deviceName),
  m_horizontal(horizontal),
  m_mode(mode),
  m_home_pressed(false)
{
  if (m_deviceName.isEmpty())
    m_deviceName = QString::fromUtf8("Wiimote Gamepad Device (undefined)");
}


bool WiimoteGamepadDevice::uinput_open() {
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
  linux_register_keybit(BTN_START);

  if (m_mode == WiimoteGamepadDevice::DPadPositionConstant)
    linux_register_keybit(BTN_MODE);


  linux_register_keybit(BTN_C);
  linux_register_keybit(BTN_Z);

  linux_register_absbit(NUNCHUK_STICK_LINUX_AXIS_X);
  linux_register_absbit(NUNCHUK_STICK_LINUX_AXIS_Y);
  linux_register_absbit(WIIMOTE_DPAD_LINUX_AXIS_X);
  linux_register_absbit(WIIMOTE_DPAD_LINUX_AXIS_Y);
  linux_register_absbit(WIIMOTE_PITCH_LINUX_AXIS);
  linux_register_absbit(WIIMOTE_ROLL_LINUX_AXIS);
  linux_register_absbit(NUNCHUK_PITCH_LINUX_AXIS);
  linux_register_absbit(NUNCHUK_ROLL_LINUX_AXIS);

  linux_abs_set_range(NUNCHUK_STICK_LINUX_AXIS_X, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
  linux_abs_set_range(NUNCHUK_STICK_LINUX_AXIS_Y, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
  linux_abs_set_range(WIIMOTE_DPAD_LINUX_AXIS_X, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
  linux_abs_set_range(WIIMOTE_DPAD_LINUX_AXIS_Y, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);

  linux_abs_set_range(WIIMOTE_PITCH_LINUX_AXIS, WIIMOTE_PITCH_MAX, WIIMOTE_PITCH_MIN);
  linux_abs_set_range(WIIMOTE_ROLL_LINUX_AXIS, WIIMOTE_ROLL_MAX, WIIMOTE_ROLL_MIN);
  linux_abs_set_range(NUNCHUK_PITCH_LINUX_AXIS, NUNCHUK_PITCH_MAX, NUNCHUK_PITCH_MIN);
  linux_abs_set_range(NUNCHUK_ROLL_LINUX_AXIS, NUNCHUK_ROLL_MAX, NUNCHUK_ROLL_MIN);

  centerStick(WiimoteGamepadDevice::NunchukStick);
  centerStick(WiimoteGamepadDevice::DpadStick);
  centerStick(WiimoteGamepadDevice::WiimoteAccelerometer);
  centerStick(WiimoteGamepadDevice::NunchukAccelerometer);

  write(uinput_fd, &dev, sizeof(dev));
  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    qWarning("%s: Unable to create virtual input device", m_deviceName.toAscii().constData());
    uinput_close();
    return false;
  }

  return (alreadyOpened = true);
}

#include <QDebug>

void WiimoteGamepadDevice::setWiimoteButtons(quint64 buttons) {
  sendEvent(EV_KEY, BTN_A, (buttons & WIIMOTE_BTN_A) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_B, (buttons & WIIMOTE_BTN_B) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_X, (buttons & WIIMOTE_BTN_1) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_Y, (buttons & WIIMOTE_BTN_2) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_START, (buttons & WIIMOTE_BTN_PLUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_SELECT, (buttons & WIIMOTE_BTN_MINUS) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_0, (buttons & WIIMOTE_BTN_RIGHT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_1, (buttons & WIIMOTE_BTN_LEFT) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_2, (buttons & WIIMOTE_BTN_DOWN) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_3, (buttons & WIIMOTE_BTN_UP) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

  if (m_mode == WiimoteGamepadDevice::DPadPositionConstant)
    sendEvent(EV_KEY, BTN_MODE, (buttons & WIIMOTE_BTN_HOME) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

  if (m_mode == WiimoteGamepadDevice::DPadPositionSwitchable)
    if (((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME) && (m_home_pressed == false))
      switch (m_horizontal) {
      case WiimoteGamepadDevice::GamepadHorizontal:
        m_horizontal = WiimoteGamepadDevice::GamepadVertical;
        break;
      case WiimoteGamepadDevice::GamepadVertical:
        m_horizontal = WiimoteGamepadDevice::GamepadHorizontal;
        break;
      }

  m_home_pressed = ((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME);

  m_last_dpad_x = 0x00;
  m_last_dpad_y = 0x00;

  if (buttons & WIIMOTE_BTN_RIGHT) m_last_dpad_x = WIIMOTE_DPAD_MAX; else
  if (buttons & WIIMOTE_BTN_LEFT) m_last_dpad_x = WIIMOTE_DPAD_MIN;
  if (buttons & WIIMOTE_BTN_DOWN) m_last_dpad_y = WIIMOTE_DPAD_MAX; else
  if (buttons & WIIMOTE_BTN_UP) m_last_dpad_y = WIIMOTE_DPAD_MIN;

  sendEventSync();
  syncAxes();
}

void WiimoteGamepadDevice::setNunchukButtons(quint64 buttons) {
  sendEvent(EV_KEY, BTN_C, (buttons & NUNCHUK_BTN_C) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_Z, (buttons & NUNCHUK_BTN_Z) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
  sendEventSync();
}

void WiimoteGamepadDevice::centerStick(Stick id) {
  switch (id) {
  case WiimoteGamepadDevice::NunchukStick:
    m_last_stick_x = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
    m_last_stick_y = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
    break;
  case WiimoteGamepadDevice::DpadStick:
    m_last_dpad_x = 0x00;
    m_last_dpad_y = 0x00;
    break;
  case WiimoteGamepadDevice::NunchukAccelerometer:
    m_last_nunchuk_acc_pitch = 0x00;
    m_last_nunchuk_acc_roll = 0x00;
    break;
  case WiimoteGamepadDevice::WiimoteAccelerometer:
    m_last_wiimote_acc_pitch = 0x00;
    m_last_wiimote_acc_roll = 0x00;
    break;
  }
}

void WiimoteGamepadDevice::syncAxes() {
  switch (m_horizontal) {
  case WiimoteGamepadDevice::GamepadHorizontal:
    sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_y);
    sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_x * -0x01);
    break;
  case WiimoteGamepadDevice::GamepadVertical:
    sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_x);
    sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_y);
    break;
  }

  sendEvent(EV_ABS, WIIMOTE_PITCH_LINUX_AXIS, m_last_wiimote_acc_pitch);
  sendEvent(EV_ABS, WIIMOTE_ROLL_LINUX_AXIS, m_last_wiimote_acc_roll);
  sendEvent(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_X, m_last_stick_x);
  sendEvent(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_Y, m_last_stick_y);
  sendEvent(EV_ABS, NUNCHUK_PITCH_LINUX_AXIS, m_last_nunchuk_acc_pitch);
  sendEvent(EV_ABS, NUNCHUK_ROLL_LINUX_AXIS, m_last_nunchuk_acc_roll);
  sendEventSync();
}

void WiimoteGamepadDevice::setWiimoteAcc(double pitch, double roll) {
  m_last_wiimote_acc_pitch = pitch;
  m_last_wiimote_acc_roll = roll;
  syncAxes();
}

void WiimoteGamepadDevice::setNunchukStick(qint32 x, qint32 y) {
  m_last_stick_x = x;
  m_last_stick_y = y;
  syncAxes();
}

void WiimoteGamepadDevice::setNunchukAcc(double pitch, double roll) {
  m_last_nunchuk_acc_pitch = pitch;
  m_last_nunchuk_acc_roll = roll;
  syncAxes();
}
