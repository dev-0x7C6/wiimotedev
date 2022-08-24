#include "classic-dispatcher.h"

#include "classicadaptor.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "containers/stick-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

ClassicDispatcher::ClassicDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new ClassicAdaptor(this);
}

Adaptor ClassicDispatcher::type() const { return Adaptor::Classic; }
QList<uint> ClassicDispatcher::list() const { return m_ids.values(); }

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void ClassicDispatcher::process(const Device device, const u32 id, const dae::container::structs::event &ev) {
	if (Device::Classic != device)
		return;

	//    std::visit(overloaded {
	//      [&](dae::container::structs::status v) {
	//        if (v.is_connected) {
	//            m_ids.insert(id);
	//            emit connected(id);
	//        } else {
	//            m_ids.remove(id);
	//            emit disconnected(id);
	//        }
	//      }
	//     }, ev.second);

	//	auto process_key = [this, id, &container]() {
	//		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
	//		emit buttonDataChanged(id, state);
	//	};

	//	auto process_stick = [this, id, &container]() {
	//		const auto lx = static_cast<const StickContainer *>(container.get())->lx();
	//		const auto ly = static_cast<const StickContainer *>(container.get())->ly();
	//		const auto rx = static_cast<const StickContainer *>(container.get())->rx();
	//		const auto ry = static_cast<const StickContainer *>(container.get())->ry();
	//		emit stickDataChanged(id, lx, ly, rx, ry);
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

	//	if (event == Event::Button)
	//		process_key();
	//	else if (event == Event::Stick)
	//		process_stick();
	//	else if (event == Event::Status)
	//		process_status();
}
