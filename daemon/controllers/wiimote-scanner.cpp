#include "wiimote-scanner.h"

#include <thread>
#include <chrono>
#include "factories/controller-manager-factory.h"

using namespace service::controller;
using namespace service::factory;
using namespace service::interface;
using namespace std::literals;

WiimoteScanner::WiimoteScanner(const IWiimote::Api api)
		: m_manager(ControllerManagerFactory::create(api)) {
}

std::unique_ptr<IWiimote> WiimoteScanner::process() {
	auto controller = m_manager->connect();

	if (controller) {
		const auto id = (controller->id() % 15) + 1;
		std::this_thread::sleep_for(200ms); // wait for led ready
		controller->setLedStatus(0, id & 0x1);
		controller->setLedStatus(1, id & 0x2);
		controller->setLedStatus(2, id & 0x4);
		controller->setLedStatus(3, id & 0x8);

		return controller;
	}

	return nullptr;
}
