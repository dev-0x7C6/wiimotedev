#include "xwiimote-monitor.h"

#include <xwiimote.h>

using namespace service::api::helper;

XWiimoteMonitor::XWiimoteMonitor() {
	m_monitor = xwii_monitor_new(true, true);
}

XWiimoteMonitor::~XWiimoteMonitor() {
	xwii_monitor_unref(m_monitor);
}

bool XWiimoteMonitor::isValid() const {
	return m_monitor != nullptr;
}

std::string XWiimoteMonitor::dequeue() {
	auto cstring = xwii_monitor_poll(m_monitor);

	if (cstring == nullptr)
		return {};

	const std::string result(cstring);

	return result;
}
