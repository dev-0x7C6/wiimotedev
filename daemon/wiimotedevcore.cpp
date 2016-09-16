#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <thread>

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QFile>

#include "syslog/syslog.h"
#include "wiimotedevconnection.h"
#include "wiimotedevcore.h"
#include "wiimotedevdevice.h"

#include "factories/controller-manager-factory.h"
#include "functionals/unique-id.h"

using namespace daemon::factory;
using namespace daemon::functional;
using namespace daemon::interface;
using namespace std::literals;

WiimotedevCore::WiimotedevCore(QObject *parent)
		: QThread(parent)
		, WiimotedevDBusEvents(0)
		, dbusServiceAdaptor(0)
		, m_interrupted(false)
		, result(EXIT_SUCCESS) {
	if (!QFile::exists(WIIMOTEDEV_SETTINGS_FILE)) {
		systemlog::critical(QString("missing configuration file %1").arg(WIIMOTEDEV_SETTINGS_FILE));
		result = EXIT_FAILURE;
		return;
	}

	systemlog::notice(QString("config: %1").arg(WIIMOTEDEV_SETTINGS_FILE));
	settings = new WiimotedevSettings(this);
	sequence = settings->connectionTable();
	QDBusConnection connection = QDBusConnection::systemBus();
	WiimotedevDBusEvents = new WiimotedevDBusEventsWrapper(this, connection);
	dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
	bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

	if (!(WiimotedevDBusEvents->isRegistred() &&
			dbusServiceAdaptor->isRegistred() && registred)) {
		systemlog::critical("cannot dbus service, quit...");
		result = EXIT_FAILURE;
		return;
	}
}

WiimotedevCore::~WiimotedevCore() {
}

void WiimotedevCore::interrupt() { m_interrupted = true; }

void WiimotedevCore::run() {
	UniqueId<256> unique;

	auto manager = ControllerManagerFactory::create(ApiType::XWiimote);
	std::list<std::unique_ptr<IWiimote>> devices;

	while (!m_interrupted) {
		auto controller = manager->connect();

		if (controller) {
			const auto id = unique.take();
			devices.emplace_back(std::move(controller));
		}

		std::this_thread::sleep_for(1s);
	}
}
//	if (result == EXIT_FAILURE) {
//		QCoreApplication::quit();
//		return;
//	}

//	WiimotedevDevice *dev = new WiimotedevDevice();
//	QElapsedTimer m_bluetoothInertia;
//	QMutex m_bluetoothBlocking;
//	uint32_t id = 0;
//	m_bluetoothBlocking.lock();

//	while (!m_interrupted) {
//		m_bluetoothInertia.start();

//		if (dev->connectToDevice(1)) {
//			id = sequence.value(dev->getWiimoteSAddr(), 0);

//			if (!id)
//				if ((id = settings->registerWiiremote(dev->getWiimoteSAddr()))) {
//					systemlog::information(QString("note: wiiremote %1 registred, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));
//					sequence = settings->connectionTable();
//				}

//			systemlog::information(QString("wiiremote %1 connected, id %2").arg(dev->getWiimoteSAddr(), QString::number(id)));
//			WiimotedevConnection *thread = new WiimotedevConnection(dev, id);
//			thread->setPowerSafeTimeout(settings->powerSaveTiemout() * 60000);
//			connect(thread, &WiimotedevConnection::dbusVirtualCursorPosition, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusVirtualCursorPosition, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusVirtualCursorFound, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusVirtualCursorFound, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusVirtualCursorLost, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusVirtualCursorLost, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteGeneralButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteGeneralButtons, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteConnected, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteConnected, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteDisconnected, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteDisconnected, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteBatteryLife, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteBatteryLife, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteButtons, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteInfrared, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteInfrared, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteAcc, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusWiimoteAcc, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusNunchukPlugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukPlugged, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusNunchukUnplugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukUnplugged, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusNunchukStick, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukStick, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusNunchukButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukButtons, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusNunchukAcc, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusNunchukAcc, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusClassicControllerPlugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerPlugged, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusClassicControllerUnplugged, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerUnplugged, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusClassicControllerButtons, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerButtons, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusClassicControllerLStick, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerLStick, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusClassicControllerRStick, WiimotedevDBusEvents, &WiimotedevDBusEventsWrapper::dbusClassicControllerRStick, Qt::QueuedConnection);
//			connect(thread, &WiimotedevConnection::dbusWiimoteDisconnected, this, &WiimotedevCore::dbusWiimoteDisconnected, Qt::QueuedConnection);
//			threads.insert(id, thread);
//			thread->start();
//			dev = new WiimotedevDevice();
//		}

