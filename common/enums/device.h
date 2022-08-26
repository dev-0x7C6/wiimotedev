#pragma once

#include <string>
#include <optional>

namespace common::enums {

enum class device {
	balance_board,
	classic_controller,
	motion_plus,
	nunchuk,
	pro_controller,
	wiimote,
};

constexpr static auto devices = {
	device::balance_board,
	device::classic_controller,
	device::motion_plus,
	device::nunchuk,
	device::pro_controller,
	device::wiimote,
};

namespace is {

constexpr auto wiimote(const device v) noexcept {
	return v == device::wiimote;
}

constexpr auto balance_board(const device v) noexcept {
	return v == device::balance_board;
}

constexpr auto motion_plus(const device v) noexcept {
	return v == device::motion_plus;
}

constexpr auto nunchuk(const device v) noexcept {
	return v == device::nunchuk;
}

constexpr auto classic_controller(const device v) noexcept {
	return v == device::classic_controller;
}

constexpr auto pro_controller(const device v) noexcept {
	return v == device::pro_controller;
}

}

constexpr auto to_string(const device &device) -> const char * {
	switch (device) {
		case device::balance_board: return "balanceboard";
		case device::classic_controller: return "classic";
		case device::nunchuk: return "nunchuk";
		case device::pro_controller: return "procontroller";
		case device::wiimote: return "wiimote";
		case device::motion_plus: return "motion_plus";
	}

	return "";
}

inline auto convert(const std::string &str) -> std::optional<device> {
	for (auto &&value : devices)
		if (to_string(value) == str)
			return value;

	return {};
}
}
