#ifndef WIIMOTERAWSTREAM_H
#define WIIMOTERAWSTREAM_H

#include <QMainWindow>
#include <QGraphicsView>
#include "linux/usr/include/wiimotedev/deviceevents.h"
#include "infraredcameraview.h"

class QComboBox;
class QProgressBar;
class QTreeWidgetItem;

namespace Ui {
class WiimoteRawStream;
}

class WiimoteRawStream : public QMainWindow {
	Q_OBJECT

public:
	explicit WiimoteRawStream(WiimotedevDeviceEvents *iface, InfraredCameraView *graphics, QWidget *parent = 0);
	~WiimoteRawStream();

protected:
	void timerEvent(QTimerEvent *);

private:
	Ui::WiimoteRawStream *ui;
	WiimotedevDeviceEvents *m_interface;

	QComboBox *m_wiimoteComboBox;
	QProgressBar *m_wiimoteBatteryProgressBar;

	QTreeWidgetItem *m_accelerometerItems[2][5];
	QTreeWidgetItem *m_infraredItems[10];
	QTreeWidgetItem *m_wiimoteStatusItems[10];
	QTreeWidgetItem *m_wiimoteButtonItems[64];
	QTreeWidgetItem *m_stickItems[3];

	QAction *m_wiimoteLeds[4];
	QAction *m_wiimotePoints[4];
	QTreeWidgetItem *m_wiimoteLedItems[4];
	QAction *m_wiimoteRumble;
	QAction *m_classicConnected;
	QAction *m_nunchukConnected;
	QAction *m_cursorVisible;
	QList<QTreeWidgetItem *> opts;

	void updateWiimoteComboBox();

	uint32_t m_id;
	InfraredCameraView *m_mainWindow;

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
	void changeWiimote(uint32_t id);

	void dbusVirtualCursorPosition(uint32_t id, double x, double y, double size, double angle);
	void dbusVirtualCursorLost(uint32_t id);
	void dbusVirtualCursorFound(uint32_t id);
	void dbusWiimoteAcc(uint32_t id, const accdata &acc);
	void dbusWiimoteBatteryLife(uint32_t id, uint8_t life);
	void dbusWiimoteConnected(uint32_t id);
	void dbusWiimoteDisconnected(uint32_t id);
	void dbusWiimoteGeneralButtons(uint32_t id, uint64_t value);
	void dbusWiimoteInfrared(uint32_t id, const QList<struct irpoint> &points);
	void dbusWiimoteLedStatusChanged(uint32_t, uint8_t);
	void dbusWiimoteRumbleStatusChanged(uint32_t, uint8_t);
	void dbusNunchukPlugged(uint32_t id);
	void dbusNunchukUnplugged(uint32_t id);
	void dbusClassicPlugged(uint32_t id);
	void dbusClassicUnplugged(uint32_t id);
	void dbusNunchukAcc(uint32_t id, const accdata &acc);
	void dbusNunchukStick(uint32_t id, const stickdata &stick);
	void dbusClassicControllerLStick(uint32_t id, const stickdata &stick);
	void dbusClassicControllerRStick(uint32_t id, const stickdata &stick);
};

#endif // WIIMOTERAWSTREAM_H
