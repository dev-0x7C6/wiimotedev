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

#ifndef WIIMOTEDEVPROTO_H
#define WIIMOTEDEVPROTO_H

const unsigned short int iddbusWiimoteGeneralButtons = 0x01;
const unsigned short int iddbusWiimoteConnected= 0x02;
const unsigned short int iddbusWiimoteDisconnected = 0x03;
const unsigned short int iddbusWiimoteBatteryLife = 0x04;
const unsigned short int iddbusWiimoteButtons = 0x05;
const unsigned short int iddbusWiimoteStatus = 0x06;
const unsigned short int iddbusWiimoteInfrared = 0x07;
const unsigned short int iddbusWiimoteAcc = 0x08;
const unsigned short int iddbusNunchukPlugged = 0x09;
const unsigned short int iddbusNunchukUnplugged = 0x0A;
const unsigned short int iddbusNunchukButtons = 0x0B;
const unsigned short int iddbusNunchukStick = 0x0C;
const unsigned short int iddbusNunchukAcc = 0x0D;
const unsigned short int iddbusClassicControllerPlugged = 0x0E;
const unsigned short int iddbusClassicControllerUnplugged = 0x0F;
const unsigned short int iddbusClassicControllerButtons = 0x10;
const unsigned short int iddbusClassicControllerLStick = 0x11;
const unsigned short int iddbusClassicControllerRStick = 0x12;

const unsigned short int iddbusResponseBool = 0xF0;
const unsigned short int iddbusResponseInt8 = 0xF1;

const unsigned short int iddbusWiimoteGetRumbleStatus = 0x01;
const unsigned short int iddbusWiimoteSetLedStatus = 0x02;
const unsigned short int iddbusWiimoteSetRumbleStatus = 0x03;
const unsigned short int iddbusWiimoteGetLedStatus = 0x04;

#endif // WIIMOTEDEVPROTO_H
