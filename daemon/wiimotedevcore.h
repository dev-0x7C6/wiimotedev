#pragma once

#include <atomic>

#include "controllers/wiimote-scanner-thread.h"
#include "interfaces/icontainer-processor.h"
#include "include/wiimotedev/wiimotedev"

namespace dae::core {

class WiimotedevCore final {
public:
	explicit WiimotedevCore();

	void process();

private:
	dae::controller::WiimoteScannerThread m_scanner;
	std::list<std::unique_ptr<dae::interface::IWiimote>> m_devices;
	std::list<std::unique_ptr<dae::interface::IContainerProcessor>> m_adaptors;
};

}
