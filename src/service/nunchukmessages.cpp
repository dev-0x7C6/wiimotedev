#include "wiimotemessagethread.h"

void WiimoteMessageThread::cwiid_process_nunchuk_init() {
  cwiid_process_nunchuk_clear();
}

void WiimoteMessageThread::cwiid_process_nunchuk_done() {
  cwiid_process_nunchuk_clear();
}

void WiimoteMessageThread::cwiid_process_nunchuk_clear() {
  cstate[ix_nunchuk_device] = 0x00;
  lstate[ix_nunchuk_device] = 0x00;
  stick[ix_nunchuk_stick].x = 0x00;
  stick[ix_nunchuk_stick].y = 0x00;
  available[ix_nunchuk_device] = false;
}

void WiimoteMessageThread::cwiid_process_nunchuk_buttons(quint8 cwiid_buttons) {
  cstate[ix_nunchuk_device] &= NUNCHUK_BUTTON_NOTMASK;
  if (cwiid_buttons & CWIID_NUNCHUK_BTN_C) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_C;
  if (cwiid_buttons & CWIID_NUNCHUK_BTN_Z) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_Z;
}

void WiimoteMessageThread::cwiid_process_nunchuk_stick(quint8 cwiid_stick[2]) {
  if (stick[ix_nunchuk_stick].x != cwiid_stick[ix_x_axis] ||
      stick[ix_nunchuk_stick].y != cwiid_stick[ix_y_axis]) {
    stick[ix_nunchuk_stick].x = cwiid_stick[ix_x_axis];
    stick[ix_nunchuk_stick].y = cwiid_stick[ix_y_axis];
    emit dbusNunchukStick(m_id, stick[ix_nunchuk_stick]);
  }

  if (stick[ix_nunchuk_stick].x > nunchukStickMaxX) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_RIGHT; else
  if (stick[ix_nunchuk_stick].x < nunchukStickMinX) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_LEFT;
  if (stick[ix_nunchuk_stick].y > nunchukStickMaxY) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_STICK_UP; else
  if (stick[ix_nunchuk_stick].y < nunchukStickMinY) cstate[ix_nunchuk_device]|= NUNCHUK_BTN_STICK_DOWN;
}

void WiimoteMessageThread::cwiid_process_nunchuk_acc(quint8 cwiid_acc[3]) {
  calcAccelerometerValues(cwiid_acc, calibration[ix_nunchuk_device], acc[ix_nunchuk_device]);
  emit dbusNunchukAcc(m_id, acc[ix_nunchuk_device]);

  cstate[ix_nunchuk_device] &= NUNCHUK_TILT_NOTMASK;
  if (acc[ix_wiimote_device].pitch < -45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_FRONT; else
  if (acc[ix_wiimote_device].pitch >  45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_BACK;
  if (acc[ix_wiimote_device].roll >  45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_RIGHT; else
  if (acc[ix_wiimote_device].roll < -45.0) cstate[ix_nunchuk_device] |= NUNCHUK_BTN_TILT_LEFT;
}

void WiimoteMessageThread::cwiid_process_nunchuk_status(cwiid_ext_type type) {
  switch (type) {
  case CWIID_EXT_NONE:
    if (available[ix_nunchuk_device]) {
      cwiid_process_nunchuk_clear();
      emit dbusNunchukPlugged(m_id);
    }
    break;

  case CWIID_EXT_NUNCHUK:
    if (!available[ix_nunchuk_device]) {
      available[ix_nunchuk_device] = true;
      emit dbusNunchukPlugged(m_id);
    }
    break;
  }
}

