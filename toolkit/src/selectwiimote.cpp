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

#include "selectwiimote.h"
#include "ui_selectwiimote.h"

SelectWiimote::SelectWiimote(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SelectWiimote)
{
  ui->setupUi(this);
  wiimoteid = 0;
  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(selectButtonPushed()));
}

SelectWiimote::~SelectWiimote()
{
  delete ui;
}

void SelectWiimote::selectButtonPushed()
{
  wiimoteid = ui->comboBox->currentText().toInt();
  close();
}

void SelectWiimote::setWiimoteList(QList < uint> &list)
{
  qSort(list.begin(), list.end());
  for (register int i = 0; i < list.count(); ++i)
    ui->comboBox->addItem(QString::number(list.at(i)));
}
