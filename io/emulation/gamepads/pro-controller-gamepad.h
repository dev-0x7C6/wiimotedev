#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class ProControllerGamepad final : public interface::IGamepad {
public:
	explicit ProControllerGamepad(std::string &&name, u32 id);

	common::enums::Device type() const final;
};
}
}
}
