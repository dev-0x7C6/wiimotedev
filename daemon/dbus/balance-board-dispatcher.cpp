#include "balance-board-dispatcher.h"
#include "balanceboardadaptor.h"

#include "containers/pressure-container.h"
#include "containers/status-container.h"

using namespace common::enums;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;
using namespace dae::container;

BalanceBoardDispatcher::BalanceBoardDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new BalanceboardAdaptor(this);
}

Device BalanceBoardDispatcher::device() const { return Device::BalanceBoard; }
QList<uint> BalanceBoardDispatcher::balanceBoardList() { return m_ids.toList(); }

void BalanceBoardDispatcher::process(const uint32_t id, const std::unique_ptr<dae::interface::IContainer> &container) {
	auto process_pressure = [this, id, &container]() {
		const auto data = static_cast<PressureContainer *>(container.get())->data();
		emit balanceBoardDataChanged(id, data.tl, data.tr, data.bl, data.br);
	};

	auto process_status = [this, id, &container]() {
		const auto state = static_cast<const StatusContainer *>(container.get())->state();

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit balanceBoardConnected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.remove(id);
			emit balanceBoardDisconnected(id);
		}
	};

	switch (container->event()) {
		case Event::Pressure: return process_pressure();
		case Event::Status: return process_status();
	}
}
