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

#include "devicewidget.h"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QString physicalAddress, quint32 id, quint32 exts, QWidget *parent) :
  QWidget(parent),
  physicalAddress(physicalAddress),
  id(id),
  exts(exts),
  ui(new Ui::DeviceWidget)
{
  ui->setupUi(this);
  ui->physicalAddress->setText(QString("Physical address: %1").arg(physicalAddress));
  ui->id->setText(QString("Ident number: %1").arg(id));
  //ui->extensions->setText(QString("Extensions: %1").arg(exts));

  switch (exts) {
  case ExtNone:
    break;
  case ExtNunchuk:
    ui->nunchukIcon->setPixmap(QPixmap(":/exts/nunchuk_active.png"));
    break;
  case ExtClassic:
    ui->classicIcon->setPixmap(QPixmap(":/exts/controller_active.png"));
    break;
  }

  connect(ui->runToolkit, SIGNAL(clicked()), this, SLOT(runToolkit()));
  connect(&process, SIGNAL(finished(int)), this, SLOT(closeToolkit()));
  connect(&process, SIGNAL(started()), this, SLOT(startToolkit()));
}

DeviceWidget::~DeviceWidget()
{
  delete ui;
}

void DeviceWidget::dbusNunchukPlugged(quint32) {
  ui->nunchukIcon->setPixmap(QPixmap(":/exts/nunchuk_active.png"));
}

void DeviceWidget::dbusNunchukUnplugged(quint32) {
  ui->nunchukIcon->setPixmap(QPixmap(":/exts/nunchuk.png"));
}

void DeviceWidget::dbusClassicControllerPlugged(quint32) {
  ui->classicIcon->setPixmap(QPixmap(":/exts/controller_active.png"));
}

void DeviceWidget::dbusClassicControllerUnplugged(quint32) {
  ui->classicIcon->setPixmap(QPixmap(":/exts/controller.png"));
}


void DeviceWidget::runToolkit() {
  QStringList args;
  args << QString::number(id);
  process.start("wiimotedev-toolkit", args);
}

void DeviceWidget::startToolkit() {
  ui->runToolkit->setEnabled(false);
  QApplication::activeWindow()->hide();
}

void DeviceWidget::closeToolkit() {
  ui->runToolkit->setEnabled(true);
}
