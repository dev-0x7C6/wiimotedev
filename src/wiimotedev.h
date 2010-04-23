/**********************************************************************************
 * Wiimotedev daemon, wiiremote system service                                    *
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

#ifndef WIIMOTEDEV_H
#define WIIMOTEDEV_H

#include <iostream>

#define WIIMOTEDEV_CONFIG_FILE "/etc/wiimotedev/wiimotedev.conf"
#define WIIMOTEDEV_DBUS_SERVICE_OBJECT_NAME "/service"
#define WIIMOTEDEV_DBUS_DEVICE_EVENTS_OBJECT_NAME "/deviceEvents"
#define WIIMOTEDEV_DBUS_SERVICE_NAME "org.wiimotedev.daemon"

/* Compatibility ------------------------------------------------- */

#define WIIMOTEDEV_DBUS_OBJECT_NAME WIIMOTEDEV_DBUS_DEVICE_EVENTS_OBJECT_NAME

//

/* Typedefs ------------------------------------------------------ */

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef signed long long int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;

/* Wiiremote status  --------------------------------------------- */

const uint8 STATUS_WIIMOTE_DISCONNECTED = 0x00;
const uint8 STATUS_WIIMOTE_CONNECTED = 0x01;
const uint8 STATUS_WIIMOTE_NUNCHUK_CONNECTED = 0x02;
const uint8 STATUS_WIIMOTE_CLASSIC_CONNECTED = 0x04;

#define STATUS_NUNCHUK_CONNECTED STATUS_WIIMOTE_NUNCHUK_CONNECTED
#define STATUS_CLASSIC_CONNECTED STATUS_WIIMOTE_CLASSIC_CONNECTED

/* Structs  --------------------------------------------- */

struct deviceinfo
{
    uint32 id;
    std::string addr;
    bool registred;
    bool nunchuk;
    bool classic;
};

struct irpoint
{
    int16 size;
    uint16 x;
    uint16 y;
};

struct accdata
{
    uint8 x;
    uint8 y;
    uint8 z;
    double pitch;
    double roll;
};

struct stickdata
{
    uint8 x;
    uint8 y;
};

enum GENERAL_BUTTONS
{
    GENERAL_WIIMOTE_BTN_1 = 0,
    GENERAL_WIIMOTE_BTN_2,
    GENERAL_WIIMOTE_BTN_A,
    GENERAL_WIIMOTE_BTN_B,
    GENERAL_WIIMOTE_BTN_MINUS,
    GENERAL_WIIMOTE_BTN_PLUS,
    GENERAL_WIIMOTE_BTN_HOME,
    GENERAL_WIIMOTE_BTN_RIGHT,
    GENERAL_WIIMOTE_BTN_LEFT,
    GENERAL_WIIMOTE_BTN_DOWN,
    GENERAL_WIIMOTE_BTN_UP,
    GENERAL_WIIMOTE_BTN_SHIFT_BACKWARD,
    GENERAL_WIIMOTE_BTN_SHIFT_FORWARD,
    GENERAL_WIIMOTE_BTN_SHIFT_RIGHT,
    GENERAL_WIIMOTE_BTN_SHIFT_LEFT,
    GENERAL_WIIMOTE_BTN_SHIFT_DOWN,
    GENERAL_WIIMOTE_BTN_SHIFT_UP,
    GENERAL_WIIMOTE_BTN_TILT_FRONT,
    GENERAL_WIIMOTE_BTN_TILT_BACK,
    GENERAL_WIIMOTE_BTN_TILT_RIGHT,
    GENERAL_WIIMOTE_BTN_TILT_LEFT,
    GENERAL_NUNCHUK_BTN_C,
    GENERAL_NUNCHUK_BTN_Z,
    GENERAL_NUNCHUK_BTN_STICK_RIGHT,
    GENERAL_NUNCHUK_BTN_STICK_LEFT,
    GENERAL_NUNCHUK_BTN_STICK_DOWN,
    GENERAL_NUNCHUK_BTN_STICK_UP,
    GENERAL_NUNCHUK_BTN_SHIFT_BACKWARD,
    GENERAL_NUNCHUK_BTN_SHIFT_FORWARD,
    GENERAL_NUNCHUK_BTN_SHIFT_RIGHT,
    GENERAL_NUNCHUK_BTN_SHIFT_LEFT,
    GENERAL_NUNCHUK_BTN_SHIFT_DOWN,
    GENERAL_NUNCHUK_BTN_SHIFT_UP,
    GENERAL_NUNCHUK_BTN_TILT_FRONT,
    GENERAL_NUNCHUK_BTN_TILT_BACK,
    GENERAL_NUNCHUK_BTN_TILT_RIGHT,
    GENERAL_NUNCHUK_BTN_TILT_LEFT,
    GENERAL_CLASSIC_BTN_X,
    GENERAL_CLASSIC_BTN_Y,
    GENERAL_CLASSIC_BTN_A,
    GENERAL_CLASSIC_BTN_B,
    GENERAL_CLASSIC_BTN_L,
    GENERAL_CLASSIC_BTN_R,
    GENERAL_CLASSIC_BTN_ZL,
    GENERAL_CLASSIC_BTN_ZR,
    GENERAL_CLASSIC_BTN_MINUS,
    GENERAL_CLASSIC_BTN_PLUS,
    GENERAL_CLASSIC_BTN_HOME,
    GENERAL_CLASSIC_BTN_RIGHT,
    GENERAL_CLASSIC_BTN_LEFT,
    GENERAL_CLASSIC_BTN_DOWN,
    GENERAL_CLASSIC_BTN_UP,
    GENERAL_CLASSIC_BTN_LSTICK_RIGHT,
    GENERAL_CLASSIC_BTN_LSTICK_LEFT,
    GENERAL_CLASSIC_BTN_LSTICK_DOWN,
    GENERAL_CLASSIC_BTN_LSTICK_UP,
    GENERAL_CLASSIC_BTN_RSTICK_RIGHT,
    GENERAL_CLASSIC_BTN_RSTICK_LEFT,
    GENERAL_CLASSIC_BTN_RSTICK_DOWN,
    GENERAL_CLASSIC_BTN_RSTICK_UP
};

