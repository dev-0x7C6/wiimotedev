#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class WiimoteGamepad final : public IGamepad {
	Q_OBJECT
public:
	enum Position {
		GamepadHorizontal,
		GamepadVertical
	};

	enum Mode {
		DPadPositionConstant,
		DPadPositionSwitchable
	};

	enum Device {
		Nunchuk,
		Wiimote
	};

public:
	explicit WiimoteGamepad(const std::string &name, const uint32_t id, Mode mode = DPadPositionConstant, Position horizontal = GamepadVertical);

	virtual Type type() const override;

	void setHomeSwitchPosition(bool option);

	virtual bool input(const uint64_t buttons) override;
	virtual bool input(const Stick stick, const int32_t x, const int32_t y) override { return false; };

	virtual bool configure() override;
	virtual bool centerAllAxis() override;

private:
	void centerStick(Stick id);
	void syncSticks();

signals:
	void setLedState(uint32_t, uint32_t);

private:
	int m_last_stick_x;
	int m_last_stick_y;
	int m_last_dpad_x;
	int m_last_dpad_y;
	Position m_horizontal;
	Mode m_mode;
	bool m_home_pressed;
	bool m_home_switch_position;
};
}
}
}
