#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class ClassicGamepad final : public interface::IGamepad {
public:
	explicit ClassicGamepad(const std::string &name, const uint32_t id);
	virtual ~ClassicGamepad() = default;

	virtual Type type() const override;
};
}
}
}
