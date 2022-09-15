#include "wiimote-scanner-thread.h"

#include <chrono>
#include <iostream>
#include "daemon/controllers/wiimote-scanner.h"

using namespace dae::controller;
using namespace dae::interface;
using namespace std::literals;

WiimoteScannerThread::WiimoteScannerThread(const IWiimote::Api api)
		: m_api(api)
		, m_thread([this](auto &&v) { scan(std::forward<decltype(v)>(v)); })

{
}

void WiimoteScannerThread::scan(const std::atomic_bool &interrupted) {
	WiimoteScanner scan;

	while (!interrupted) {
		auto device = scan.process();

		if (device) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_devices.emplace_back(std::move(device));
			m_count++;
		}

		std::this_thread::sleep_for(250ms);
	}
}

void WiimoteScannerThread::merge(std::list<std::unique_ptr<IWiimote>> &list) {
	if (m_count == 0)
		return;

	std::lock_guard<std::mutex> lock(m_mutex);
	list.merge(m_devices);
	m_count = 0;
}
