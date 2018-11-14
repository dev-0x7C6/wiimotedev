#pragma once

#include "interfaces/igamepad.h"

namespace io {
namespace emulation {
namespace gamepad {

class ClassicGamepad final : public interface::IGamepad {
public:
	explicit ClassicGamepad(std::string &&name, u32 id);

	common::enums::Device type() const final;
};
}
}
}
