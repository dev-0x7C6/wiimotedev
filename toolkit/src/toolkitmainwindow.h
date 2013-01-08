#ifndef TOOLKITMAINWINDOW_H
#define TOOLKITMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "linux/usr/include/wiimotedev/deviceevents.h"
#include <src/mainwindow.h>

class QComboBox;
class QProgressBar;
class QTreeWidgetItem;

namespace Ui {
class ToolkitMainWindow;
}

class ToolkitMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ToolkitMainWindow(WiimotedevDeviceEvents *iface, MainWindow *graphics, QWidget *parent = 0);
  ~ToolkitMainWindow();

protected:
  void timerEvent(QTimerEvent *);

private:
  Ui::ToolkitMainWindow *ui;
  WiimotedevDeviceEvents *m_interface;

  QComboBox *m_wiimoteComboBox;
  QProgressBar *m_wiimoteBatteryProgressBar;

  QTreeWidgetItem* m_accelerometerItems[2][5];
  QTreeWidgetItem* m_infraredItems[10];
  QTreeWidgetItem* m_wiimoteStatusItems[10];
  QTreeWidgetItem* m_wiimoteButtonItems[64];
  QTreeWidgetItem* m_stickItems[3];

  QAction *m_wiimoteLeds[4];
  QAction *m_wiimotePoints[4];
  QTreeWidgetItem *m_wiimoteLedItems[4];
  QAction *m_wiimoteRumble;
  QAction *m_classicConnected;
  QAction *m_nunchukConnected;
  QAction *m_cursorVisible;
  QList < QTreeWidgetItem*> opts;

  void updateWiimoteComboBox();

  uint m_id;
  MainWindow *m_mainWindow;

private slots:
  void wiimoteComboBoxChanged(int);
  void toggleRumble();
  void toggleLed1();
  void toggleLed2();
  void toggleLed3();
  void toggleLed4();
  void pressedPoint1();
  void pressedPoint2();
  void pressedPoint3();
  void pressedPoint4();
  void pressedClassic();
  void pressedNunchuk();
  void pressedCursor();
  void changeWiimote(uint id);

  void dbusVirtualCursorPosition(uint id, double x, double y, double size, double angle);
  void dbusVirtualCursorLost(uint id);
  void dbusVirtualCursorFound(uint id);
  void dbusWiimoteAcc(uint id, const accdata &acc);
  void dbusWiimoteBatteryLife(uint id, uint8 life);
  void dbusWiimoteConnected(uint id);
  void dbusWiimoteDisconnected(uint id);
  void dbusWiimoteGeneralButtons(uint id, uint64 value);
  void dbusWiimoteInfrared(uint id, const QList< struct irpoint> &points);
  void dbusWiimoteLedStatusChanged(uint, uint8);
  void dbusWiimoteRumbleStatusChanged(uint, uint8);
  void dbusNunchukPlugged(uint id);
  void dbusNunchukUnplugged(uint id);
  void dbusClassicPlugged(uint id);
  void dbusClassicUnplugged(uint id);
  void dbusNunchukAcc(uint id, const accdata &acc);
  void dbusNunchukStick(uint id, const stickdata &stick);
  void dbusClassicControllerLStick(uint id, const stickdata &stick);
  void dbusClassicControllerRStick(uint id, const stickdata &stick);
};

#endif // TOOLKITMAINWINDOW_H
