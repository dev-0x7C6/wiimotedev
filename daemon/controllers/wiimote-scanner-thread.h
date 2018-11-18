#pragma once

#include <atomic>
#include <list>
#include <mutex>
#include <thread>

#include "interfaces/iwiimote-api.h"
#include "externals/common/std/raii/raii-thread.hpp"

namespace dae::controller {

class WiimoteScannerThread {
public:
	explicit WiimoteScannerThread(const interface::IWiimote::Api api);

	void merge(std::list<std::unique_ptr<interface::IWiimote>> &list);

private:
	void scan(const std::atomic_bool &interrupted);

private:
	const interface::IWiimote::Api m_api;
	std::atomic<i32> m_count{0};
	raii_thread m_thread;
	std::mutex m_mutex;
	std::list<std::unique_ptr<interface::IWiimote>> m_devices;
};

}
