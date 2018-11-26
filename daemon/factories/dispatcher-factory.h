#pragma once

#include "interfaces/icontainer-processor.h"
#include "enums/adaptor.h"

namespace dae {
namespace factory {

std::unique_ptr<interface::IContainerProcessor> make_dispatcher(EventCallback &&eventCallback, enums::Adaptor type) noexcept;
}
}
