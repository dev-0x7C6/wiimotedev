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

#ifndef EIOGAMEPADMANAGER_H
#define EIOGAMEPADMANAGER_H

#include <QObject>

#include "eioclassicjoystick.h"
#include "eionunchukjoystick.h"
#include "eiowiimotejoystick.h"

class EIOGamepadManager : public QObject {
	Q_OBJECT
public:
	explicit EIOGamepadManager(QObject *parent = 0);

	enum EIOGamepadVariants {
		ClassicGamepadVariant,
		NunchukGamepadVariant,
		WiimoteGamepadVariant
	};

	//uint32_t create(EIOGamepadVariants variant, uint32_t assign, QList< QPair< QString, QVariant> > arguments);
	bool destroy(uint32_t id);

signals:

public slots:
};

#endif // EIOGAMEPADMANAGER_H
