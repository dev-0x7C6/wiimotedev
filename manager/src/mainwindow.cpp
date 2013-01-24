#include "src/mainwindow.h"
#include "ui_mainwindow.h"

#include "../config.h"

MainWindow::MainWindow(WiimoteRawStream *stream, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_wiimoteRawStreamWidget(stream)
{
//  QPalette windowColor;
//  QBrush brush(QColor(255, 255, 255, 255));
//  brush.setStyle(Qt::SolidPattern);
//  windowColor.setBrush(QPalette::Active, QPalette::WindowText, brush);
//  QBrush brush1(QColor(0, 0, 0, 255));
//  brush1.setStyle(Qt::SolidPattern);
//  windowColor.setBrush(QPalette::Active, QPalette::Button, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Light, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Midlight, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Dark, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Mid, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Text, brush);
//  windowColor.setBrush(QPalette::Active, QPalette::BrightText, brush);
//  windowColor.setBrush(QPalette::Active, QPalette::ButtonText, brush);
//  windowColor.setBrush(QPalette::Active, QPalette::Base, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Window, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::Shadow, brush1);
//  windowColor.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
//  QBrush brush2(QColor(255, 255, 220, 255));
//  brush2.setStyle(Qt::SolidPattern);
//  windowColor.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
//  windowColor.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Button, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Light, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Text, brush);
//  windowColor.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
//  windowColor.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Base, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Window, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
//  windowColor.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
//  windowColor.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Button, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Light, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Text, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
//  windowColor.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Base, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Window, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
//  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
//  windowColor.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
 // setPalette(windowColor);
  ui->setupUi(this);

  ui->rawStream->layout()->addWidget(m_wiimoteRawStreamWidget);

  setWindowTitle("Wiimotedev Manager " + QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
                 QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
                 QString::number(WIIMOTEDEV_VERSION_PATCH));

}

MainWindow::~MainWindow()
{
  delete ui;
}
