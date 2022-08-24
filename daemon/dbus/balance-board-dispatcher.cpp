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

void BalanceBoardDispatcher::process(const u32 id, const dae::container::structs::event &ev) {
	if (Device::BalanceBoard != ev.first)
		return;

	//	auto process_pressure = [this, id, &container]() {
	//		const auto data = static_cast<PressureContainer *>(container.get())->data();
	//		emit dataChanged(id, data.tl, data.tr, data.bl, data.br);
	//	};

	//	auto process_status = [this, id, &container]() {
	//		const auto state = static_cast<const StatusContainer *>(container.get())->state();

	//		if (state == StatusContainer::State::Connected) {
	//			m_ids.insert(id);
	//			emit connected(id);
	//		}

	//		if (state == StatusContainer::State::Disconnected) {
	//			m_ids.remove(id);
	//			emit disconnected(id);
	//		}
	//	};

	//	const auto event = container->event();

	//	if (event == Event::Pressure)
	//		process_pressure();
	//	else if (event == Event::Status)
	//		process_status();
}
