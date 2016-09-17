#pragma once

#include <list>

#include "functionals/unique-id.h"
#include "interfaces/iwiimote-api.h"
#include "interfaces/iwiimote-manager.h"

namespace daemon {
namespace controller {

class WiimoteConnector {
public:
	explicit WiimoteConnector(const interface::IWiimote::Api api);
	virtual ~WiimoteConnector() = default;

	void process();

private:
	std::unique_ptr<interface::IWiimoteManager> m_manager;
	std::list<std::unique_ptr<interface::IWiimote>> m_controllers;
	std::array<functional::UniqueId<128>, 3> m_uniqueId;
};
}
}
