#include "src/infraredsettings.h"
#include "ui_infraredsettings.h"

InfraredSettings::InfraredSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfraredSettings)
{
    ui->setupUi(this);
}

InfraredSettings::~InfraredSettings()
{
    delete ui;
}
