#include "balance-board-dispatcher.h"
#include "balanceboardadaptor.h"

using namespace service::dbus;
using namespace service::interface;

BalanceBoardDispatcher::BalanceBoardDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new BalanceboardAdaptor(this);
}

IContainerProcessor::Type BalanceBoardDispatcher::type() const {
	return IContainerProcessor::Type::BalanceBoard;
}

void BalanceBoardDispatcher::process(const uint32_t id, const std::unique_ptr<service::interface::IContainer> &container) {
}

QList<uint> BalanceBoardDispatcher::balanceBoardList() {
	return {};
}
