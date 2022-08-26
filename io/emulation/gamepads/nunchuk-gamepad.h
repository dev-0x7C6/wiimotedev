#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class NunchukGamepad final : public interface::IGamepad {
public:
	explicit NunchukGamepad(std::string &&name, u32 id);

	common::enums::device type() const final;
};
}
}
}