//		if (m_bluetoothInertia.elapsed() < WiimotedevCore::BluetoothFlood && !m_interrupted) {
//			for (int i = 0; i < 100; ++i) {
//				m_bluetoothBlocking.tryLock(WiimotedevCore::WaitForBluetooth / 100);

//				if (m_interrupted)
//					break;
//			}
//		}

//		sched_yield();
//	}

//	m_bluetoothBlocking.unlock();
//	dev->disconnectFromDevice();
//	delete dev;
//	foreach (WiimotedevConnection *thread, threads.values()) {
//		systemlog::information(QString("wiiremote %1 disconnecting..., id %2").arg(thread->dbusWiimoteGetMacAddress(), QString::number(id)));
//		thread->interrupt();
//		thread->wait();
//		delete thread;
//	}
//	threads.clear();
//	QCoreApplication::quit();

void WiimotedevCore::dbusWiimoteDisconnected(quint32 id) {
	WiimotedevConnection *thread = threads.value(id, 0);

	if (!thread)
		return;

	QString addr = thread->dbusWiimoteGetMacAddress();
	thread->interrupt();
	thread->wait();
	delete thread;
	threads.remove(id);
	systemlog::information(QString("wiiremote %1 disconnected., id %2").arg(addr, QString::number(id)));
}

bool WiimotedevCore::dbusReloadSequenceList() {
	systemlog::notice(QString("loading sequences from %1").arg(WIIMOTEDEV_SETTINGS_FILE));
	settings->reload();
	sequence = settings->connectionTable();
	return true;
}

QList<uint> WiimotedevCore::dbusGetWiimoteList() {
	QList<uint> list;

	for (const WiimotedevConnection *thread : threads.values())
		list << thread->id();

	qSort(list);
	return list;
}

QList<uint> WiimotedevCore::dbusNunchukGetAccelerometrCalibration(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusNunchukGetAccelerometrCalibration();

	return {};
}

QList<uint> WiimotedevCore::dbusWiimoteGetAccelerometrCalibration(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetAccelerometrCalibration();

	return {};
}

uint8_t WiimotedevCore::dbusWiimoteGetStatus(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetStatus();

	return 0;
}

uint8_t WiimotedevCore::dbusWiimoteGetLedStatus(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetLedStatus();

	return 0;
}

bool WiimotedevCore::dbusWiimoteGetRumbleStatus(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetRumbleStatus();

	return false;
}

bool WiimotedevCore::dbusWiimoteSetLedStatus(quint32 id, quint32 status) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteSetLedStatus(status);

	return false;
}

bool WiimotedevCore::dbusWiimoteSetRumbleStatus(quint32 id, bool status) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteSetRumbleStatus(status);

	return false;
}

quint32 WiimotedevCore::dbusWiimoteGetCurrentLatency(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetCurrentLatency();

	return 0;
}

quint32 WiimotedevCore::dbusWiimoteGetAverageLatency(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetCurrentLatency();

	return 0;
}

QString WiimotedevCore::dbusWiimoteGetMacAddress(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetMacAddress();

	return QString();
}

bool WiimotedevCore::dbusIsClassicConnected(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusIsClassicConnected();

	return false;
}

bool WiimotedevCore::dbusIsNunchukConnected(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusIsNunchukConnected();

	return false;
}

bool WiimotedevCore::dbusIsWiimoteConnected(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusIsWiimoteConnected();

	return false;
}

quint32 WiimotedevCore::dbusWiimoteGetBatteryLife(quint32 id) {
	WiimotedevConnection *thread = threads.value(id);

	if (thread)
		return threads.value(id)->dbusWiimoteGetBatteryLife();

	return false;
}
