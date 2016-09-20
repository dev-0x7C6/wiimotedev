#pragma once

#include <atomic>

#include <QMutex>
#include <QMutexLocker>
#include <QThread>

#include "controllers/wiimote-scanner-thread.h"
#include "dbus/daemonservice.h"
//#include "dbus/deviceevents.h"
#include "linux/usr/include/wiimotedev/consts.h"
#include "wiimotedevconnection.h"
#include "wiimotedevsettings.h"

namespace service {
namespace core {

class WiimotedevCore final : public QObject {
	Q_OBJECT
public:
	explicit WiimotedevCore(QObject *parent = nullptr);
	virtual ~WiimotedevCore();

	uint32_t result;

	QHash<uint32_t, WiimotedevConnection *> threads;

protected:
	virtual void timerEvent(QTimerEvent *event) override;

public slots:
	bool dbusIsClassicConnected(quint32 id);
	bool dbusIsNunchukConnected(quint32 id);
	bool dbusIsWiimoteConnected(quint32 id);
	QList<uint> dbusNunchukGetAccelerometrCalibration(quint32 id);
	QList<uint> dbusWiimoteGetAccelerometrCalibration(quint32 id);
	quint32 dbusWiimoteGetAverageLatency(quint32 id);
	quint32 dbusWiimoteGetBatteryLife(quint32 id);
	quint32 dbusWiimoteGetCurrentLatency(quint32 id);
	QString dbusWiimoteGetMacAddress(quint32 id);
	bool dbusWiimoteGetRumbleStatus(quint32 id);
	bool dbusWiimoteSetLedStatus(quint32 id, quint32 status);
	bool dbusWiimoteSetRumbleStatus(quint32 id, bool status);
	uint8_t dbusWiimoteGetLedStatus(quint32 id);
	uint8_t dbusWiimoteGetStatus(quint32 id);
	QList<uint> dbusGetWiimoteList();
	bool dbusReloadSequenceList();

protected:
	void run();

private slots:
	void dbusWiimoteDisconnected(quint32);

private:
	// Adaptor section ------------------------------------------ /
	DBusServiceAdaptorWrapper *dbusServiceAdaptor;

	service::controller::WiimoteScannerThread m_scanner;
	std::list<std::unique_ptr<service::interface::IWiimote>> m_devices;

signals:
	void dbusReportUnregistredWiimote(QString);

signals:
	void dbusClassicControllerButtons(uint id, qulonglong value);
	void dbusClassicControllerLStick(uint id, uchar x, uchar y);
	void dbusClassicControllerPlugged(uint id);
	void dbusClassicControllerRStick(uint id, uchar x, uchar y);
	void dbusClassicControllerUnplugged(uint id);
	void dbusNunchukAcc(uint id, uchar x, uchar y, uchar z, double pitch, double roll);
	void dbusNunchukButtons(uint id, qulonglong value);
	void dbusNunchukPlugged(uint id);
	void dbusNunchukStick(uint id, uchar x, uchar y);
	void dbusNunchukUnplugged(uint id);
	void dbusVirtualCursorFound(uint id);
	void dbusVirtualCursorLost(uint id);
	void dbusVirtualCursorPosition(uint id, double x, double y, double length, double angle);
	void dbusWiimoteAcc(uint id, uchar x, uchar y, uchar z, double pitch, double roll);
	void dbusWiimoteBatteryLife(uint id, uchar life);
	void dbusWiimoteButtons(uint id, qulonglong value);
	void dbusWiimoteConnected(uint id);
	//	void dbusWiimoteDisconnected(uint id);
	void dbusWiimoteGeneralButtons(uint id, qulonglong value);
	void dbusWiimoteInfrared(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void dbusWiimoteLedStatusChanged(uint id, uchar value);
	void dbusWiimoteRumbleStatusChanged(uint id, uchar value);
	void dbusWiimoteStatus(uint id, uchar status);
};
}
}
