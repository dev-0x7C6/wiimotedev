/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef WIIMOTEDEV_H
#define WIIMOTEDEV_H

#define WIIMOTEDEV_DBUS_OBJECT_NAME "/deviceEvents"
#define WIIMOTEDEV_DBUS_SERVICE_NAME "org.wiimotedev.deviceEvents"

const unsigned char STATUS_WIIMOTE_DISCONNECTED = 0x00;
const unsigned char STATUS_WIIMOTE_CONNECTED = 0x01;
const unsigned char STATUS_WIIMOTE_NUNCHUK_CONNECTED = 0x02;
const unsigned char STATUS_WIIMOTE_CLASSIC_CONNECTED = 0x04;


const unsigned int NUNCHUK_STICK_X_MAX = 0xFF;
const unsigned int NUNCHUK_STICK_Y_MAX = 0xFF;

const unsigned int CLASSIC_LSTICK_X_MAX = 0x3F;
const unsigned int CLASSIC_LSTICK_Y_MAX = 0x3F;

const unsigned int CLASSIC_RSTICK_X_MAX = 0x1F;
const unsigned int CLASSIC_RSTICK_Y_MAX = 0x1F;

struct irpoint
{
    signed short int size;
    unsigned short int x;
    unsigned short int y;
};

struct accdata
{
    unsigned char x;
    unsigned char y;
    unsigned char z;
    double pitch;
    double roll;
};

struct stickdata
{
    unsigned char x;
    unsigned char y;
};

