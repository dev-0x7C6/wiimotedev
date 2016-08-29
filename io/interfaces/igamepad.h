#pragma once

#include <cstdint>
#include <emulation/input-device.h>

class IGamepad : public InputDevice {
public:
	explicit IGamepad(const std::string &name, uint32_t id);

	enum class Type {
		Classic,
		Nunchuk,
		Wiimote
	};

	enum class Stick {
		ClassicDPad,
		ClassicLStick,
		ClassicRStick,
		NunchukStick,
		WiimoteDPad,
	};

	virtual Type type() const = 0;

	virtual bool input(const uint64_t buttons) = 0;
	virtual bool input(const Stick stick, const int32_t x, const int32_t y) = 0;

	virtual bool centerAllAxis() = 0;
};
