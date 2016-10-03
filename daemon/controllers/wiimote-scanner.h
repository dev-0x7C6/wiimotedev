#pragma once

#include "interfaces/iwiimote-api.h"
#include "interfaces/iwiimote-manager.h"

namespace service {
namespace controller {

class WiimoteScanner {
public:
	explicit WiimoteScanner(const interface::IWiimote::Api api);
	virtual ~WiimoteScanner() = default;

	std::unique_ptr<interface::IWiimote> process();

private:
	std::unique_ptr<interface::IWiimoteManager> m_manager;
};
}
}
