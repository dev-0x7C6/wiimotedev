#pragma once

#include <memory>
#include "interfaces/iwiimote-manager.h"

namespace daemon {
namespace factory {

class ControllerManagerFactory {
public:
	explicit ControllerManagerFactory() = delete;
	virtual ~ControllerManagerFactory() = delete;

	static std::unique_ptr<interface::IWiimoteManager> create(const interface::ApiType type);
};

}
}
