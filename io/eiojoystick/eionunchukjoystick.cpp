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

#include "eionunchukjoystick.h"

EIO_NunchukJoystick::EIO_NunchukJoystick(QString deviceName, int id, QObject *parent) :
  QObject(parent),
  EIO_UInputObject(),
  m_deviceName(deviceName),
  m_id(id),
  m_stick_invert_x(0x00),
  m_stick_invert_y(0x00),
  m_report_buttons(0x01),
  m_report_stick(0x01),
  m_report_pitch(0x01),
  m_report_roll(0x01) {
  if (m_deviceName.isEmpty())
    m_deviceName = QString::fromUtf8("Wiimote Gamepad Device (undefined)");
}

quint32 EIO_NunchukJoystick::assign() {
  return m_id;
}

void EIO_NunchukJoystick::setStickInvertX(bool option) {
  m_stick_invert_x = option;
}

void EIO_NunchukJoystick::setStickInvertY(bool option) {
  m_stick_invert_y = option;
}

void EIO_NunchukJoystick::setReportButtons(bool report) {
  m_report_buttons = report;
}

void EIO_NunchukJoystick::setReportStick(bool report) {
  m_report_stick = report;
}

void EIO_NunchukJoystick::setReportPitch(bool report) {
  m_report_pitch = report;
}

void EIO_NunchukJoystick::setReportRoll(bool report) {
  m_report_roll = report;
}


bool EIO_NunchukJoystick::create() {
  if (alreadyOpened)
    uinput_close();

  if (!(uinput_fd = open(uinputFile.toLocal8Bit().constData(), O_WRONLY | O_NDELAY))) {
    qWarning("%s: Unable to open %s", m_deviceName.toLocal8Bit().constData(), uinputFile.toLocal8Bit().constData());
    return false;
  }

  memset(&dev, 0, sizeof(dev));
  strncpy(dev.name, m_deviceName.toLocal8Bit().constData(), m_deviceName.length());
  dev.id.product = UINPUT_PRODUCT_ID;
  dev.id.version = UINPUT_VERSION_ID;
  dev.id.vendor = UINPUT_VENDOR_ID;
  dev.id.bustype = UINPUT_BUSTYPE_ID;
  linux_register_evbit(EV_KEY);
  linux_register_evbit(EV_MSC);
  linux_register_evbit(EV_ABS);

  if (m_report_buttons) {
    linux_register_keybit(BTN_GAMEPAD);
    linux_register_keybit(BTN_B);
  }

  if (m_report_stick) {
    linux_register_absbit(NUNCHUK_STICK_LINUX_AXIS_X);
    linux_register_absbit(NUNCHUK_STICK_LINUX_AXIS_Y);
    linux_abs_set_range(NUNCHUK_STICK_LINUX_AXIS_X, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
    linux_abs_set_range(NUNCHUK_STICK_LINUX_AXIS_Y, NUNCHUK_STICK_MAX, NUNCHUK_STICK_MIN);
  }

  if (m_report_pitch) {
    linux_register_absbit(NUNCHUK_PITCH_LINUX_AXIS);
    linux_abs_set_range(NUNCHUK_PITCH_LINUX_AXIS, NUNCHUK_PITCH_MAX, NUNCHUK_PITCH_MIN);
  }

  if (m_report_roll) {
    linux_register_absbit(NUNCHUK_ROLL_LINUX_AXIS);
    linux_abs_set_range(NUNCHUK_ROLL_LINUX_AXIS, NUNCHUK_ROLL_MAX, NUNCHUK_ROLL_MIN);
  }

  centerStick(EIO_NunchukJoystick::NunchukStick);
  write(uinput_fd, &dev, sizeof(dev));

  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    qWarning("%s: Unable to create virtual input device", m_deviceName.toLocal8Bit().constData());
    uinput_close();
    return false;
  }

  return (alreadyOpened = true);
}

void EIO_NunchukJoystick::setNunchukButtons(uint64 buttons) {
  if (!m_report_buttons)
    return;

  sendEvent(EV_KEY, BTN_A, (buttons & NUNCHUK_BTN_C) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
  sendEvent(EV_KEY, BTN_B, (buttons & NUNCHUK_BTN_Z) ? NUNCHUK_BUTTON_PUSHED : NUNCHUK_BUTTON_RELEASED);
  sendEventSync();
}

void EIO_NunchukJoystick::centerStick(Stick id) {
  switch (id) {
    case EIO_NunchukJoystick::NunchukStick:
      m_last_stick_x = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
      m_last_stick_y = (NUNCHUK_STICK_MAX - NUNCHUK_STICK_MIN) / 2;
      break;

    case EIO_NunchukJoystick::DpadStick:
      m_last_dpad_x = 0x00;
      m_last_dpad_y = 0x00;
      break;

    case EIO_NunchukJoystick::NunchukAccelerometer:
      m_last_nunchuk_acc_pitch = 0x00;
      m_last_nunchuk_acc_roll = 0x00;
      break;

    case EIO_NunchukJoystick::WiimoteAccelerometer:
      m_last_wiimote_acc_pitch = 0x00;
      m_last_wiimote_acc_roll = 0x00;
      break;
  }
}

void EIO_NunchukJoystick::syncAxes() {
  if (m_report_stick) {
    sendEvent(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_X, m_last_stick_x);
    sendEvent(EV_ABS, NUNCHUK_STICK_LINUX_AXIS_Y, m_last_stick_y);
  }

  if (m_report_pitch)
    sendEvent(EV_ABS, NUNCHUK_PITCH_LINUX_AXIS, m_last_nunchuk_acc_pitch);

  if (m_report_roll)
    sendEvent(EV_ABS, NUNCHUK_ROLL_LINUX_AXIS, m_last_nunchuk_acc_roll);

  sendEventSync();
}

void EIO_NunchukJoystick::setNunchukStick(int32 x, int32 y) {
  m_last_stick_x = x;
  m_last_stick_y = y;
  syncAxes();
}

void EIO_NunchukJoystick::setNunchukAcc(double pitch, double roll) {
  m_last_nunchuk_acc_pitch = pitch;
  m_last_nunchuk_acc_roll = roll;
  syncAxes();
}
