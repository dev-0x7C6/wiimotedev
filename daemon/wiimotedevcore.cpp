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
		m_adaptors.emplace_back(DispatcherFactory::create([this](auto &&command) -> CommandResult {
			return event(std::forward<decltype(command)>(command));
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

CommandResult WiimotedevCore::event(CommandEvent &&event) noexcept {
	for (const auto &device : m_devices) {
		const auto id = device->id();
		if (id == event.id())
			switch (event.command()) {
				case CommandType::GetLedState: return device->ledStatus(std::get<uint32_t>(event.parameter()));
				case CommandType::SetLedState: return device->setLedStatus(1, std::get<bool>(event.parameter()));
				case CommandType::GetRumbleState: return device->rumbleStatus();
				case CommandType::SetRumbleState: return device->setRumbleStatus(std::get<bool>(event.parameter()));
			}
	}

	return {};
}
