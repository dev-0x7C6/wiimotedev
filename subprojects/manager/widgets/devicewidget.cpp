#include "devicewidget.h"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QString physicalAddress, quint32 id, QString extenstions, QWidget *parent) :
  QWidget(parent),
  physicalAddress(physicalAddress),
  id(id),
  exts(extenstions),
  ui(new Ui::DeviceWidget)
{
  ui->setupUi(this);
  ui->physicalAddress->setText(QString("Physical address: %1").arg(physicalAddress));
  ui->id->setText(QString("Ident number: %1").arg(id));
  ui->extensions->setText(QString("Extensions: %1").arg(exts));

  connect(ui->runToolkit, SIGNAL(clicked()), this, SLOT(runToolkit()));
  connect(&process, SIGNAL(finished(int)), this, SLOT(closeToolkit()));
  connect(&process, SIGNAL(started()), this, SLOT(startToolkit()));
}

DeviceWidget::~DeviceWidget()
{
  delete ui;
}

void DeviceWidget::runToolkit() {
  QStringList args;
  args << QString::number(id+1);
  process.start("wiimotedev-toolkit", args);
}

void DeviceWidget::startToolkit() {
  ui->runToolkit->setEnabled(false);
}

void DeviceWidget::closeToolkit() {
  ui->runToolkit->setEnabled(true);
}