#define calcbit(x) (static_cast<uint64>( true) << x)

const uint64 WIIMOTE_BTN_1 = calcbit(GENERAL_WIIMOTE_BTN_1);
const uint64 WIIMOTE_BTN_2 = calcbit(GENERAL_WIIMOTE_BTN_2);
const uint64 WIIMOTE_BTN_A = calcbit(GENERAL_WIIMOTE_BTN_A);
const uint64 WIIMOTE_BTN_B = calcbit(GENERAL_WIIMOTE_BTN_B);

const uint64 WIIMOTE_BTN_MINUS = calcbit(GENERAL_WIIMOTE_BTN_MINUS);
const uint64 WIIMOTE_BTN_PLUS = calcbit(GENERAL_WIIMOTE_BTN_PLUS);
const uint64 WIIMOTE_BTN_HOME = calcbit(GENERAL_WIIMOTE_BTN_HOME);

const uint64 WIIMOTE_BTN_RIGHT = calcbit(GENERAL_WIIMOTE_BTN_RIGHT);
const uint64 WIIMOTE_BTN_LEFT = calcbit(GENERAL_WIIMOTE_BTN_LEFT);
const uint64 WIIMOTE_BTN_DOWN = calcbit(GENERAL_WIIMOTE_BTN_DOWN);
const uint64 WIIMOTE_BTN_UP = calcbit(GENERAL_WIIMOTE_BTN_UP);

const uint64 WIIMOTE_BTN_SHIFT_BACKWARD = calcbit(GENERAL_WIIMOTE_BTN_SHIFT_BACKWARD);
const uint64 WIIMOTE_BTN_SHIFT_FORWARD = calcbit(GENERAL_WIIMOTE_BTN_SHIFT_FORWARD);
const uint64 WIIMOTE_BTN_SHIFT_RIGHT = calcbit(GENERAL_WIIMOTE_BTN_SHIFT_RIGHT);
const uint64 WIIMOTE_BTN_SHIFT_LEFT = calcbit(GENERAL_WIIMOTE_BTN_SHIFT_LEFT);
const uint64 WIIMOTE_BTN_SHIFT_DOWN = calcbit(GENERAL_WIIMOTE_BTN_SHIFT_DOWN);
const uint64 WIIMOTE_BTN_SHIFT_UP = calcbit(GENERAL_WIIMOTE_BTN_SHIFT_UP);

