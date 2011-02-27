#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QProcess>

namespace Ui {
  class DeviceWidget;
}


class DeviceWidget : public QWidget
{
  Q_OBJECT
  QProcess process;
  QString physicalAddress;
  quint32 id;
  QString exts;

public:
  explicit DeviceWidget(QString physicalAddress, quint32 id, QString extenstions, QWidget *parent = 0);
  ~DeviceWidget();

private:
  Ui::DeviceWidget *ui;

private Q_SLOTS:
  void runToolkit();
  void startToolkit();
  void closeToolkit();

};

#endif // DEVICEWIDGET_H
