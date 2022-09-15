#pragma once

#include "interfaces/iwiimote-api.h"
#include "api/xwiimote-manager.h"

namespace dae::controller {

class WiimoteScanner {
public:
	auto process() -> std::unique_ptr<interface::IWiimote>;

private:
	dae::api::XWiimoteManager manager;
};
} // namespace dae::controller
