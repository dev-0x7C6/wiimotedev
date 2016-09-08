#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class WiimoteGamepad final : public IGamepad {
public:
	explicit WiimoteGamepad(const std::string &name, const uint32_t id);
	virtual ~WiimoteGamepad() = default;

	virtual Type type() const override;
};
}
}
}