#pragma once

#include <string>

namespace common::enums {

enum class Device {
	BalanceBoard,
	Classic,
	Nunchuk,
	ProController,
	Wiimote,
	Last,
};

constexpr static auto device_list = {
	Device::BalanceBoard,
	Device::Classic,
	Device::Nunchuk,
	Device::ProController,
	Device::Wiimote,
};

constexpr auto name(const Device &device) {
	switch (device) {
		case Device::BalanceBoard: return "balanceboard";
		case Device::Classic: return "classic";
		case Device::Nunchuk: return "nunchuk";
		case Device::ProController: return "procontroller";
		case Device::Wiimote: return "wiimote";
		case Device::Last: return "";
	}

	return "";
}

inline auto convert(const std::string &str) -> Device {
	for (auto &&value : device_list)
		if (name(value) == str)
			return value;

	return Device::Last;
}
}
