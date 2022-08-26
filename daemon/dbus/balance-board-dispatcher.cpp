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
QList<uint> BalanceBoardDispatcher::list() { return m_ids.values(); }

void BalanceBoardDispatcher::process(const u32 id, const dae::container::event &ev) {
	if (!is::balance_board(ev.first))
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](const dae::container::pressure v) {
					   emit dataChanged(id, v.top.first, v.top.second, v.bottom.first, v.bottom.second);
				   },
				   [&](const dae::container::status v) {
					   if (v.is_connected)
						   m_ids.insert(id);
					   else
						   m_ids.remove(id);
					   emit connectionChanged(id, v.is_connected);
				   }},
		ev.second);
}
