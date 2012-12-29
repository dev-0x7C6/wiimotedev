#ifndef TOOLKITMAINWINDOW_H
#define TOOLKITMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "interfaces/deviceevents.h"

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
  explicit ToolkitMainWindow(DBusDeviceEventsInterface *iface, QGraphicsView *graphics, QWidget *parent = 0);
  ~ToolkitMainWindow();
  
private:
  Ui::ToolkitMainWindow *ui;
  DBusDeviceEventsInterface *m_interface;

  QComboBox *m_wiimoteComboBox;
  QProgressBar *m_wiimoteBatteryProgressBar;

  QHash < uint32, bool> m_classicConnected;
  QHash < uint32, bool> m_nunchukConnected;

  QTreeWidgetItem* m_accelerometerItems[2][5];
  QTreeWidgetItem* m_infraredItems[10];
  QTreeWidgetItem* m_wiimoteStatusItems[10];
  QTreeWidgetItem* m_wiimoteButtonItems[64];

  QAction *m_wiimoteLeds[4];
  QAction *m_wiimoteRumble;

  void updateWiimoteComboBox();


private slots:
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
