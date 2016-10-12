#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class BalanceBoardGamepad final : public interface::IGamepad {
public:
	explicit BalanceBoardGamepad(const std::string &name, const uint32_t id);
	virtual ~BalanceBoardGamepad() = default;

	virtual common::enums::Device type() const override;
};
}
}
}
