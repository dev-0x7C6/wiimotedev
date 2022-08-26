#pragma once

#include "common/enums/device.h"
#include "functionals/unique-id.h"

#include <externals/common/types.hpp>

namespace dae {
namespace interface {

class IIdManager {
public:
	virtual ~IIdManager() = default;

	virtual u32 reserve(common::enums::device device) = 0;
	virtual u32 release(common::enums::device device, u32 index) = 0;
};
}
}
