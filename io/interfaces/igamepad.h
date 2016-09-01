#pragma once

#include <cstdint>
#include <emulation/input-device.h>

struct ButtonMapping {
	constexpr ButtonMapping(uint64_t w, uint64_t u)
			: wiimotedev_btn(w)
			, uinput_btn(u) {}
	uint64_t wiimotedev_btn;
	uint64_t uinput_btn;
};

struct AxisMapping {
	constexpr AxisMapping(int32_t axis, int32_t max, int32_t min, int32_t raw_max, int32_t raw_min)
			: axis(axis)
			, max(max)
			, min(min)
			, raw_max(raw_max)
			, raw_min(raw_min) {}
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

	template <typename ButtonCfg, typename AxisCfg>
	bool configure_gamepad(const ButtonCfg &buttons, const AxisCfg &axis) {
		bool isValid = true;
		isValid &= set_ev_bit(EV_KEY) == 0;
		isValid &= set_ev_bit(EV_ABS) == 0;

		for (const auto &value : buttons)
			isValid &= set_key_bit(value.uinput_btn) == 0;

		for (const auto &value : axis) {
			isValid &= set_abs_bit(value.axis) == 0;
			set_range(value.axis, value.max, value.min);
		}

		return isValid;
	}

	template <typename Buttons>
	bool toggle_buttons(const Buttons &buttons, uint64_t button) {
		bool isValid = true;
		for (const auto &value : buttons)
			isValid &= report(EV_KEY, value.uinput_btn, (button & value.wiimotedev_btn) ? 1 : 0);
		return isValid;
	}
};
