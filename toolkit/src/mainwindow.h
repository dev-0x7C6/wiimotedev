#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wiimoterawstream.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  WiimoteRawStream *m_wiimoteRawStreamWidget;
public:
  explicit MainWindow(WiimoteRawStream *stream, QWidget *parent = 0);
  ~MainWindow();
  
private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
