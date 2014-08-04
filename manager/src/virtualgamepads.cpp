#include "src/virtualgamepads.h"
#include "ui_virtualgamepads.h"

VirtualGamepads::VirtualGamepads(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::VirtualGamepads) {
  ui->setupUi(this);
}

VirtualGamepads::~VirtualGamepads() {
  delete ui;
}
