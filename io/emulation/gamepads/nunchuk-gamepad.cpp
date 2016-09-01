#include "emulation/gamepads/nunchuk-gamepad.h"

using namespace io::emulation::gamepad;

constexpr std::array<ButtonMapping, 2> BUTTONS{{
	{NUNCHUK_BTN_C, BTN_X},
	{NUNCHUK_BTN_Z, BTN_A},
}};

constexpr std::array<AxisMapping, 2> AXISES{{
	{ABS_X, 0xFF - 0x1F, 0x1F, 0xFF, 0x00},
	{ABS_Y, 0xFF - 0x1F, 0x1F, 0xFF, 0x00},
}};

NunchukGamepad::NunchukGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id) {
}

IGamepad::Type NunchukGamepad::type() const { return Type::Nunchuk; }

bool NunchukGamepad::input(const uint64_t buttons) {
	bool isValid = true;
	for (const auto &value : BUTTONS)
		isValid &= report(EV_KEY, value.uinput_btn, (buttons & value.wiimotedev_btn) ? 1 : 0);

	isValid &= sync();
	return isValid;
}

bool NunchukGamepad::input(const NunchukGamepad::Stick stick, const int32_t x, const int32_t y) {
	if (Stick::NunchukStick == stick) {
		m_last_stick_x = std::max(AXISES[0].min, std::min(x, AXISES[0].max));
		m_last_stick_y = std::max(AXISES[1].min, std::min(y ^ AXISES[0].raw_max, AXISES[0].max));
	}
	return syncSticks();
}

bool NunchukGamepad::centerStick(Stick id) {
	switch (id) {
		case Stick::NunchukStick:
			m_last_stick_x = AXISES[0].raw_max / 2;
			m_last_stick_y = AXISES[1].raw_max / 2;
			break;

		default:
			return false;
	}

	return true;
}

bool NunchukGamepad::syncSticks() {
	bool isValid = true;
	isValid &= report(EV_ABS, AXISES[0].axis, m_last_stick_x);
	isValid &= report(EV_ABS, AXISES[1].axis, m_last_stick_y);
	isValid &= sync();
	return isValid;
}

bool NunchukGamepad::configure() {
	return configure_gamepad<decltype(BUTTONS), decltype(AXISES)>(BUTTONS, AXISES);
}

bool NunchukGamepad::centerAllAxis() {
	centerStick(Stick::NunchukStick);
	syncSticks();
}