enum GENERAL_BUTTONS
{
    GENERAL_WIIMOTE_BTN_1,
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

const unsigned long long WIIMOTE_BTN_1 = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_1;
const unsigned long long WIIMOTE_BTN_2 = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_2;
const unsigned long long WIIMOTE_BTN_A = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_A;
const unsigned long long WIIMOTE_BTN_B = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_B;

const unsigned long long WIIMOTE_BTN_MINUS = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_MINUS;
const unsigned long long WIIMOTE_BTN_PLUS =  static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_PLUS;
const unsigned long long WIIMOTE_BTN_HOME =  static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_HOME;

const unsigned long long WIIMOTE_BTN_RIGHT = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_RIGHT;
const unsigned long long WIIMOTE_BTN_LEFT =  static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_LEFT;
const unsigned long long WIIMOTE_BTN_DOWN =  static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_DOWN;
const unsigned long long WIIMOTE_BTN_UP =    static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_UP;

const unsigned long long WIIMOTE_BTN_SHIFT_BACKWARD = static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_SHIFT_BACKWARD;
const unsigned long long WIIMOTE_BTN_SHIFT_FORWARD =  static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_SHIFT_FORWARD;
const unsigned long long WIIMOTE_BTN_SHIFT_RIGHT =    static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_SHIFT_RIGHT;
const unsigned long long WIIMOTE_BTN_SHIFT_LEFT =     static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_SHIFT_LEFT;
const unsigned long long WIIMOTE_BTN_SHIFT_DOWN =     static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_SHIFT_DOWN;
const unsigned long long WIIMOTE_BTN_SHIFT_UP =       static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_SHIFT_UP;

const unsigned long long WIIMOTE_BTN_TILT_FRONT =     static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_TILT_FRONT;
const unsigned long long WIIMOTE_BTN_TILT_BACK =      static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_TILT_BACK;
const unsigned long long WIIMOTE_BTN_TILT_RIGHT =     static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_TILT_RIGHT;
const unsigned long long WIIMOTE_BTN_TILT_LEFT =      static_cast<unsigned long long>( true) << GENERAL_WIIMOTE_BTN_TILT_LEFT;

const unsigned long long NUNCHUK_BTN_C = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_C;
const unsigned long long NUNCHUK_BTN_Z = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_Z;

const unsigned long long NUNCHUK_BTN_STICK_RIGHT = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_STICK_RIGHT;
const unsigned long long NUNCHUK_BTN_STICK_LEFT =  static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_STICK_LEFT;
const unsigned long long NUNCHUK_BTN_STICK_DOWN =  static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_STICK_DOWN;
const unsigned long long NUNCHUK_BTN_STICK_UP =    static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_STICK_UP;

const unsigned long long NUNCHUK_BTN_SHIFT_BACKWARD = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_SHIFT_BACKWARD;
const unsigned long long NUNCHUK_BTN_SHIFT_FORWARD = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_SHIFT_FORWARD;
const unsigned long long NUNCHUK_BTN_SHIFT_RIGHT = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_SHIFT_RIGHT;
const unsigned long long NUNCHUK_BTN_SHIFT_LEFT = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_SHIFT_LEFT;
const unsigned long long NUNCHUK_BTN_SHIFT_DOWN = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_SHIFT_DOWN;
const unsigned long long NUNCHUK_BTN_SHIFT_UP = static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_SHIFT_UP;

const unsigned long long NUNCHUK_BTN_TILT_FRONT =  static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_TILT_FRONT;
const unsigned long long NUNCHUK_BTN_TILT_BACK =   static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_TILT_BACK;
const unsigned long long NUNCHUK_BTN_TILT_RIGHT =  static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_TILT_RIGHT;
const unsigned long long NUNCHUK_BTN_TILT_LEFT =   static_cast<unsigned long long>( true) << GENERAL_NUNCHUK_BTN_TILT_LEFT;

const unsigned long long CLASSIC_BTN_X =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_X;
const unsigned long long CLASSIC_BTN_Y =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_Y;
const unsigned long long CLASSIC_BTN_A =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_A;
const unsigned long long CLASSIC_BTN_B =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_B;
const unsigned long long CLASSIC_BTN_L =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_L;
const unsigned long long CLASSIC_BTN_R =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_R;
const unsigned long long CLASSIC_BTN_ZL = static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_ZL;
const unsigned long long CLASSIC_BTN_ZR = static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_ZR;
const unsigned long long CLASSIC_BTN_MINUS = static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_MINUS;
const unsigned long long CLASSIC_BTN_PLUS =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_PLUS;
const unsigned long long CLASSIC_BTN_HOME =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_HOME;
const unsigned long long CLASSIC_BTN_RIGHT = static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_RIGHT;
const unsigned long long CLASSIC_BTN_LEFT =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_LEFT;
const unsigned long long CLASSIC_BTN_DOWN =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_DOWN;
const unsigned long long CLASSIC_BTN_UP =    static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_UP;

const unsigned long long CLASSIC_BTN_LSTICK_RIGHT = static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_LSTICK_RIGHT;
const unsigned long long CLASSIC_BTN_LSTICK_LEFT =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_LSTICK_LEFT;
const unsigned long long CLASSIC_BTN_LSTICK_DOWN =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_LSTICK_DOWN;
const unsigned long long CLASSIC_BTN_LSTICK_UP =    static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_LSTICK_UP;

const unsigned long long CLASSIC_BTN_RSTICK_RIGHT = static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_RSTICK_RIGHT;
const unsigned long long CLASSIC_BTN_RSTICK_LEFT =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_RSTICK_LEFT;

const unsigned long long CLASSIC_BTN_RSTICK_DOWN =  static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_RSTICK_DOWN;
const unsigned long long CLASSIC_BTN_RSTICK_UP =    static_cast<unsigned long long>( true) << GENERAL_CLASSIC_BTN_RSTICK_UP;

const unsigned long long WIIMOTE_BUTTON_MASK =  WIIMOTE_BTN_1 |WIIMOTE_BTN_2 | WIIMOTE_BTN_A | WIIMOTE_BTN_B |
                                                WIIMOTE_BTN_MINUS | WIIMOTE_BTN_PLUS | WIIMOTE_BTN_HOME |
                                                WIIMOTE_BTN_RIGHT | WIIMOTE_BTN_LEFT | WIIMOTE_BTN_DOWN | WIIMOTE_BTN_UP;

const unsigned long long WIIMOTE_SHIFT_MASK = WIIMOTE_BTN_SHIFT_BACKWARD | WIIMOTE_BTN_SHIFT_FORWARD | WIIMOTE_BTN_SHIFT_RIGHT |
                                              WIIMOTE_BTN_SHIFT_LEFT | WIIMOTE_BTN_SHIFT_DOWN | WIIMOTE_BTN_SHIFT_UP;

const unsigned long long WIIMOTE_TILT_MASK = WIIMOTE_BTN_TILT_FRONT | WIIMOTE_BTN_TILT_BACK | WIIMOTE_BTN_TILT_RIGHT | WIIMOTE_BTN_TILT_LEFT;

const unsigned long long NUNCHUK_BUTTON_MASK = NUNCHUK_BTN_C | NUNCHUK_BTN_Z;

const unsigned long long NUNCHUK_STICK_MASK = NUNCHUK_BTN_STICK_RIGHT | NUNCHUK_BTN_STICK_LEFT | NUNCHUK_BTN_STICK_DOWN | NUNCHUK_BTN_STICK_UP;

const unsigned long long NUNCHUK_SHIFT_MASK = GENERAL_NUNCHUK_BTN_SHIFT_BACKWARD | GENERAL_NUNCHUK_BTN_SHIFT_FORWARD | GENERAL_NUNCHUK_BTN_SHIFT_RIGHT |
                                              GENERAL_NUNCHUK_BTN_SHIFT_LEFT | GENERAL_NUNCHUK_BTN_SHIFT_DOWN | GENERAL_NUNCHUK_BTN_SHIFT_UP;

const unsigned long long NUNCHUK_TILT_MASK = NUNCHUK_BTN_TILT_FRONT | NUNCHUK_BTN_TILT_BACK | NUNCHUK_BTN_TILT_RIGHT | NUNCHUK_BTN_TILT_LEFT;

const unsigned long long CLASSIC_BUTTON_MASK = CLASSIC_BTN_X | CLASSIC_BTN_Y | CLASSIC_BTN_A | CLASSIC_BTN_B |
                                               CLASSIC_BTN_L | CLASSIC_BTN_R | CLASSIC_BTN_ZL | CLASSIC_BTN_ZR |
                                               CLASSIC_BTN_MINUS | CLASSIC_BTN_PLUS | CLASSIC_BTN_HOME | CLASSIC_BTN_RIGHT |
                                               CLASSIC_BTN_LEFT | CLASSIC_BTN_DOWN | CLASSIC_BTN_UP;

const unsigned long long CLASSIC_LSTICK_MASK = CLASSIC_BTN_LSTICK_RIGHT  | CLASSIC_BTN_LSTICK_LEFT | CLASSIC_BTN_LSTICK_DOWN | CLASSIC_BTN_LSTICK_UP;

const unsigned long long CLASSIC_RSTICK_MASK = CLASSIC_BTN_RSTICK_RIGHT  | CLASSIC_BTN_RSTICK_LEFT | CLASSIC_BTN_RSTICK_DOWN | CLASSIC_BTN_RSTICK_UP;

const unsigned long long WIIMOTE_BUTTON_NOTMASK = ~WIIMOTE_BUTTON_MASK;
const unsigned long long WIIMOTE_SHIFT_NOTMASK = ~WIIMOTE_SHIFT_MASK;
const unsigned long long WIIMOTE_TILT_NOTMASK = ~WIIMOTE_TILT_MASK;
const unsigned long long NUNCHUK_BUTTON_NOTMASK = ~NUNCHUK_BUTTON_MASK;
const unsigned long long NUNCHUK_STICK_NOTMASK = ~NUNCHUK_STICK_MASK;
const unsigned long long NUNCHUK_SHIFT_NOTMASK = ~NUNCHUK_SHIFT_MASK;
const unsigned long long NUNCHUK_TILT_NOTMASK = ~NUNCHUK_TILT_MASK;
const unsigned long long CLASSIC_BUTTON_NOTMASK = ~CLASSIC_BUTTON_MASK;
const unsigned long long CLASSIC_LSTICK_NOTMASK = ~CLASSIC_LSTICK_MASK;
const unsigned long long CLASSIC_RSTICK_NOTMASK = ~CLASSIC_RSTICK_MASK;

#endif
