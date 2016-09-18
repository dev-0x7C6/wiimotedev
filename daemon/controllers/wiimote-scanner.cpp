#include "wiimote-scanner.h"

#include "factories/controller-manager-factory.h"

using namespace service::controller;
using namespace service::factory;
using namespace service::interface;

WiimoteScanner::WiimoteScanner(const IWiimote::Api api)
		: m_manager(ControllerManagerFactory::create(api)) {
}

std::unique_ptr<IWiimote> WiimoteScanner::process() {
	auto controller = m_manager->connect();

	if (controller) {
		const auto id = m_uniqueId.at(static_cast<std::size_t>(controller->type())).take();
		controller->setId(id);
		return controller;
	}

	return nullptr;
}
