#pragma once

#include <atomic>
#include <list>
#include <mutex>
#include <thread>

#include "interfaces/iwiimote-api.h"

namespace dae {
namespace controller {

class WiimoteScannerThread {
public:
	explicit WiimoteScannerThread(const interface::IWiimote::Api api);
	virtual ~WiimoteScannerThread();

	void merge(std::list<std::unique_ptr<interface::IWiimote>> &list);

private:
	const interface::IWiimote::Api m_api;
	std::atomic<bool> m_interrupted;
	std::atomic<int32_t> m_count{0};
	std::unique_ptr<std::thread> m_thread;
	std::mutex m_mutex;
	std::list<std::unique_ptr<interface::IWiimote>> m_devices;
};
}
}
