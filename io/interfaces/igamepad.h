#pragma once

#include <cstdint>
#include <emulation/input-device.h>

class IGamepad : public InputDevice {
public:
	explicit IGamepad(const std::string &name, uint32_t id);

	enum class Type {
		ClassicController,
		Nunchuk,
		Wiimote
	};

	enum class Stick {
		ClassicControllerLStick,
		ClassicControllerRStick,
		NunchukStick,
		WiimoteDPad,
		WiimoteAccelerometer,
		NunchukAccelerometer,
	};

	virtual Type type() const = 0;

	virtual bool inputButtons(const uint64_t buttons) = 0;
	virtual bool inputStick(const Stick stick, const int32_t x, const int32_t y) = 0;
	virtual bool inputAccelerometer(const double pitch, const double roll) = 0;
};
