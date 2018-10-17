#include "wiimotedevcore.h"

#include <iostream>
#include <QDBusConnection>

#include "factories/controller-manager-factory.h"
#include "factories/dispatcher-factory.h"
#include "interfaces/icontainer.h"

using namespace common::enums;
using namespace dae::controller;
using namespace dae::core;
using namespace dae::enums;
using namespace dae::factory;
using namespace dae::interface;

WiimotedevCore::WiimotedevCore()
		: m_scanner(IWiimote::Api::XWiimote) {
	const auto processors = {
		Adaptor::BalanceBoard,
		Adaptor::Classic,
		Adaptor::Nunchuk,
		Adaptor::ProController,
		Adaptor::VirtualCursor,
		Adaptor::Wiimote,
	};

	for (const auto &processor : processors)
		m_adaptors.emplace_back(DispatcherFactory::create([](auto &&) -> CommandResult {
			return false; //TODO
		},
			processor));

	QDBusConnection connection = QDBusConnection::systemBus();
	for (const auto &adaptor : m_adaptors)
		connection.registerObject("/" + QString::fromStdString(name(adaptor->type())), adaptor.get());

	connection.registerService("org.wiimotedev.daemon");
}

void WiimotedevCore::process() {
	m_scanner.merge(m_devices);
	std::unique_ptr<IContainer> container;

	for (const auto &device : m_devices) {
		while ((container = device->process())) {
			for (const auto &adaptor : m_adaptors)
				adaptor->process(device->type(), device->id(), container);
		}

		if (!device->isValid()) {
			m_devices.remove_if([&device](const auto &value) { return value.get() == device.get(); });
			return;
		}
	}
}
