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

  QPalette windowColor;
  QBrush brush(QColor(255, 255, 255, 255));
  brush.setStyle(Qt::SolidPattern);
  windowColor.setBrush(QPalette::Active, QPalette::WindowText, brush);
  QBrush brush1(QColor(0, 0, 0, 255));
  brush1.setStyle(Qt::SolidPattern);
  windowColor.setBrush(QPalette::Active, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Text, brush);
  windowColor.setBrush(QPalette::Active, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Active, QPalette::ButtonText, brush);
  windowColor.setBrush(QPalette::Active, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::Window, QBrush(QColor(43, 162, 246, 255)));
  windowColor.setBrush(QPalette::Active, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
  QBrush brush2(QColor(255, 255, 220, 255));
  QBrush brush3(QColor(100, 100, 100, 255));
  brush2.setStyle(Qt::SolidPattern);
  brush3.setStyle(Qt::SolidPattern);
  windowColor.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Text, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
  windowColor.setBrush(QPalette::Inactive, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::Window, QColor(43, 162, 246, 150));
  windowColor.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
  windowColor.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
  windowColor.setBrush(QPalette::Disabled, QPalette::Button, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Light, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Text, brush3);
  windowColor.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
  windowColor.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Base, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::Window, QBrush(QColor(43, 162, 246, 150)));
  windowColor.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
  ui->groupBox->setPalette(windowColor);

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
}

void DeviceWidget::closeToolkit() {
  ui->runToolkit->setEnabled(true);
}
