#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class NunchukGamepad final : public interface::IGamepad {
public:
	explicit NunchukGamepad(const std::string &name, const uint32_t id);
	virtual ~NunchukGamepad() = default;

	virtual Type type() const override;
};
}
}
}
