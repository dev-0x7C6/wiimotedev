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

#include "eiowiimotejoystick.h"

EIO_WiimoteJoystick::EIO_WiimoteJoystick(QString deviceName, int id, Mode mode, Position horizontal, QObject *parent) :
  QObject(parent),
  EIO_UInputObject(),
  m_deviceName(deviceName),
  m_horizontal(horizontal),
  m_mode(mode),
  m_home_pressed(false),
  m_id(id),
  m_dpad_invert_x(0x00),
  m_dpad_invert_y(0x00),
  m_home_switch_position(0x01),
  m_report_buttons(0x01),
  m_report_dstick(0x01),
  m_report_pitch(0x01),
  m_report_roll(0x01) {
  if (m_deviceName.isEmpty())
    m_deviceName = QString::fromUtf8("Wiimote Gamepad Device (undefined)");
}

int EIO_WiimoteJoystick::assign() {
  return m_id;
}

void EIO_WiimoteJoystick::setDStickInvertX(bool option) {
  m_dpad_invert_x = option;
}

void EIO_WiimoteJoystick::setDStickInvertY(bool option) {
  m_dpad_invert_y = option;
}

void EIO_WiimoteJoystick::setHomeSwitchPosition(bool option) {
  m_home_switch_position = option;
}

void EIO_WiimoteJoystick::setReportButtons(bool report) {
  m_report_buttons = report;
}

void EIO_WiimoteJoystick::setReportDStick(bool report) {
  m_report_dstick = report;
}

void EIO_WiimoteJoystick::setReportPitch(bool report) {
  m_report_pitch = report;
}

void EIO_WiimoteJoystick::setReportRoll(bool report) {
  m_report_roll = report;
}

bool EIO_WiimoteJoystick::create() {
  switch (m_horizontal) {
    case EIO_WiimoteJoystick::GamepadHorizontal:
      emit setLedState(m_id, 1 + 2);
      break;

    case EIO_WiimoteJoystick::GamepadVertical:
      emit setLedState(m_id, 4 + 8);
      break;
  }

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

  if (m_report_buttons) {
    linux_register_keybit(BTN_0);
    linux_register_keybit(BTN_1);
    linux_register_keybit(BTN_2);
    linux_register_keybit(BTN_3);
    linux_register_keybit(BTN_GAMEPAD);
    linux_register_keybit(BTN_B);
    linux_register_keybit(BTN_X);
    linux_register_keybit(BTN_Y);
    linux_register_keybit(BTN_SELECT);
    linux_register_keybit(BTN_START);

    if (!m_home_switch_position)
      linux_register_keybit(BTN_MODE);
  }

  if (m_report_dstick) {
    linux_register_absbit(WIIMOTE_DPAD_LINUX_AXIS_X);
    linux_register_absbit(WIIMOTE_DPAD_LINUX_AXIS_Y);
    linux_abs_set_range(WIIMOTE_DPAD_LINUX_AXIS_X, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
    linux_abs_set_range(WIIMOTE_DPAD_LINUX_AXIS_Y, WIIMOTE_DPAD_MAX, WIIMOTE_DPAD_MIN);
  }

  if (m_report_pitch) {
    linux_register_absbit(WIIMOTE_PITCH_LINUX_AXIS);
    linux_abs_set_range(WIIMOTE_PITCH_LINUX_AXIS, WIIMOTE_PITCH_MAX, WIIMOTE_PITCH_MIN);
  }

  if (m_report_roll) {
    linux_register_absbit(WIIMOTE_ROLL_LINUX_AXIS);
    linux_abs_set_range(WIIMOTE_ROLL_LINUX_AXIS, WIIMOTE_ROLL_MAX, WIIMOTE_ROLL_MIN);
  }

  centerStick(EIO_WiimoteJoystick::DpadStick);
  centerStick(EIO_WiimoteJoystick::WiimoteAccelerometer);
  centerStick(EIO_WiimoteJoystick::NunchukAccelerometer);
  write(uinput_fd, &dev, sizeof(dev));

  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    qWarning("%s: Unable to create virtual input device", m_deviceName.toAscii().constData());
    uinput_close();
    return false;
  }

  return (alreadyOpened = true);
}

