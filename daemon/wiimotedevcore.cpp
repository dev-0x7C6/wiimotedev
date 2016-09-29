#include "wiimotedevcore.h"

#include <iostream>
#include <QDBusConnection>

#include "factories/controller-manager-factory.h"
#include "factories/dispatcher-factory.h"
#include "interfaces/icontainer.h"

using namespace service::controller;
using namespace service::core;
using namespace service::factory;
using namespace service::interface;

WiimotedevCore::WiimotedevCore(QObject *parent)
		: QObject(parent)
		, m_scanner(IWiimote::Api::XWiimote) {
	const auto processors = {
		IContainerProcessor::BalanceBoard,
		IContainerProcessor::Classic,
		IContainerProcessor::Nunchuk,
		IContainerProcessor::ProController,
		IContainerProcessor::Wiimote,
	};

	for (const auto &processor : processors)
		m_adaptors.emplace_back(DispatcherFactory::create(processor));

	QDBusConnection connection = QDBusConnection::systemBus();
	for (const auto &adaptor : m_adaptors)
		connection.registerObject("/" + QString::fromStdString(adaptor->interface()), adaptor.get());

	connection.registerService("org.wiimotedev.daemon");
	startTimer(1, Qt::PreciseTimer);
}

void WiimotedevCore::timerEvent(QTimerEvent *event) {
	static_cast<void>(event);

	m_scanner.merge(m_devices);
	std::unique_ptr<IContainer> container;

	for (const auto &device : m_devices) {
		const auto id = device->id();

		while ((container = device->process())) {
			for (const auto &adaptor : m_adaptors)
				adaptor->process(id, container);

			if (!device->isValid()) {
				m_devices.remove_if([&device](const auto &value) { return value.get() == device.get(); });
				return;
			}
		}
	}
}
