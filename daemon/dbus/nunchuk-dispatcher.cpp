#include "nunchuk-dispatcher.h"

#include "nunchukadaptor.h"

using namespace service::dbus;
using namespace service::interface;

NunchukDispatcher::NunchukDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new NunchukAdaptor(this);
}

QList<uint> NunchukDispatcher::nunchukList() const { return {}; }

IContainerProcessor::Type NunchukDispatcher::type() const {
	return Type::Nunchuk;
}

void NunchukDispatcher::process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) {
}
