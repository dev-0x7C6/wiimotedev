#include "dispatcher-factory.h"
#include "dbus/balance-board-dispatcher.h"
#include "dbus/classic-dispatcher.h"
#include "dbus/nunchuk-dispatcher.h"
#include "dbus/pro-controller-dispatcher.h"
#include "dbus/wiimote-dispatcher.h"
#include "dbus/virtual-cursor-dispatcher.h"

using namespace common::enums;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::factory;
using namespace dae::interface;

std::unique_ptr<IContainerProcessor> DispatcherFactory::create(const Adaptor type) {
	switch (type) {
		case Adaptor::BalanceBoard: return std::make_unique<BalanceBoardDispatcher>();
		case Adaptor::Classic: return std::make_unique<ClassicDispatcher>();
		case Adaptor::Nunchuk: return std::make_unique<NunchukDispatcher>();
		case Adaptor::ProController: return std::make_unique<ProControllerDispatcher>();
		case Adaptor::VirtualCursor: return std::make_unique<VirtualCursorDispatcher>();
		case Adaptor::Wiimote: return std::make_unique<WiimoteDispatcher>();
	}

	return nullptr;
}
