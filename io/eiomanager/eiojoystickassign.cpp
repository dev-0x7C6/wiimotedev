/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008-2015  Bartłomiej Burdukiewicz                               *
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

#include "manager.h"

#include "emulation/gamepads/classic-gamepad.h"
#include "emulation/gamepads/nunchuk-gamepad.h"
#include "emulation/gamepads/wiimote-gamepad.h"

using namespace io::emulation::gamepad;

void UInputProfileManager::setupClassicJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<ClassicGamepad>(name.toStdString(), assign);
	device->open();
	device->configure();

	if (device->create()) {
		device->centerAllAxis();
		m_gamepads.emplace_back(std::move(device));
	}
}

void UInputProfileManager::setupWiimoteJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<WiimoteGamepad>(name.toStdString(), assign,
		WiimoteGamepad::DPadPositionSwitchable,
		WiimoteGamepad::GamepadVertical);
	device->setHomeSwitchPosition(settings.value("PositionSwitchable", 0x01).toBool());
	device->open();
	device->configure();

	if (device->create()) {
		device->centerAllAxis();
		m_gamepads.emplace_back(std::move(device));
	}
}

void UInputProfileManager::setupNunchukJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<NunchukGamepad>(name.toStdString(), assign);
	device->open();
	device->configure();

	if (device->create()) {
		device->centerAllAxis();
		m_gamepads.emplace_back(std::move(device));
	}
}

void UInputProfileManager::assignJoystickEvents(const QString &key, QSettings &settings) {
	settings.beginGroup(key);
	uint32_t assign = settings.value("assign").toULongLong();
	QString device = settings.value("device").toString().toLower();
	QString name = settings.value("name").toString();

	if (device == QString::fromUtf8("classic"))
		setupClassicJoystick(assign, name, settings);
	else if (device == QString::fromUtf8("nunchuk"))
		setupNunchukJoystick(assign, name, settings);
	else if (device == QString::fromUtf8("wiimote"))
		setupWiimoteJoystick(assign, name, settings);

	settings.endGroup();
}

void UInputProfileManager::freeJoystickEvents() { m_gamepads.clear(); }

void UInputProfileManager::gamepad_iterator(const IGamepad::Type type, const uint32_t id, std::function<void(const std::unique_ptr<IGamepad> &)> &&function) {
	for (const auto &gamepad : m_gamepads) {
		if (gamepad->id() == id && gamepad->type() == type && gamepad->isCreated()) {
			function(gamepad);
		}
	}
}

void UInputProfileManager::dbusWiimoteButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Wiimote, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void UInputProfileManager::dbusNunchukButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void UInputProfileManager::dbusNunchukStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&stick](const auto &gamepad) { gamepad->input(IGamepad::Stick::NunchukStick, stick.x, stick.y); });
}

void UInputProfileManager::dbusClassicControllerButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&buttons](const auto &gamepad) { gamepad->input(buttons); });
}

void UInputProfileManager::dbusClassicControllerLStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&stick](const auto &gamepad) { gamepad->input(IGamepad::Stick::ClassicLStick, stick.x, stick.y); });
}

void UInputProfileManager::dbusClassicControllerRStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Classic, id,
		[&stick](const auto &gamepad) { gamepad->input(IGamepad::Stick::ClassicRStick, stick.x, stick.y); });
}
