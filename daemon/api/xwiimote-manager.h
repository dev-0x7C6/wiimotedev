#pragma once

#include <memory>
#include "controllers/wiimote-id-manager.h"
#include "interfaces/iwiimote-api.h"

namespace dae {
namespace api {

namespace helper {
class XWiimoteMonitor;
}

class XWiimoteManager {
public:
	explicit XWiimoteManager();
	~XWiimoteManager();

	auto connect() -> std::unique_ptr<interface::IWiimote>;

private:
	std::unique_ptr<helper::XWiimoteMonitor> m_monitor;
	controller::WiimoteIdManager m_idManager;
};
}
}
