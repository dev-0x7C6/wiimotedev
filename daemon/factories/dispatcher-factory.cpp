#include "dispatcher-factory.h"
#include "dbus/balance-board-dispatcher.h"
#include "dbus/classic-dispatcher.h"
#include "dbus/nunchuk-dispatcher.h"
#include "dbus/pro-controller-dispatcher.h"
#include "dbus/wiimote-dispatcher.h"

using namespace service::dbus;
using namespace service::enums;
using namespace service::factory;
using namespace service::interface;

std::unique_ptr<IContainerProcessor> DispatcherFactory::create(const Device type) {
	switch (type) {
		case Device::BalanceBoard: return std::make_unique<BalanceBoardDispatcher>();
		case Device::Classic: return std::make_unique<ClassicDispatcher>();
		case Device::Nunchuk: return std::make_unique<NunchukDispatcher>();
		case Device::ProController: return std::make_unique<ProControllerDispatcher>();
		case Device::Wiimote: return std::make_unique<WiimoteDispatcher>();
	}

	return nullptr;
}
