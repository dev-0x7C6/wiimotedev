#include "pro-controller-dispatcher.h"
#include "procontrolleradaptor.h"

#include "containers/button-container.h"
#include "containers/status-container.h"
#include "containers/stick-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

ProControllerDispatcher::ProControllerDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new ProcontrollerAdaptor(this);
}

Adaptor ProControllerDispatcher::type() const { return Adaptor::ProController; }
QList<uint> ProControllerDispatcher::list() { return m_ids.toList(); }

void ProControllerDispatcher::process(const Device device, const uint32_t id, const std::unique_ptr<IContainer> &container) {
	if (Device::ProController != device)
		return;

	auto process_key = [this, id, &container]() {
		const auto state = static_cast<const ButtonContainer *>(container.get())->state();
		emit buttonDataChanged(id, state);
	};

	auto process_status = [this, id, &container]() {
		const auto state = static_cast<const StatusContainer *>(container.get())->state();

		if (state == StatusContainer::State::Connected) {
			m_ids.insert(id);
			emit connected(id);
		}

		if (state == StatusContainer::State::Disconnected) {
			m_ids.insert(id);
			emit disconnected(id);
		}
	};

	auto process_stick = [this, id, &container]() {
		const auto lx = static_cast<const StickContainer *>(container.get())->lx();
		const auto ly = static_cast<const StickContainer *>(container.get())->ly();
		const auto rx = static_cast<const StickContainer *>(container.get())->rx();
		const auto ry = static_cast<const StickContainer *>(container.get())->ry();
		emit stickDataChanged(id, lx, ly, rx, ry);
	};

	const auto event = container->event();

	if (event == Event::Status)
		return process_status();
	else if (event == Event::Button)
		return process_key();
	else if (event == Event::Stick)
		return process_stick();
}
