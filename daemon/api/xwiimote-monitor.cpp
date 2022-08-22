#include "xwiimote-monitor.h"

#include <xwiimote-ng.h>

using namespace dae::api::helper;

XWiimoteMonitor::XWiimoteMonitor()
		: m_monitor(xwii_monitor_new(true, true)) {
}

XWiimoteMonitor::~XWiimoteMonitor() noexcept {
	if (isValid())
		xwii_monitor_unref(m_monitor);
}

bool XWiimoteMonitor::isValid() const {
	return m_monitor != nullptr;
}

std::string XWiimoteMonitor::dequeue() {
	auto cstring = xwii_monitor_poll(m_monitor);

	if (cstring == nullptr)
		return {};

	return cstring;
}
