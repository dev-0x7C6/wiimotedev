/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2008  Bartłomiej Burdukiewicz                                    *
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

#include "settings.h"

WiimotedevSettings::WiimotedevSettings(QObject *parent):
  QObject(parent)
{
  m_settings = new QSettings(WIIMOTEDEV_CONFIG_FILE, QSettings::IniFormat, this);
  m_connections = new QSettings(WIIMOTEDEV_CONNECTIONS_CONFIG_FILE, QSettings::IniFormat, this);
  reload();
}

uint WiimotedevSettings::registerWiiremote(const QString &mac) {
  uint id = m_sequence.value(mac, 0);

  if (id)
    return id;

  id = 1;

  QHashIterator < QString, uint> iterator(m_sequence);
  while (iterator.hasNext()) {
    iterator.next();
    if (iterator.value() == id) {
      id++;
      iterator.toFront();
    }
  }

  m_sequence[mac] = id;
  m_connections->setValue("WIIREMOTE-" + mac + "/id", id);
  m_connections->setValue("WIIREMOTE-" + mac + "/mac", mac);
  m_connections->sync();

  return id;
}

void WiimotedevSettings::reload()
{
  m_powersave = m_settings->value("features/powersave", 10).toUInt();
  m_sequence.clear();

  foreach (const QString &key, m_connections->childGroups())
    m_sequence[m_connections->value(key + "/mac", QString()).toString().toUpper()]
        = m_connections->value(key + "/id", 0).toULongLong();
}

QHash < QString, uint> WiimotedevSettings::connectionTable() {
  return m_sequence;
}

uint WiimotedevSettings::powerSaveTiemout() {
  return m_powersave;
}
