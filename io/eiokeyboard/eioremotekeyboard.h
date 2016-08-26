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

#pragma once

#include <io/emulation/event-device.h>
#include "linux/usr/include/wiimotedev/consts.h"
#include <io/functionals/hash-compare.h>

#include <QTimer>
#include <QTime>

#include <QMap>

struct KeyboardAction {
	QHash<uint32_t, uint64_t> event;
	QList<uint> keys;
	bool pushed;
};

class EIORemoteKeyboard : public QObject {
	Q_OBJECT
public:
	EIORemoteKeyboard(EventDevice &device);
	~EIORemoteKeyboard();

	void addKeyboardAction(KeyboardAction &);
	void clearKeyboardActions();
	void setCompareType(QString);

public Q_SLOTS:
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

private:
	void pressKeyboardButtons(QList<uint> &);
	void releaseKeyboardButtons(QList<uint> &);
	void pressKeyboardExtendedButton(uint32_t);
	void releaseKeyboardExtendedButton(uint32_t);

private:
	EventDevice &m_device;
	uint32_t id;
	uint32_t compareType;

	QHash<uint32_t, uint64_t> buttons;

	QList<KeyboardAction *> keyboardActions;
};
