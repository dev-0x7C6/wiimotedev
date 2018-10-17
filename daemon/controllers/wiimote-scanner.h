#pragma once

#include "interfaces/iwiimote-api.h"

namespace dae::interface {
class IWiimoteManager;
}

namespace dae::controller {

class WiimoteScanner {
public:
	explicit WiimoteScanner(const interface::IWiimote::Api api);
	~WiimoteScanner();

	std::unique_ptr<interface::IWiimote> process();

private:
	std::unique_ptr<interface::IWiimoteManager> m_manager;
};
} // namespace dae::controller
