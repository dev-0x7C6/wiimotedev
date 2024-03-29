#include "wiimotedevcore.h"

#include <iostream>
#include <QDBusConnection>

#include "factories/dispatcher-factory.h"

using namespace common::enums;
using namespace dae::controller;
using namespace dae::core;
using namespace dae::enums;
using namespace dae::factory;
using namespace dae::interface;

namespace {
constexpr auto processors = {
	Adaptor::BalanceBoard,
	Adaptor::Classic,
	Adaptor::Nunchuk,
	Adaptor::ProController,
	Adaptor::VirtualCursor,
	Adaptor::Wiimote,
};
}

WiimotedevCore::WiimotedevCore()
		: m_scanner(IWiimote::Api::XWiimote) {

	for (const auto &processor : processors)
		m_adaptors.emplace_back(make_dispatcher([this](auto &&command) -> CommandResult {
			return event(std::forward<decltype(command)>(command));
		},
			processor));

	auto connection = []() {
		if (getuid() == 0)
			return QDBusConnection::systemBus();

		return QDBusConnection::sessionBus();
	}();

	for (const auto &adaptor : m_adaptors)
		connection.registerObject("/" + QString::fromStdString(name(adaptor->type())), adaptor.get());

	connection.registerService("org.wiimotedev.daemon");
}

void WiimotedevCore::process() {
	m_scanner.merge(m_devices);
	m_devices.remove_if([](auto &&device) { return !device->isValid(); });

	for (const auto &device : m_devices)
		for (auto &&event : device->process())
			for (const auto &adaptor : m_adaptors)
				adaptor->process(device->id(), event);
}

CommandResult WiimotedevCore::event(CommandEvent &&event) noexcept {
	auto getLedStatus = [](const std::unique_ptr<IWiimote> &device, const CommandParameter &parameter) {
		const auto event = std::get<GetLedStateEvent>(parameter);
		return device->ledStatus(event.led_id);
	};

	auto setLedStatus = [](const std::unique_ptr<IWiimote> &device, const CommandParameter &parameter) {
		const auto event = std::get<SetLedStateEvent>(parameter);
		return device->setLedStatus(event.led_id, event.state);
	};

	auto setRumbleStatus = [](const std::unique_ptr<IWiimote> &device, const CommandParameter &parameter) {
		const auto event = std::get<SetRumbleStateEvent>(parameter);
		return device->setRumbleStatus(event.state);
	};

	for (const auto &device : m_devices) {
		const auto id = device->id();
		if (id == event.id())
			switch (event.command()) {
				case CommandType::GetLedState: return getLedStatus(device, event.parameter());
				case CommandType::SetLedState: return setLedStatus(device, event.parameter());
				case CommandType::GetRumbleState: return device->rumbleStatus();
				case CommandType::SetRumbleState: return setRumbleStatus(device, event.parameter());
			}
	}

	return {};
}
