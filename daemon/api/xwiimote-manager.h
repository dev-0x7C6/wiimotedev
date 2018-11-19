#pragma once

#include <memory>
#include "interfaces/iwiimote-manager.h"
#include "controllers/wiimote-id-manager.h"

namespace dae {
namespace api {

namespace helper {
class XWiimoteMonitor;
}

class XWiimoteManager final : public interface::IWiimoteManager {
public:
	explicit XWiimoteManager();

	interface::IWiimote::Api api() const final {
		return interface::IWiimote::Api::XWiimote;
	}
	std::unique_ptr<interface::IWiimote> connect() final;

private:
	std::unique_ptr<helper::XWiimoteMonitor> m_monitor;
	controller::WiimoteIdManager m_idManager;
};
}
}
