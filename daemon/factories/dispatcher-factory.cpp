#include "dispatcher-factory.h"
#include "dbus/balance-board-dispatcher.h"
#include "dbus/classic-dispatcher.h"
#include "dbus/nunchuk-dispatcher.h"
#include "dbus/pro-controller-dispatcher.h"
#include "dbus/wiimote-dispatcher.h"

using namespace service::factory;
using namespace service::interface;
using namespace service::dbus;

std::unique_ptr<IContainerProcessor> DispatcherFactory::create(const IContainerProcessor::Type type) {
	switch (type) {
		case IContainerProcessor::Type::BalanceBoard: return std::make_unique<BalanceBoardDispatcher>();
		case IContainerProcessor::Type::Classic: return std::make_unique<ClassicDispatcher>();
		case IContainerProcessor::Type::Nunchuk: return std::make_unique<NunchukDispatcher>();
		case IContainerProcessor::Type::ProController: return std::make_unique<ProControllerDispatcher>();
		case IContainerProcessor::Type::Wiimote: return std::make_unique<WiimoteDispatcher>();
	}

	return nullptr;
}
