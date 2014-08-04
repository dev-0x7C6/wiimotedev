#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wiimoterawstream.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
private:
  Ui::MainWindow *ui;
  WiimoteRawStream *m_wiimoteRawStreamWidget;

public:
  explicit MainWindow(WiimoteRawStream *stream, QWidget *parent = 0);
  ~MainWindow();
};

#endif // MAINWINDOW_H
