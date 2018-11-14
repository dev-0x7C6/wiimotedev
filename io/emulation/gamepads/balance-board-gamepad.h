#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class BalanceBoardGamepad final : public interface::IGamepad {
public:
	explicit BalanceBoardGamepad(std::string &&name, u32 id);

	common::enums::Device type() const final;
};
}
}
}
