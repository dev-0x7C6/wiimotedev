#pragma once

#include "interfaces/icontainer-processor.h"

namespace dae {
namespace factory {

class DispatcherFactory {
public:
	explicit DispatcherFactory() = delete;
	virtual ~DispatcherFactory() = delete;

	static std::unique_ptr<interface::IContainerProcessor> create(const common::enums::Device type);
};
}
}
