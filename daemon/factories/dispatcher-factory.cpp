#include "dispatcher-factory.h"
#include "dbus/balance-board-dispatcher.h"
#include "dbus/classic-dispatcher.h"
#include "dbus/nunchuk-dispatcher.h"
#include "dbus/pro-controller-dispatcher.h"
#include "dbus/virtual-cursor-dispatcher.h"
#include "dbus/wiimote-dispatcher.h"

using namespace common::enums;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::factory;
using namespace dae::interface;

std::unique_ptr<IContainerProcessor> dae::factory::make_dispatcher(EventCallback &&eventCallback, enums::Adaptor type) noexcept {
	switch (type) {
		case Adaptor::BalanceBoard: return std::make_unique<BalanceBoardDispatcher>(std::move(eventCallback));
		case Adaptor::Classic: return std::make_unique<ClassicDispatcher>(std::move(eventCallback));
		case Adaptor::Nunchuk: return std::make_unique<NunchukDispatcher>(std::move(eventCallback));
		case Adaptor::ProController: return std::make_unique<ProControllerDispatcher>(std::move(eventCallback));
		case Adaptor::VirtualCursor: return std::make_unique<VirtualCursorDispatcher>(std::move(eventCallback));
		case Adaptor::Wiimote: return std::make_unique<WiimoteDispatcher>(std::move(eventCallback));
	}

	return nullptr;
}
