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
	EIONunchukJoystick *device = new EIONunchukJoystick(name.toStdString(), assign);
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

void UInputProfileManager::dbusWiimoteAcc(uint32_t id, struct accdata acc) {
	for (int i = 0; i < EIOWiimoteJoysticks.count(); ++i)
		if (EIOWiimoteJoysticks[i]->assign() == id) {
			EIOWiimoteJoysticks[i]->setWiimoteAcc(acc.pitch, acc.roll);
			return;
		}
}

void UInputProfileManager::dbusWiimoteButtons(uint32_t id, uint64_t buttons) {
	for (int i = 0; i < EIOWiimoteJoysticks.count(); ++i)
		if (EIOWiimoteJoysticks[i]->assign() == id) {
			EIOWiimoteJoysticks[i]->setWiimoteButtons(buttons);
			return;
		}
}

void UInputProfileManager::dbusNunchukAcc(uint32_t id, struct accdata acc) {
	for (int i = 0; i < EIONunchukJoysticks.count(); ++i)
		if (EIONunchukJoysticks[i]->assign() == id) {
			EIONunchukJoysticks[i]->setNunchukAcc(acc.pitch, acc.roll);
			return;
		}
}

void UInputProfileManager::dbusNunchukButtons(uint32_t id, uint64_t buttons) {
	for (int i = 0; i < EIONunchukJoysticks.count(); ++i)
		if (EIONunchukJoysticks[i]->assign() == id) {
			EIONunchukJoysticks[i]->setNunchukButtons(buttons);
			return;
		}
}

void UInputProfileManager::dbusNunchukStick(uint32_t id, struct stickdata stick) {
	for (int i = 0; i < EIONunchukJoysticks.count(); ++i)
		if (EIONunchukJoysticks[i]->assign() == id) {
			EIONunchukJoysticks[i]->setNunchukStick(stick.x, 0xFF - stick.y);
			return;
		}
}

void UInputProfileManager::dbusClassicControllerButtons(uint32_t id, uint64_t buttons) {
	for (int i = 0; i < EIOClassicJoysticks.count(); ++i)
		if (EIOClassicJoysticks[i]->assign() == id) {
			EIOClassicJoysticks[i]->setButtons(buttons);
			return;
		}
}

void UInputProfileManager::dbusClassicControllerLStick(uint32_t id, struct stickdata stick) {
	for (int i = 0; i < EIOClassicJoysticks.count(); ++i)
		if (EIOClassicJoysticks[i]->assign() == id) {
			EIOClassicJoysticks[i]->setStick(EIOClassicJoystick::LeftStick, stick.x, stick.y);
			return;
		}
}

void UInputProfileManager::dbusClassicControllerRStick(uint32_t id, struct stickdata stick) {
	for (int i = 0; i < EIOClassicJoysticks.count(); ++i)
		if (EIOClassicJoysticks[i]->assign() == id) {
			EIOClassicJoysticks[i]->setStick(EIOClassicJoystick::RightStick, stick.x, stick.y);
			return;
		}
}
