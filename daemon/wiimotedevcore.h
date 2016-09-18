#pragma once

#include <atomic>

#include <QMutex>
#include <QMutexLocker>
#include <QThread>

#include "controllers/wiimote-scanner-thread.h"
#include "dbus/daemonservice.h"
#include "dbus/deviceevents.h"
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
	WiimotedevDBusEventsWrapper *m_events;
	DBusServiceAdaptorWrapper *dbusServiceAdaptor;

	// Settings ------------------------------------------------- /
	WiimotedevSettings *settings;
	QHash<QString, uint> sequence;

	service::controller::WiimoteScannerThread m_scanner;
	std::list<std::unique_ptr<service::interface::IWiimote>> m_devices;

signals:
	void dbusReportUnregistredWiimote(QString);
};
}
}
