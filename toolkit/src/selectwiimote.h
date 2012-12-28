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

#ifndef SELECTWIIMOTE_H
#define SELECTWIIMOTE_H

#include <QDialog>

namespace Ui {
  class SelectWiimote;
}

class SelectWiimote : public QDialog
{
  Q_OBJECT
public:
  explicit SelectWiimote(QWidget *parent = 0);
  ~SelectWiimote();

  void setWiimoteList(QList < uint> &list);
  inline uint getSelectedWiimote() { return wiimoteid; }

private slots:
  void selectButtonPushed();

private:
  Ui::SelectWiimote *ui;
  uint wiimoteid;
};

#endif // SELECTWIIMOTE_H
