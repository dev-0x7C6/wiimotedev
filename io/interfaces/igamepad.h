#pragma once

#include <cstdint>
#include <emulation/input-device.h>

struct ButtonMapping {
	constexpr ButtonMapping(uint64_t w, uint64_t u) : wiimotedev_btn(w), uinput_btn(u) {}
	uint64_t wiimotedev_btn;
	uint64_t uinput_btn;
};

struct AxisMapping {
	constexpr AxisMapping(int32_t axis, int32_t max, int32_t min, int32_t raw_max, int32_t raw_min)
		: axis(axis), max(max), min(min), raw_max(raw_max), raw_min(raw_min) {}
	int32_t axis;
	int32_t max;
	int32_t min;
	int32_t raw_max;
	int32_t raw_min;
};


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
