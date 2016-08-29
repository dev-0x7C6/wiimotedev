#include "emulation/gamepads/nunchuk-gamepad.h"

using namespace io::emulation::gamepad;

constexpr auto UINPUT_NUNCHUK_BTN_C = BTN_A;
constexpr auto UINPUT_NUNCHUK_BTN_Z = BTN_B;
constexpr auto UINPUT_NUNCHUK_STICK_AXIS_X = ABS_X;
constexpr auto UINPUT_NUNCHUK_STICK_AXIS_Y = ABS_Y;
constexpr auto UINPUT_NUNCHUK_STICK_MAX = 0xFF - 0x15;
constexpr auto UINPUT_NUNCHUK_STICK_MIN = 0x00 + 0x15;

NunchukGamepad::NunchukGamepad(const std::string &name, const uint32_t id)
		: IGamepad(name, id) {
}

IGamepad::Type NunchukGamepad::type() const { return Type::Nunchuk; }

bool NunchukGamepad::input(const uint64_t buttons) {
	bool isValid = true;
	isValid &= report(EV_KEY, UINPUT_NUNCHUK_BTN_C, (buttons & NUNCHUK_BTN_C) ? 1 : 0);
	isValid &= report(EV_KEY, UINPUT_NUNCHUK_BTN_Z, (buttons & NUNCHUK_BTN_Z) ? 1 : 0);
	isValid &= sync();
	return isValid;
}

bool NunchukGamepad::input(const NunchukGamepad::Stick stick, const int32_t x, const int32_t y) {
	if (Stick::NunchukStick == stick) {
		m_last_stick_x = std::max(UINPUT_NUNCHUK_STICK_MIN, std::min(x, UINPUT_NUNCHUK_STICK_MAX));
		m_last_stick_y = std::max(UINPUT_NUNCHUK_STICK_MIN, std::min(0xFF - y, UINPUT_NUNCHUK_STICK_MAX));
	}
	return syncSticks();
}

bool NunchukGamepad::centerStick(Stick id) {
	switch (id) {
		case Stick::NunchukStick:
			m_last_stick_x = (UINPUT_NUNCHUK_STICK_MAX - UINPUT_NUNCHUK_STICK_MIN) / 2;
			m_last_stick_y = (UINPUT_NUNCHUK_STICK_MAX - UINPUT_NUNCHUK_STICK_MIN) / 2;
			break;

		default:
			return false;
	}

	return true;
}

bool NunchukGamepad::syncSticks() {
	bool isValid = true;
	isValid &= report(EV_ABS, UINPUT_NUNCHUK_STICK_AXIS_X, m_last_stick_x);
	isValid &= report(EV_ABS, UINPUT_NUNCHUK_STICK_AXIS_Y, m_last_stick_y);
	isValid &= sync();
	return isValid;
}

bool NunchukGamepad::configure() {
	bool isValid = true;
	isValid &= set_ev_bit(EV_KEY);
	isValid &= set_ev_bit(EV_MSC);
	isValid &= set_ev_bit(EV_ABS);

	isValid &= set_key_bit(UINPUT_NUNCHUK_BTN_C);
	isValid &= set_key_bit(UINPUT_NUNCHUK_BTN_Z);

	isValid &= set_abs_bit(UINPUT_NUNCHUK_STICK_AXIS_X);
	isValid &= set_abs_bit(UINPUT_NUNCHUK_STICK_AXIS_Y);
	set_range(UINPUT_NUNCHUK_STICK_AXIS_X, UINPUT_NUNCHUK_STICK_MAX, UINPUT_NUNCHUK_STICK_MIN);
	set_range(UINPUT_NUNCHUK_STICK_AXIS_Y, UINPUT_NUNCHUK_STICK_MAX, UINPUT_NUNCHUK_STICK_MIN);

	return isValid;
}

bool NunchukGamepad::centerAllAxis() {
	centerStick(Stick::NunchukStick);
	syncSticks();
}
