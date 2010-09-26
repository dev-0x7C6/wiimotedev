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

#include <QFile>
#include "devices/keyboard.h"

UInputObject::UInputObject(QObject *parent): QObject(parent) {
    uinputFile = QString::fromUtf8("");
    uinput_fd = 0;
    alreadyOpened = false;

    for (register int i = 0; i < UInputLocation.count(); ++i) if (QFile::exists(UInputLocation.at(i))) {
        uinputFile = UInputLocation.at(i);
        break;
    }
}

void UInputObject::sendEvent(quint16 type, quint16 code, qint32 value) {
    if (!uinput_fd)
        return;

    struct input_event event;
    memset(&event.time, 0, sizeof(event.time));
    event.code = code;
    event.type = type;
    event.value = value;
    write(uinput_fd, &event, sizeof(event));
}

bool UInputObject::uinput_open() {
    if (alreadyOpened) uinput_close();
    if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
        qWarning("%s: Unable to open %s", staticInterfaceName(), uinputFile.toAscii().constData());
        return false;
    }

    memset(&dev, 0, sizeof(dev));
    strncpy(dev.name, defaults::UInputObject::deviceSignature.toAscii().constData(),
                      defaults::UInputObject::deviceSignature.length());

    dev.id.product = UINPUT_PRODUCT_ID;
    dev.id.version = UINPUT_VERSION_ID;
    dev.id.vendor = UINPUT_VENDOR_ID;
    dev.id.bustype = UINPUT_BUSTYPE_ID;

/* Register events ---------------------------------------------- */
    linux_register_evbit(EV_KEY);
    linux_register_evbit(EV_REP);
    linux_register_evbit(EV_REL);

/* Keyboard events ---------------------------------------------- */
    for (register quint16 i = 0; i < BTN_MISC; ++i)
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
        qWarning("%s: Unable to create virtual input device", staticInterfaceName());
        uinput_close();
        return false;
    }
    return (alreadyOpened = true);
}

bool UInputClassicGamepad::uinput_open() {
    if (alreadyOpened) uinput_close();
    if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
        qWarning("%s: Unable to open %s", staticInterfaceName(), uinputFile.toAscii().constData());
        return false;
    }

    memset(&dev, 0, sizeof(dev));

    strncpy(dev.name, defaults::UInputClassicGamepad::deviceSignature.toAscii().constData(),
                      defaults::UInputClassicGamepad::deviceSignature.length());

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
    linux_abs_set_range(ABS_X, defaults::UInputClassicGamepad::lstick_max,
                               defaults::UInputClassicGamepad::lstick_min);
    linux_abs_set_range(ABS_Y, defaults::UInputClassicGamepad::lstick_max,
                               defaults::UInputClassicGamepad::lstick_min);

    linux_register_absbit(ABS_RX);
    linux_register_absbit(ABS_RY);
    linux_abs_set_range(ABS_RX, defaults::UInputClassicGamepad::rstick_max,
                                defaults::UInputClassicGamepad::rstick_min);
    linux_abs_set_range(ABS_RY, defaults::UInputClassicGamepad::rstick_max,
                                defaults::UInputClassicGamepad::rstick_min);

    linux_register_absbit(ABS_HAT0X);
    linux_register_absbit(ABS_HAT0Y);
    linux_abs_set_range(ABS_HAT0X, defaults::UInputClassicGamepad::dpad_max,
                                   defaults::UInputClassicGamepad::dpad_min);
    linux_abs_set_range(ABS_HAT0Y, defaults::UInputClassicGamepad::dpad_max,
                                   defaults::UInputClassicGamepad::dpad_min);

    write(uinput_fd, &dev, sizeof(dev));
    if (ioctl(uinput_fd, UI_DEV_CREATE)) {
        qWarning("%s: Unable to create virtual input device", staticInterfaceName());
        uinput_close();
        return false;
    }
    return (alreadyOpened = true);
}

