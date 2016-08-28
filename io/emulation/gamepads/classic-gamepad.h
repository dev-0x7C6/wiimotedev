#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class ClassicGamepad final : public IGamepad {
public:
	explicit ClassicGamepad(const std::string &name, const uint32_t id);

	enum Sticks {
		LeftStick,
		RightStick,
		DpadStick
	};

	virtual Type type() const override;

	virtual bool inputButtons(const uint64_t buttons) override;
	virtual bool inputStick(const Stick stick, const int32_t x, const int32_t y) override;
	virtual bool inputAccelerometer(const double pitch, const double roll) override;

	virtual bool configure() override;

private:
	void centerStick(Sticks stick);
	void syncSticks();

private:
	int32_t m_last_r_stick_x;
	int32_t m_last_r_stick_y;
	int32_t m_last_l_stick_x;
	int32_t m_last_l_stick_y;
	int32_t m_last_dpad_x;
	int32_t m_last_dpad_y;
};
}
}
}
