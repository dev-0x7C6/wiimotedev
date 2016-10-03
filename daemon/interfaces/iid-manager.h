#pragma once

#include "enums/device-type.h"
#include "functionals/unique-id.h"

namespace service {
namespace interface {

class IIdManager {
public:
	explicit IIdManager() = default;
	virtual ~IIdManager() = default;

	virtual uint32_t take(const enums::Device device) = 0;
	virtual uint32_t debt(const enums::Device device, const uint32_t index) = 0;
};
}
}