bool UInputWiimoteGamepad::uinput_open() {
    if (alreadyOpened) uinput_close();
    if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
        qWarning("%s: Unable to open %s", staticInterfaceName(), uinputFile.toAscii().constData());
        return false;
    }

    memset(&dev, 0, sizeof(dev));

    strncpy(dev.name, defaults::UInputWiimoteGamepad::deviceSignature.toAscii().constData(),
                      defaults::UInputWiimoteGamepad::deviceSignature.length());

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
    linux_register_keybit(BTN_X); // wiiremote 1
    linux_register_keybit(BTN_Y); // wiiremote 2
    linux_register_keybit(BTN_SELECT);
    linux_register_keybit(BTN_MODE);
    linux_register_keybit(BTN_START);

    linux_register_keybit(BTN_C);
    linux_register_keybit(BTN_Z);

    linux_register_absbit(ABS_X);
    linux_register_absbit(ABS_Y);
    linux_abs_set_range(ABS_X, defaults::UInputWiimoteGamepad::stick_max,
                               defaults::UInputWiimoteGamepad::stick_min);
    linux_abs_set_range(ABS_Y, defaults::UInputWiimoteGamepad::stick_max,
                               defaults::UInputWiimoteGamepad::stick_min);

    linux_register_absbit(ABS_RX);
    linux_register_absbit(ABS_RY);
    linux_abs_set_range(ABS_RX, defaults::UInputWiimoteGamepad::dpad_max,
                                defaults::UInputWiimoteGamepad::dpad_min);
    linux_abs_set_range(ABS_RY, defaults::UInputWiimoteGamepad::dpad_max,
                                defaults::UInputWiimoteGamepad::dpad_min);

    linux_register_absbit(ABS_HAT0X);
    linux_register_absbit(ABS_HAT0Y);
    linux_abs_set_range(ABS_HAT0X, defaults::UInputWiimoteGamepad::pitch_max,
                                   defaults::UInputWiimoteGamepad::pitch_min);
    linux_abs_set_range(ABS_HAT0Y, defaults::UInputWiimoteGamepad::roll_max,
                                   defaults::UInputWiimoteGamepad::roll_min);

    linux_register_absbit(ABS_HAT1X);
    linux_register_absbit(ABS_HAT1Y);
    linux_abs_set_range(ABS_HAT1X, defaults::UInputWiimoteGamepad::pitch_max,
                                   defaults::UInputWiimoteGamepad::pitch_min);
    linux_abs_set_range(ABS_HAT1Y, defaults::UInputWiimoteGamepad::roll_max,
                                   defaults::UInputWiimoteGamepad::roll_min);

    write(uinput_fd, &dev, sizeof(dev));
    if (ioctl(uinput_fd, UI_DEV_CREATE)) {
        qWarning("%s: Unable to create virtual input device", staticInterfaceName());
        uinput_close();
        return false;
    }
    return (alreadyOpened = true);
}

bool UInputEvent::uinput_open(bool replay){
    if (alreadyOpened) uinput_close();
    if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
        qWarning("%s: Unable to open %s", staticInterfaceName(), uinputFile.toAscii().constData());
        return false;
    }

    memset(&dev, 0, sizeof(dev));
    strncpy(dev.name, defaults::UInputEvent::deviceSignature.toAscii().constData(),
                      defaults::UInputEvent::deviceSignature.length());

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
        qWarning("%s: Unable to create virtual input device", staticInterfaceName());
        uinput_close();
        return false;
    }
    return (alreadyOpened = true);
}