const uint64 WIIMOTE_BTN_TILT_FRONT = calcbit(GENERAL_WIIMOTE_BTN_TILT_FRONT);
const uint64 WIIMOTE_BTN_TILT_BACK = calcbit(GENERAL_WIIMOTE_BTN_TILT_BACK);
const uint64 WIIMOTE_BTN_TILT_RIGHT = calcbit(GENERAL_WIIMOTE_BTN_TILT_RIGHT);
const uint64 WIIMOTE_BTN_TILT_LEFT = calcbit(GENERAL_WIIMOTE_BTN_TILT_LEFT);

const uint64 NUNCHUK_BTN_C = calcbit(GENERAL_NUNCHUK_BTN_C);
const uint64 NUNCHUK_BTN_Z = calcbit(GENERAL_NUNCHUK_BTN_Z);

const uint64 NUNCHUK_BTN_STICK_RIGHT = calcbit(GENERAL_NUNCHUK_BTN_STICK_RIGHT);
const uint64 NUNCHUK_BTN_STICK_LEFT = calcbit(GENERAL_NUNCHUK_BTN_STICK_LEFT);
const uint64 NUNCHUK_BTN_STICK_DOWN = calcbit(GENERAL_NUNCHUK_BTN_STICK_DOWN);
const uint64 NUNCHUK_BTN_STICK_UP = calcbit(GENERAL_NUNCHUK_BTN_STICK_UP);

const uint64 NUNCHUK_BTN_SHIFT_BACKWARD = calcbit(GENERAL_NUNCHUK_BTN_SHIFT_BACKWARD);
const uint64 NUNCHUK_BTN_SHIFT_FORWARD = calcbit(GENERAL_NUNCHUK_BTN_SHIFT_FORWARD);
const uint64 NUNCHUK_BTN_SHIFT_RIGHT = calcbit(GENERAL_NUNCHUK_BTN_SHIFT_RIGHT);
const uint64 NUNCHUK_BTN_SHIFT_LEFT = calcbit(GENERAL_NUNCHUK_BTN_SHIFT_LEFT);
const uint64 NUNCHUK_BTN_SHIFT_DOWN = calcbit(GENERAL_NUNCHUK_BTN_SHIFT_DOWN);
const uint64 NUNCHUK_BTN_SHIFT_UP = calcbit(GENERAL_NUNCHUK_BTN_SHIFT_UP);

const uint64 NUNCHUK_BTN_TILT_FRONT = calcbit(GENERAL_NUNCHUK_BTN_TILT_FRONT);
const uint64 NUNCHUK_BTN_TILT_BACK = calcbit(GENERAL_NUNCHUK_BTN_TILT_BACK);
const uint64 NUNCHUK_BTN_TILT_RIGHT = calcbit(GENERAL_NUNCHUK_BTN_TILT_RIGHT);
const uint64 NUNCHUK_BTN_TILT_LEFT = calcbit(GENERAL_NUNCHUK_BTN_TILT_LEFT);

const uint64 CLASSIC_BTN_X = calcbit(GENERAL_CLASSIC_BTN_X);
const uint64 CLASSIC_BTN_Y = calcbit(GENERAL_CLASSIC_BTN_Y);
const uint64 CLASSIC_BTN_A = calcbit(GENERAL_CLASSIC_BTN_A);
const uint64 CLASSIC_BTN_B = calcbit(GENERAL_CLASSIC_BTN_B);
const uint64 CLASSIC_BTN_L = calcbit(GENERAL_CLASSIC_BTN_L);
const uint64 CLASSIC_BTN_R = calcbit(GENERAL_CLASSIC_BTN_R);
const uint64 CLASSIC_BTN_ZL = calcbit(GENERAL_CLASSIC_BTN_ZL);
const uint64 CLASSIC_BTN_ZR = calcbit(GENERAL_CLASSIC_BTN_ZR);
const uint64 CLASSIC_BTN_MINUS = calcbit(GENERAL_CLASSIC_BTN_MINUS);
const uint64 CLASSIC_BTN_PLUS = calcbit(GENERAL_CLASSIC_BTN_PLUS);
const uint64 CLASSIC_BTN_HOME = calcbit(GENERAL_CLASSIC_BTN_HOME);
const uint64 CLASSIC_BTN_RIGHT = calcbit(GENERAL_CLASSIC_BTN_RIGHT);
const uint64 CLASSIC_BTN_LEFT = calcbit(GENERAL_CLASSIC_BTN_LEFT);
const uint64 CLASSIC_BTN_DOWN = calcbit(GENERAL_CLASSIC_BTN_DOWN);
const uint64 CLASSIC_BTN_UP = calcbit(GENERAL_CLASSIC_BTN_UP);

