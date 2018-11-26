#pragma once

#include <memory>
#include "interfaces/iwiimote-manager.h"

namespace dae {
namespace factory {

std::unique_ptr<interface::IWiimoteManager> make_controller_manager(interface::IWiimote::Api type) noexcept;
}
}
