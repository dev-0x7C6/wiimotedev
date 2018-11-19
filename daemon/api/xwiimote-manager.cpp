#include "xwiimote-manager.h"
#include "api/xwiimote-controller.h"
#include "api/xwiimote-monitor.h"

using namespace dae::api;
using namespace dae::api::helper;
using namespace dae::interface;

XWiimoteManager::XWiimoteManager()
		: m_monitor(std::make_unique<XWiimoteMonitor>()) {
}

std::unique_ptr<IWiimote> XWiimoteManager::connect() {
	if (!m_monitor->isValid())
		return nullptr;

	auto device = m_monitor->dequeue();

	if (device.empty())
		return nullptr;

	return std::make_unique<XWiimoteController>(m_idManager, std::move(device));
}