const uint64 CLASSIC_BTN_LSTICK_RIGHT = calcbit(GENERAL_CLASSIC_BTN_LSTICK_RIGHT);
const uint64 CLASSIC_BTN_LSTICK_LEFT = calcbit(GENERAL_CLASSIC_BTN_LSTICK_LEFT);
const uint64 CLASSIC_BTN_LSTICK_DOWN = calcbit(GENERAL_CLASSIC_BTN_LSTICK_DOWN);
const uint64 CLASSIC_BTN_LSTICK_UP = calcbit(GENERAL_CLASSIC_BTN_LSTICK_UP);

const uint64 CLASSIC_BTN_RSTICK_RIGHT = calcbit(GENERAL_CLASSIC_BTN_RSTICK_RIGHT);
const uint64 CLASSIC_BTN_RSTICK_LEFT = calcbit(GENERAL_CLASSIC_BTN_RSTICK_LEFT);

const uint64 CLASSIC_BTN_RSTICK_DOWN = calcbit(GENERAL_CLASSIC_BTN_RSTICK_DOWN);
const uint64 CLASSIC_BTN_RSTICK_UP = calcbit(GENERAL_CLASSIC_BTN_RSTICK_UP);


const uint64 WIIMOTE_BUTTON_MASK =  WIIMOTE_BTN_1 |
                                    WIIMOTE_BTN_2 |
                                    WIIMOTE_BTN_A |
                                    WIIMOTE_BTN_B |
                                    WIIMOTE_BTN_MINUS |
                                    WIIMOTE_BTN_PLUS |
                                    WIIMOTE_BTN_HOME |
                                    WIIMOTE_BTN_RIGHT |
                                    WIIMOTE_BTN_LEFT |
                                    WIIMOTE_BTN_DOWN |
                                    WIIMOTE_BTN_UP;

const uint64 WIIMOTE_SHIFT_MASK = WIIMOTE_BTN_SHIFT_BACKWARD |
                                  WIIMOTE_BTN_SHIFT_FORWARD |
                                  WIIMOTE_BTN_SHIFT_RIGHT |
                                  WIIMOTE_BTN_SHIFT_LEFT |
                                  WIIMOTE_BTN_SHIFT_DOWN |
                                  WIIMOTE_BTN_SHIFT_UP;

const uint64 WIIMOTE_TILT_MASK = WIIMOTE_BTN_TILT_FRONT |
                                 WIIMOTE_BTN_TILT_BACK |
                                 WIIMOTE_BTN_TILT_RIGHT |
                                 WIIMOTE_BTN_TILT_LEFT;

const uint64 NUNCHUK_BUTTON_MASK = NUNCHUK_BTN_C |
                                   NUNCHUK_BTN_Z;

const uint64 NUNCHUK_STICK_MASK = NUNCHUK_BTN_STICK_RIGHT |
                                  NUNCHUK_BTN_STICK_LEFT |
                                  NUNCHUK_BTN_STICK_DOWN |
                                  NUNCHUK_BTN_STICK_UP;

const uint64 NUNCHUK_SHIFT_MASK = NUNCHUK_BTN_SHIFT_BACKWARD |
                                  NUNCHUK_BTN_SHIFT_FORWARD |
                                  NUNCHUK_BTN_SHIFT_RIGHT |
                                  NUNCHUK_BTN_SHIFT_LEFT |
                                  NUNCHUK_BTN_SHIFT_DOWN |
                                  NUNCHUK_BTN_SHIFT_UP;

const uint64 NUNCHUK_TILT_MASK = NUNCHUK_BTN_TILT_FRONT |
                                 NUNCHUK_BTN_TILT_BACK |
                                 NUNCHUK_BTN_TILT_RIGHT |
                                 NUNCHUK_BTN_TILT_LEFT;

const uint64 CLASSIC_BUTTON_MASK = CLASSIC_BTN_X |
                                   CLASSIC_BTN_Y |
                                   CLASSIC_BTN_A |
                                   CLASSIC_BTN_B |
                                   CLASSIC_BTN_L |
                                   CLASSIC_BTN_R |
                                   CLASSIC_BTN_ZL |
                                   CLASSIC_BTN_ZR |
                                   CLASSIC_BTN_MINUS |
                                   CLASSIC_BTN_PLUS |
                                   CLASSIC_BTN_HOME |
                                   CLASSIC_BTN_RIGHT |
                                   CLASSIC_BTN_LEFT |
                                   CLASSIC_BTN_DOWN |
                                   CLASSIC_BTN_UP;

