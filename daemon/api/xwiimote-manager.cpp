#include "xwiimote-manager.h"
#include "api/xwiimote-controller.h"
#include "api/xwiimote-monitor.h"

using namespace service::api;
using namespace service::api::helper;
using namespace service::interface;

XWiimoteManager::XWiimoteManager()
		: m_monitor(std::make_unique<XWiimoteMonitor>()) {
}

std::unique_ptr<IWiimote> XWiimoteManager::connect() {
	if (!m_monitor->isValid())
		return nullptr;

	const auto device = m_monitor->dequeue();

	if (device.empty())
		return nullptr;

	return std::make_unique<XWiimoteController>(m_idManager, device);
}
