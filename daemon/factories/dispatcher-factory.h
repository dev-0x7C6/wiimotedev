#pragma once

#include "interfaces/icontainer-processor.h"
#include "enums/adaptor.h"

namespace dae {
namespace factory {

class DispatcherFactory {
public:
	explicit DispatcherFactory() = delete;
	~DispatcherFactory() = delete;

	static std::unique_ptr<interface::IContainerProcessor> create(EventCallback &&eventCallback, enums::Adaptor type);
};
}
}