const uint64 CLASSIC_LSTICK_MASK = CLASSIC_BTN_LSTICK_RIGHT  |
                                   CLASSIC_BTN_LSTICK_LEFT |
                                   CLASSIC_BTN_LSTICK_DOWN |
                                   CLASSIC_BTN_LSTICK_UP;

const uint64 CLASSIC_RSTICK_MASK = CLASSIC_BTN_RSTICK_RIGHT  |
                                   CLASSIC_BTN_RSTICK_LEFT |
                                   CLASSIC_BTN_RSTICK_DOWN |
                                   CLASSIC_BTN_RSTICK_UP;

const uint64 WIIMOTE_BUTTON_NOTMASK = ~WIIMOTE_BUTTON_MASK;
const uint64 WIIMOTE_SHIFT_NOTMASK = ~WIIMOTE_SHIFT_MASK;
const uint64 WIIMOTE_TILT_NOTMASK = ~WIIMOTE_TILT_MASK;
const uint64 NUNCHUK_BUTTON_NOTMASK = ~NUNCHUK_BUTTON_MASK;
const uint64 NUNCHUK_STICK_NOTMASK = ~NUNCHUK_STICK_MASK;
const uint64 NUNCHUK_SHIFT_NOTMASK = ~NUNCHUK_SHIFT_MASK;
const uint64 NUNCHUK_TILT_NOTMASK = ~NUNCHUK_TILT_MASK;
const uint64 CLASSIC_BUTTON_NOTMASK = ~CLASSIC_BUTTON_MASK;
const uint64 CLASSIC_LSTICK_NOTMASK = ~CLASSIC_LSTICK_MASK;
const uint64 CLASSIC_RSTICK_NOTMASK = ~CLASSIC_RSTICK_MASK;

namespace Wiimote
{
    namespace Buttons {
        const uint64 Mask = WIIMOTE_BUTTON_MASK;
        const uint64 NotMask = WIIMOTE_BUTTON_NOTMASK;
        const uint64 B1 = WIIMOTE_BTN_1;
        const uint64 B2 = WIIMOTE_BTN_2;
        const uint64 A = WIIMOTE_BTN_A;
        const uint64 B = WIIMOTE_BTN_B;
        const uint64 Minus = WIIMOTE_BTN_MINUS;
        const uint64 Plus = WIIMOTE_BTN_PLUS;
        const uint64 Home = WIIMOTE_BTN_HOME;
        const uint64 Right = WIIMOTE_BTN_RIGHT;
        const uint64 Left = WIIMOTE_BTN_LEFT;
        const uint64 Up = WIIMOTE_BTN_UP;
        const uint64 Down = WIIMOTE_BTN_DOWN;
    }

    namespace Shifts {
        const uint64 Mask = WIIMOTE_SHIFT_MASK;
        const uint64 NotMask = WIIMOTE_SHIFT_NOTMASK;
        const uint64 Backward = WIIMOTE_BTN_SHIFT_BACKWARD;
        const uint64 Forward = WIIMOTE_BTN_SHIFT_FORWARD;
        const uint64 Right = WIIMOTE_BTN_SHIFT_RIGHT;
        const uint64 Left = WIIMOTE_BTN_SHIFT_LEFT;
        const uint64 Up = WIIMOTE_BTN_SHIFT_UP;
        const uint64 Down = WIIMOTE_BTN_SHIFT_DOWN;
    }

    namespace Tilts {
        const uint64 Mask = WIIMOTE_TILT_MASK;
        const uint64 NotMask = WIIMOTE_TILT_NOTMASK;
        const uint64 Front = WIIMOTE_BTN_TILT_FRONT;
        const uint64 Back = WIIMOTE_BTN_TILT_BACK;
        const uint64 Right = WIIMOTE_BTN_TILT_RIGHT;
        const uint64 Left = WIIMOTE_BTN_TILT_LEFT;
    }
}

