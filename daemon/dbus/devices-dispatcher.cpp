#include "devices-dispatcher.h"
#include "devicesadaptor.h"

using namespace common::enums;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::interface;

DevicesDispatcher::DevicesDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new DevicesAdaptor(this);
}

Adaptor DevicesDispatcher::type() const {
	return Adaptor::Devices;
}

void DevicesDispatcher::process(const Device device, const u32 id, const dae::container::structs::event &ev) {
}
