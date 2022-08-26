#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class WiimoteGamepad final : public io::interface::IGamepad {
public:
	explicit WiimoteGamepad(std::string &&name, u32 id);

	common::enums::device type() const final;
};
}
}
}
