#pragma once

namespace dae {
namespace enums {

enum class Adaptor {
	BalanceBoard,
	Classic,
	Devices,
	Nunchuk,
	ProController,
	VirtualCursor,
	Wiimote
};

constexpr auto name(const Adaptor &device) {
	switch (device) {
		case Adaptor::BalanceBoard: return "balanceboard";
		case Adaptor::Classic: return "classic";
		case Adaptor::Devices: return "devices";
		case Adaptor::Nunchuk: return "nunchuk";
		case Adaptor::ProController: return "procontroller";
		case Adaptor::Wiimote: return "wiimote";
		case Adaptor::VirtualCursor: return "virtualcursor";
	}

	return "";
}
}
}