namespace Nunchuk
{
    namespace Buttons {
        const uint64 Mask = NUNCHUK_BUTTON_MASK;
        const uint64 NotMask = NUNCHUK_BUTTON_NOTMASK;
        const uint64 Z = NUNCHUK_BTN_Z;
        const uint64 C = NUNCHUK_BTN_C;
    }

    namespace Shifts {
        const uint64 Mask = NUNCHUK_SHIFT_MASK;
        const uint64 NotMask = NUNCHUK_SHIFT_NOTMASK;
        const uint64 Backward = NUNCHUK_BTN_SHIFT_BACKWARD;
        const uint64 Forward = NUNCHUK_BTN_SHIFT_FORWARD;
        const uint64 Right = NUNCHUK_BTN_SHIFT_RIGHT;
        const uint64 Left = NUNCHUK_BTN_SHIFT_LEFT;
        const uint64 Up = NUNCHUK_BTN_SHIFT_UP;
        const uint64 Down = NUNCHUK_BTN_SHIFT_DOWN;
    }

    namespace Tilts {
        const uint64 Mask = NUNCHUK_TILT_MASK;
        const uint64 NotMask = NUNCHUK_TILT_NOTMASK;
        const uint64 Front = NUNCHUK_BTN_TILT_FRONT;
        const uint64 Back = NUNCHUK_BTN_TILT_BACK;
        const uint64 Right = NUNCHUK_BTN_TILT_RIGHT;
        const uint64 Left = NUNCHUK_BTN_TILT_LEFT;
    }

    namespace Stick
    {
        const uint8 StickXMax = 0xFF;
        const uint8 StickYMax = 0xFF;
        const uint64 Mask = NUNCHUK_STICK_MASK;
        const uint64 NotMask = NUNCHUK_STICK_NOTMASK;
        const uint64 Up = NUNCHUK_BTN_STICK_UP;
        const uint64 Down = NUNCHUK_BTN_STICK_DOWN;
        const uint64 Right = NUNCHUK_BTN_STICK_RIGHT;
        const uint64 Left = NUNCHUK_BTN_STICK_LEFT;
    }
}

namespace Classic
{
    namespace Buttons {
        const uint64 Mask = CLASSIC_BUTTON_MASK;
        const uint64 NotMask = CLASSIC_BUTTON_NOTMASK;
        const uint64 X = CLASSIC_BTN_X;
        const uint64 Y = CLASSIC_BTN_Y;
        const uint64 A = CLASSIC_BTN_A;
        const uint64 B = CLASSIC_BTN_B;
        const uint64 L = CLASSIC_BTN_L;
        const uint64 R = CLASSIC_BTN_R;
        const uint64 ZL = CLASSIC_BTN_ZL;
        const uint64 ZR = CLASSIC_BTN_ZR;
        const uint64 Minus = CLASSIC_BTN_MINUS;
        const uint64 Plus = CLASSIC_BTN_PLUS;
        const uint64 Home = CLASSIC_BTN_HOME;
        const uint64 Right = CLASSIC_BTN_RIGHT;
        const uint64 Left = CLASSIC_BTN_LEFT;
        const uint64 Down = CLASSIC_BTN_DOWN;
        const uint64 Up = CLASSIC_BTN_UP;
    }

    namespace Sticks {
        namespace Left {
            const uint8 StickXMax = 0x3F;
            const uint8 StickYMax = 0x3F;
            const uint64 Mask = CLASSIC_LSTICK_MASK;
            const uint64 NotMask = CLASSIC_LSTICK_NOTMASK;
            const uint64 Up = CLASSIC_BTN_LSTICK_UP;
            const uint64 Down = CLASSIC_BTN_LSTICK_DOWN;
            const uint64 Right = CLASSIC_BTN_LSTICK_RIGHT;
            const uint64 Left = CLASSIC_BTN_LSTICK_LEFT;
        }
        namespace Right {
            const uint8 StickXMax = 0x1F;
            const uint8 StickYMax = 0x1F;
            const uint64 Mask = CLASSIC_RSTICK_MASK;
            const uint64 NotMask = CLASSIC_RSTICK_NOTMASK;
            const uint64 Up = CLASSIC_BTN_RSTICK_UP;
            const uint64 Down = CLASSIC_BTN_RSTICK_DOWN;
            const uint64 Right = CLASSIC_BTN_RSTICK_RIGHT;
            const uint64 Left = CLASSIC_BTN_RSTICK_LEFT;
        }
    }
}

#endif
