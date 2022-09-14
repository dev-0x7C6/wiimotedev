#include "balance-board-dispatcher.h"
#include "balanceboardadaptor.h"

using namespace common::enums;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;
using namespace dae::container;

BalanceBoardDispatcher::BalanceBoardDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new BalanceboardAdaptor(this);
}

Adaptor BalanceBoardDispatcher::type() const { return Adaptor::BalanceBoard; }

void BalanceBoardDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (!is::balance_board(ev.first))
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](const dae::container::pressure &v) {
					   emit dataChanged(id, v.top.first, v.top.second, v.bottom.first, v.bottom.second);
				   },
				   [&](const dae::container::status &v) {
					   ids.set(id, v.is_connected);
					   if (!v.is_connected)
						   emit dataChanged(id, 0, 0, 0, 0);
					   emit connectionChanged(id, v.is_connected);
				   }},
		ev.second);
}
