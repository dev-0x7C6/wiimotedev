#include "icontainer-processor.h"

using namespace service::interface;

IContainerProcessor::IContainerProcessor(QObject *parent)
		: QObject(parent) {}

std::string IContainerProcessor::interface() const {
	auto type2str = [](const IContainerProcessor::Type type) -> std::string {
		switch (type) {
			case BalanceBoard: return "balanceboard";
			case Classic: return "classic";
			case Nunchuk: return "nunchuk";
			case ProController: return "proController";
			case Wiimote: return "wiimote";
		}

		return {};
	};

	return type2str(type());
}
