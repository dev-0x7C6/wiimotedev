#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <thread>

#include <QCoreApplication>
#include <QDBusConnection>
#include <QElapsedTimer>
#include <QFile>

#include "syslog/syslog.h"
#include "wiimotedevconnection.h"
#include "wiimotedevcore.h"
#include "wiimotedevdevice.h"

#include "containers/accelerometer-container.h"
#include "containers/infrared-container.h"
#include "containers/gyroscope-container.h"
#include "containers/pressure-container.h"
#include "factories/controller-manager-factory.h"
#include "interfaces/icontainer.h"
#include "factories/dispatcher-factory.h"

using namespace service::container;
using namespace service::controller;
using namespace service::core;
using namespace service::factory;
using namespace service::interface;
using namespace std::literals;

WiimotedevCore::WiimotedevCore(QObject *parent)
		: QObject(parent)
		, m_scanner(IWiimote::Api::XWiimote)
		, result(EXIT_SUCCESS)

{
	if (!QFile::exists(WIIMOTEDEV_SETTINGS_FILE)) {
		systemlog::critical(QString("missing configuration file %1").arg(WIIMOTEDEV_SETTINGS_FILE));
		result = EXIT_FAILURE;
		return;
	}

	const auto processors = {
		IContainerProcessor::BalanceBoard,
		IContainerProcessor::Classic,
		IContainerProcessor::Nunchuk,
		IContainerProcessor::Wiimote,
	};

	for (const auto &processor : processors)
		m_adaptors.emplace_back(DispatcherFactory::create(processor));

	QDBusConnection connection = QDBusConnection::systemBus();
	for (const auto &adaptor : m_adaptors)
		connection.registerObject("/" + QString::fromStdString(adaptor->interface()), adaptor.get());

	connection.registerService("org.wiimotedev.daemon");

	//connection.registerObject("/infrared")

	//	systemlog::notice(QString("config: %1").arg(WIIMOTEDEV_SETTINGS_FILE));
	//	settings = new WiimotedevSettings(this);
	//	sequence = settings->connectionTable();
	//	QDBusConnection connection = QDBusConnection::systemBus();
	//	m_events = new WiimotedevDBusEventsWrapper(this, connection);
	//	dbusServiceAdaptor = new DBusServiceAdaptorWrapper(this, connection);
	//	bool registred = connection.registerService(WIIMOTEDEV_DBUS_SERVICE_NAME);

	//	if (!(m_events->isRegistred() &&
	//			dbusServiceAdaptor->isRegistred() && registred)) {
	//		systemlog::critical("cannot dbus service, quit...");
	//		result = EXIT_FAILURE;
	//		return;
	//	}

	startTimer(1, Qt::PreciseTimer);
}

void WiimotedevCore::timerEvent(QTimerEvent *event) {
	static_cast<void>(event);

	m_scanner.merge(m_devices);

	for (const auto &device : m_devices) {
		const auto container = device->process();
		const auto id = device->id();

		if (!container)
			continue;

		for (const auto &adaptor : m_adaptors)
			adaptor->process(id, container);
	}
}
