#pragma once

#include "common/enums/device.h"
#include "functionals/unique-id.h"

namespace dae {
namespace interface {

class IIdManager {
public:
	explicit IIdManager() = default;
	virtual ~IIdManager() = default;

	virtual uint32_t take(const common::enums::Device device) = 0;
	virtual uint32_t debt(const common::enums::Device device, const uint32_t index) = 0;
};
}
}
