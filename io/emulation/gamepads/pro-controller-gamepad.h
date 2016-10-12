#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class ProControllerGamepad final : public interface::IGamepad {
public:
	explicit ProControllerGamepad(const std::string &name, const uint32_t id);
	virtual ~ProControllerGamepad() = default;

	virtual common::enums::Device type() const override;
};
}
}
}
