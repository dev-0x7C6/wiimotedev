/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2010  Bartłomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
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

WiimotedevSettings::WiimotedevSettings(const QString &file, QObject *parent):
  QObject(parent)
{
  m_settings = new QSettings(file, QSettings::IniFormat, this);
  reload();
}

void WiimotedevSettings::reload()
{
  m_settings->sync();
  m_powersave = m_settings->value("features/powersave", 10).toUInt();

  m_sequence.clear();

  m_settings->beginGroup("sequence");
  for (register int i = 0; i < m_settings->allKeys().count(); ++i)
    m_sequence[m_settings->allKeys().at(i)] = m_settings->value(m_settings->allKeys().at(i), 0).toUInt();

  m_settings->endGroup();
}

QHash < QString, quint32> WiimotedevSettings::getWiiremoteSequence() {
  return m_sequence;
}

quint32 WiimotedevSettings::powerSaveValue() {
  return m_powersave;
}

quint32  WiimotedevSettings::registerWiiremote(const QString &mac) {
  quint32 id = 1;

  if (!m_sequence.values().isEmpty())
    while(m_sequence.values().indexOf(id) != -1) id++;

  m_sequence[mac] = id;

  m_settings->beginGroup("sequence");
  m_settings->setValue(mac, id);
  m_settings->endGroup();

  return id;
}
