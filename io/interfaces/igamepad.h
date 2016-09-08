#pragma once

#include <emulation/input-device.h>
#include <container/stick-container.h>

namespace io {
namespace emulation {
namespace gamepad {

class IGamepad : public InputDevice {
public:
	explicit IGamepad(const std::string &name, const uint32_t id, ButtonMap &&buttons, AxisMap &&axises);
	virtual ~IGamepad() = default;

	enum class Type {
		Classic,
		Nunchuk,
		Wiimote
	};

	virtual Type type() const = 0;
	virtual bool configure() override;

	virtual bool input(const uint64_t buttons);
	virtual bool input(const Stick stick, const int32_t x, const int32_t y);

protected:
	ButtonMap m_buttons;
	AxisMap m_axises;
};
}
}
}