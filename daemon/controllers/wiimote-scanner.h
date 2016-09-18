#pragma once

#include <list>

#include "functionals/unique-id.h"
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
	std::array<functional::UniqueId<128>, 3> m_uniqueId;
};
}
}
