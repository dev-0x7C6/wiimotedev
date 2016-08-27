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

void UInputProfileManager::setupClassicJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<EIOClassicJoystick>(name.toStdString(), assign);
	device->setDpadInvertX(settings.value("DStickInvertX", 0x00).toBool());
	device->setDpadInvertY(settings.value("DStickInvertY", 0x00).toBool());
	device->setLStickInvertX(settings.value("LStickInvertX", 0x00).toBool());
	device->setLStickInvertY(settings.value("LStickInvertY", 0x00).toBool());
	device->setRStickInvertX(settings.value("RStickInvertX", 0x00).toBool());
	device->setRStickInvertY(settings.value("RStickInvertY", 0x00).toBool());
	device->setReportButtons(settings.value("ReportButtons", 0x01).toBool());
	device->setReportDStick(settings.value("ReportDStick", 0x01).toBool());
	device->setReportLStick(settings.value("ReportLStick", 0x01).toBool());
	device->setReportRStick(settings.value("ReportRStick", 0x01).toBool());
	device->open();
	device->configure();

	if (device->create())
		m_gamepads.emplace_back(std::move(device));
}

void UInputProfileManager::setupWiimoteJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<EIOWiimoteJoystick>(name.toStdString(), assign,
		EIOWiimoteJoystick::DPadPositionSwitchable,
		EIOWiimoteJoystick::GamepadVertical);
	device->setDStickInvertX(settings.value("DStickInvertX", 0x00).toBool());
	device->setDStickInvertY(settings.value("DStickInvertY", 0x00).toBool());
	device->setHomeSwitchPosition(settings.value("PositionSwitchable", 0x01).toBool());
	device->setReportButtons(settings.value("ReportButtons", 0x01).toBool());
	device->setReportDStick(settings.value("ReportDStick", 0x01).toBool());
	device->setReportPitch(settings.value("ReportPitch", 0x01).toBool());
	device->setReportRoll(settings.value("ReportRoll", 0x01).toBool());
	device->open();
	device->configure();

	if (device->create())
		m_gamepads.emplace_back(std::move(device));
}

void UInputProfileManager::setupNunchukJoystick(uint32_t assign, const QString &name, QSettings &settings) {
	auto device = std::make_unique<EIONunchukJoystick>(name.toStdString(), assign);
	device->setStickInvertX(settings.value("DStickInvertX", 0x00).toBool());
	device->setStickInvertY(settings.value("DStickInvertY", 0x00).toBool());
	device->setReportButtons(settings.value("ReportButtons", 0x01).toBool());
	device->setReportStick(settings.value("ReportDStick", 0x01).toBool());
	device->setReportPitch(settings.value("ReportPitch", 0x01).toBool());
	device->setReportRoll(settings.value("ReportRoll", 0x01).toBool());
	device->open();
	device->configure();

	if (device->create())
		m_gamepads.emplace_back(std::move(device));
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

void UInputProfileManager::dbusWiimoteAcc(uint32_t id, struct accdata acc) {
	gamepad_iterator(IGamepad::Type::Wiimote, id,
		[&acc](const auto &gamepad) { gamepad->inputAccelerometer(acc.pitch, acc.roll); });
}

void UInputProfileManager::dbusWiimoteButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Wiimote, id,
		[&buttons](const auto &gamepad) { gamepad->inputButtons(buttons); });
}

void UInputProfileManager::dbusNunchukAcc(uint32_t id, struct accdata acc) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&acc](const auto &gamepad) { gamepad->inputAccelerometer(acc.pitch, acc.roll); });
}

void UInputProfileManager::dbusNunchukButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&buttons](const auto &gamepad) { gamepad->inputButtons(buttons); });
}

void UInputProfileManager::dbusNunchukStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::Nunchuk, id,
		[&stick](const auto &gamepad) { gamepad->inputStick(IGamepad::Stick::NunchukStick, stick.x, stick.y); });
}

void UInputProfileManager::dbusClassicControllerButtons(uint32_t id, uint64_t buttons) {
	gamepad_iterator(IGamepad::Type::ClassicController, id,
		[&buttons](const auto &gamepad) { gamepad->inputButtons(buttons); });
}

void UInputProfileManager::dbusClassicControllerLStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::ClassicController, id,
		[&stick](const auto &gamepad) { gamepad->inputStick(IGamepad::Stick::ClassicControllerLStick, stick.x, stick.y); });
}

void UInputProfileManager::dbusClassicControllerRStick(uint32_t id, struct stickdata stick) {
	gamepad_iterator(IGamepad::Type::ClassicController, id,
		[&stick](const auto &gamepad) { gamepad->inputStick(IGamepad::Stick::ClassicControllerRStick, stick.x, stick.y); });
}