void EIO_WiimoteJoystick::setWiimoteButtons(uint64 buttons) {
  if (m_report_buttons) {
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

    if (!m_home_switch_position)
      sendEvent(EV_KEY, BTN_MODE, (buttons & WIIMOTE_BTN_HOME) ? WIIMOTE_BUTTON_PUSHED : WIIMOTE_BUTTON_RELEASED);

    sendEventSync();
  }

  if (m_home_switch_position && m_report_buttons) {
    if (((buttons & WIIMOTE_BTN_HOME) != WIIMOTE_BTN_HOME) && (m_home_pressed == true))
      emit setLedState(m_id, m_id);

    if (((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME) && (m_home_pressed == false))
      switch (m_horizontal) {
        case EIO_WiimoteJoystick::GamepadHorizontal:
          m_horizontal = EIO_WiimoteJoystick::GamepadVertical;
          emit setLedState(m_id, 1 + 2);
          break;

        case EIO_WiimoteJoystick::GamepadVertical:
          m_horizontal = EIO_WiimoteJoystick::GamepadHorizontal;
          emit setLedState(m_id, 4 + 8);
          break;
      }
  }

  m_home_pressed = ((buttons & WIIMOTE_BTN_HOME) == WIIMOTE_BTN_HOME);
  m_last_dpad_x = 0x00;
  m_last_dpad_y = 0x00;

  if (m_report_dstick) {
    if (buttons & WIIMOTE_BTN_RIGHT) m_last_dpad_x = WIIMOTE_DPAD_MAX;
    else if (buttons & WIIMOTE_BTN_LEFT) m_last_dpad_x = WIIMOTE_DPAD_MIN;

    if (buttons & WIIMOTE_BTN_DOWN) m_last_dpad_y = WIIMOTE_DPAD_MAX;
    else if (buttons & WIIMOTE_BTN_UP) m_last_dpad_y = WIIMOTE_DPAD_MIN;

    if (m_dpad_invert_x) m_last_dpad_x *= -1;

    if (m_dpad_invert_y) m_last_dpad_y *= -1;
  }

  syncAxes();
}

void EIO_WiimoteJoystick::centerStick(Stick id) {
  switch (id) {
    case EIO_WiimoteJoystick::DpadStick:
      m_last_dpad_x = 0x00;
      m_last_dpad_y = 0x00;
      break;

    case EIO_WiimoteJoystick::NunchukAccelerometer:
      m_last_nunchuk_acc_pitch = 0x00;
      m_last_nunchuk_acc_roll = 0x00;
      break;

    case EIO_WiimoteJoystick::WiimoteAccelerometer:
      m_last_wiimote_acc_pitch = 0x00;
      m_last_wiimote_acc_roll = 0x00;
      break;

    default:
      break;
  }
}

void EIO_WiimoteJoystick::syncAxes() {
  if (m_report_dstick) {
    switch (m_horizontal) {
      case EIO_WiimoteJoystick::GamepadHorizontal:
        sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_y);
        sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_x * -0x01);
        break;

      case EIO_WiimoteJoystick::GamepadVertical:
        sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_X, m_last_dpad_x);
        sendEvent(EV_ABS, WIIMOTE_DPAD_LINUX_AXIS_Y, m_last_dpad_y);
        break;
    }
  }

  if (m_report_pitch)
    sendEvent(EV_ABS, WIIMOTE_PITCH_LINUX_AXIS, m_last_wiimote_acc_pitch);

  if (m_report_roll)
    sendEvent(EV_ABS, WIIMOTE_ROLL_LINUX_AXIS, m_last_wiimote_acc_roll);

  sendEventSync();
}

void EIO_WiimoteJoystick::setWiimoteAcc(double pitch, double roll) {
  m_last_wiimote_acc_pitch = pitch;
  m_last_wiimote_acc_roll = roll;
  syncAxes();
}
