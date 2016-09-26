#pragma once

#include <atomic>

#include <QMutex>
#include <QMutexLocker>
#include <QThread>

#include "controllers/wiimote-scanner-thread.h"
//#include "dbus/deviceevents.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "wiimotedevconnection.h"
#include "wiimotedevsettings.h"
#include "interfaces/icontainer-processor.h"

namespace service {
namespace core {

class WiimotedevCore final : public QObject {
	Q_OBJECT
public:
	explicit WiimotedevCore(QObject *parent = nullptr);
	virtual ~WiimotedevCore() = default;

	uint32_t result;

	QHash<uint32_t, WiimotedevConnection *> threads;

	QList<uint> classicList() { return {}; }
	QList<uint> nunchukList() { return {}; }
	QList<uint> wiimoteList() { return {}; }
	QList<uint> balanceBoardList() { return {}; }

protected:
	virtual void timerEvent(QTimerEvent *event) override;

protected:
	void run();

private:
	service::controller::WiimoteScannerThread m_scanner;
	std::list<std::unique_ptr<service::interface::IWiimote>> m_devices;
	std::list<std::unique_ptr<service::interface::IContainerProcessor>> m_adaptors;

signals:
	void dbusReportUnregistredWiimote(QString);

signals:
	void wiimoteAccelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void wiimoteButtonDataChanged(uint id, qulonglong mask);
	void wiimoteConnected(uint id);
	void wiimoteDisconnected(uint id);
	void wiimoteInfraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

	// balance board
	void balanceBoardDataChanged(uint id, int top_left, int top_right, int bottom_left, int bottom_right);
	// motion plus
	void motionPlusDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ);
	// infrared
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
};
}
}
