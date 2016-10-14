#pragma once

#include "interfaces/icontainer-processor.h"
#include "enums/adaptor.h"

namespace dae {
namespace factory {

class DispatcherFactory {
public:
	explicit DispatcherFactory() = delete;
	virtual ~DispatcherFactory() = delete;

	static std::unique_ptr<interface::IContainerProcessor> create(const enums::Adaptor type);
};
}
}