bool UInputMouse::uinput_open(QRect absRect, bool abs){
    if (alreadyOpened) uinput_close();
    if (!(uinput_fd = open(uinputFile.toAscii().constData(), O_WRONLY | O_NDELAY))) {
        qWarning("%s: Unable to open %s", staticInterfaceName(), uinputFile.toAscii().constData());
        return false;
    }

    memset(&dev, 0, sizeof(dev));
    strncpy(dev.name, defaults::UInputObject::deviceSignature.toAscii().constData(),
                      defaults::UInputObject::deviceSignature.length());

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
        qWarning("%s: Unable to create virtual input device", staticInterfaceName());
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

void UInputEvent::moveMousePointerRel(qint32 x, qint32 y)
{
    if (x) sendEvent(EV_REL, REL_X, x);
    if (y) sendEvent(EV_REL, REL_Y, y);
    sendEventSync();
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


void UInputClassicGamepad::classicButtons(quint32 id, quint64 buttons){
    if (id != 1) return;

    sendEvent(EV_KEY, BTN_A, (buttons & CLASSIC_BTN_A) ? 1 : 0);
    sendEvent(EV_KEY, BTN_B, (buttons & CLASSIC_BTN_B) ? 1 : 0);
    sendEvent(EV_KEY, BTN_X, (buttons & CLASSIC_BTN_X) ? 1 : 0);
    sendEvent(EV_KEY, BTN_Y, (buttons & CLASSIC_BTN_Y) ? 1 : 0);
    sendEvent(EV_KEY, BTN_TL, (buttons & CLASSIC_BTN_L) ? 1 : 0);
    sendEvent(EV_KEY, BTN_TR, (buttons & CLASSIC_BTN_R) ? 1 : 0);
    sendEvent(EV_KEY, BTN_TL2, (buttons & CLASSIC_BTN_ZL) ? 1 : 0);
    sendEvent(EV_KEY, BTN_TR2, (buttons & CLASSIC_BTN_ZR) ? 1 : 0);
    sendEvent(EV_KEY, BTN_SELECT, (buttons & CLASSIC_BTN_MINUS) ? 1 : 0);
    sendEvent(EV_KEY, BTN_START, (buttons & CLASSIC_BTN_PLUS) ? 1 : 0);
    sendEvent(EV_KEY, BTN_0, (buttons & CLASSIC_BTN_RIGHT) ? 1 : 0);
    sendEvent(EV_KEY, BTN_1, (buttons & CLASSIC_BTN_LEFT) ? 1 : 0);
    sendEvent(EV_KEY, BTN_2, (buttons & CLASSIC_BTN_DOWN) ? 1 : 0);
    sendEvent(EV_KEY, BTN_3, (buttons & CLASSIC_BTN_UP) ? 1 : 0);

    register qint8 ix = 0;
    register qint8 iy = 0;

    if (buttons & CLASSIC_BTN_RIGHT) ix = 1; else
    if (buttons & CLASSIC_BTN_LEFT) ix = -1;
    if (buttons & CLASSIC_BTN_DOWN) iy = 1; else
    if (buttons & CLASSIC_BTN_UP) iy = -1;

    sendEvent(EV_ABS, ABS_HAT0X, ix);
    sendEvent(EV_ABS, ABS_HAT0X, iy);

    sendEventSync();
}
void UInputClassicGamepad::classicLStick(quint32 id, quint16 x, quint16 y)
{
    if (id != 1) return;

    if (x) sendEvent(EV_ABS, ABS_X, x);
    if (y) sendEvent(EV_ABS, ABS_Y, y);
    sendEventSync();
}

void UInputClassicGamepad::classicRStick(quint32 id, quint16 x, quint16 y)
{
    if (id != 1) return;

    if (x) sendEvent(EV_ABS, ABS_RX, x);
    if (y) sendEvent(EV_ABS, ABS_RY, y);
    sendEventSync();
}

void UInputWiimoteGamepad::wiimoteButtons(quint32 id, quint64 buttons) {
    if (id != 1) return;

    sendEvent(EV_KEY, BTN_A, (buttons & WIIMOTE_BTN_A) ? 1 : 0);
    sendEvent(EV_KEY, BTN_B, (buttons & WIIMOTE_BTN_B) ? 1 : 0);
    sendEvent(EV_KEY, BTN_X, (buttons & WIIMOTE_BTN_1) ? 1 : 0);
    sendEvent(EV_KEY, BTN_Y, (buttons & WIIMOTE_BTN_2) ? 1 : 0);
    sendEvent(EV_KEY, BTN_START, (buttons & WIIMOTE_BTN_PLUS) ? 1 : 0);
    sendEvent(EV_KEY, BTN_MODE, (buttons & WIIMOTE_BTN_HOME) ? 1 : 0);
    sendEvent(EV_KEY, BTN_SELECT, (buttons & WIIMOTE_BTN_MINUS) ? 1 : 0);

    sendEvent(EV_KEY, BTN_0, (buttons & WIIMOTE_BTN_RIGHT) ? 1 : 0);
    sendEvent(EV_KEY, BTN_1, (buttons & WIIMOTE_BTN_LEFT) ? 1 : 0);
    sendEvent(EV_KEY, BTN_2, (buttons & WIIMOTE_BTN_DOWN) ? 1 : 0);
    sendEvent(EV_KEY, BTN_3, (buttons & WIIMOTE_BTN_UP) ? 1 : 0);

    register qint8 ix = 0;
    register qint8 iy = 0;

    if (buttons & WIIMOTE_BTN_RIGHT) ix = 1; else
    if (buttons & WIIMOTE_BTN_LEFT) ix = -1;
    if (buttons & WIIMOTE_BTN_DOWN) iy = 1; else
    if (buttons & WIIMOTE_BTN_UP) iy = -1;

    sendEvent(EV_ABS, ABS_RX, ix);
    sendEvent(EV_ABS, ABS_RX, iy);
    sendEventSync();
}

void UInputWiimoteGamepad::wiimoteAcc(quint32 id, struct accdata acc)
{
    if (id != 1) return;

    register qint16 ix = (acc.pitch * 100);
    register qint16 iy = (acc.roll * 100);

    sendEvent(EV_ABS, ABS_HAT0X, ix);
    sendEvent(EV_ABS, ABS_HAT0Y, iy);
    sendEventSync();
}

void UInputWiimoteGamepad::nunchukButtons(quint32 id , quint64 buttons){
    if (id != 1) return;

    sendEvent(EV_KEY, BTN_C, (buttons & NUNCHUK_BTN_C) ? 1 : 0);
    sendEvent(EV_KEY, BTN_Z, (buttons & NUNCHUK_BTN_Z) ? 1 : 0);

    sendEventSync();
}

void UInputWiimoteGamepad::nunchukStick(quint32 id, qint16 x, qint16 y){
    if (id != 1) return;

    sendEvent(EV_ABS, ABS_X, x);
    sendEvent(EV_ABS, ABS_Y, y);
    sendEventSync();
}

void UInputWiimoteGamepad::nunchukAcc(quint32 id, struct accdata acc){
    if (id != 1) return;

    register qint16 ix = (acc.pitch * 100);
    register qint16 iy = (acc.roll * 100);

    sendEvent(EV_ABS, ABS_HAT1X, ix);
    sendEvent(EV_ABS, ABS_HAT1Y, iy);
    sendEventSync();
}
