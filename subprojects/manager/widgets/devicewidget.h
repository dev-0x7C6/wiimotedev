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

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QProcess>

enum Extensions {
  ExtNone,
  ExtNunchuk,
  ExtClassic
};

namespace Ui {
  class DeviceWidget;
}

class DeviceWidget : public QWidget
{
  Q_OBJECT
  QProcess process;
  QString physicalAddress;
  quint32 id;
  quint32 exts;

public:
  explicit DeviceWidget(QString physicalAddress, quint32 id, quint32 exts, QWidget *parent = 0);
  ~DeviceWidget();

public Q_SLOTS:
  void dbusNunchukPlugged(quint32);
  void dbusNunchukUnplugged(quint32);
  void dbusClassicControllerPlugged(quint32);
  void dbusClassicControllerUnplugged(quint32);

private:
  Ui::DeviceWidget *ui;

private Q_SLOTS:
  void runToolkit();
  void startToolkit();
  void closeToolkit();

};

#endif // DEVICEWIDGET_H
