#pragma once

#include "common/enums/device.h"
#include "functionals/unique-id.h"

#include <externals/common/types.hpp>

namespace dae {
namespace interface {

class IIdManager {
public:
	explicit IIdManager() = default;
	virtual ~IIdManager() = default;

	virtual u32 take(const common::enums::Device device) = 0;
	virtual u32 debt(const common::enums::Device device, const u32 index) = 0;
};
}
}
