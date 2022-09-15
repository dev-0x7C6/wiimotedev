#include "wiimote-scanner.h"

#include <chrono>
#include <thread>

using namespace dae::controller;
using namespace dae::interface;
using namespace std::literals;

std::unique_ptr<IWiimote> WiimoteScanner::process() {
	auto controller = manager.connect();

	if (controller) {
		const auto id = controller->id();
		std::this_thread::sleep_for(200ms); // wait for led ready
		controller->setLedStatus(1, (id & 0x1) == 0x1);
		controller->setLedStatus(2, (id & 0x2) == 0x2);
		controller->setLedStatus(3, (id & 0x4) == 0x4);
		controller->setLedStatus(4, (id & 0x8) == 0x8);
	}

	return controller;
}
