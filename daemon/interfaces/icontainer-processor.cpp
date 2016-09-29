#include "icontainer-processor.h"

using namespace service::interface;
using namespace service::enums;

IContainerProcessor::IContainerProcessor(QObject *parent)
		: QObject(parent) {}

std::string IContainerProcessor::interface() const {
	auto type2str = [](const Device device) -> std::string {
		switch (device) {
			case Device::BalanceBoard: return "balanceboard";
			case Device::Classic: return "classic";
			case Device::Nunchuk: return "nunchuk";
			case Device::ProController: return "procontroller";
			case Device::Wiimote: return "wiimote";
			case Device::Last: return {};
		}

		return {};
	};

	return type2str(device());
}
