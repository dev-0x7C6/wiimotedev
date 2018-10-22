#pragma once

#include <memory>
#include "interfaces/iwiimote-manager.h"

namespace dae {
namespace factory {

class ControllerManagerFactory {
public:
	explicit ControllerManagerFactory() = delete;
	~ControllerManagerFactory() = delete;

	static std::unique_ptr<interface::IWiimoteManager> create(const interface::IWiimote::Api type);
};
}
}
