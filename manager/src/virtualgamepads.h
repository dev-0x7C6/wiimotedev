#ifndef VIRTUALGAMEPADS_H
#define VIRTUALGAMEPADS_H

#include <QMainWindow>

namespace Ui {
  class VirtualGamepads;
}

class VirtualGamepads : public QMainWindow {
  Q_OBJECT

public:
  explicit VirtualGamepads(QWidget *parent = 0);
  ~VirtualGamepads();

private:
  Ui::VirtualGamepads *ui;
};

#endif // VIRTUALGAMEPADS_H
