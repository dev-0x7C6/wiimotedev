#include "balance-board-dispatcher.h"
#include "balanceboardadaptor.h"
#include "containers/pressure-container.h"

using namespace service::dbus;
using namespace service::interface;
using namespace service::container;

BalanceBoardDispatcher::BalanceBoardDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new BalanceboardAdaptor(this);
}

BalanceBoardDispatcher::~BalanceBoardDispatcher() {
}

IContainerProcessor::Type BalanceBoardDispatcher::type() const {
	return IContainerProcessor::Type::BalanceBoard;
}

void BalanceBoardDispatcher::process(const uint32_t id, const std::unique_ptr<service::interface::IContainer> &container) {
	auto process_pressure = [this, id, &container]() {
		const auto data = static_cast<PressureContainer *>(container.get())->data();
		emit balanceBoardDataChanged(id, data.tl, data.tr, data.bl, data.br);
	};

	switch (container->type()) {
		case IContainer::Type::Pressure: return process_pressure();
	}
}

QList<uint> BalanceBoardDispatcher::balanceBoardList() {
	return {};
}
