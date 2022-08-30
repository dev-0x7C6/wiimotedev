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

namespace cpp {
auto xwii_monitor_poll(xwii_monitor *monitor) -> std::string {
	std::string ret;
	auto cstr = ::xwii_monitor_poll(monitor);
	if (cstr != nullptr) {
		ret = std::string(cstr);
		free(cstr);
	}
	return ret;
}
}

std::string XWiimoteMonitor::dequeue() {
	return cpp::xwii_monitor_poll(m_monitor);
}
