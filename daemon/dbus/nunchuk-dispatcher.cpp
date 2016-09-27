#include "nunchuk-dispatcher.h"

#include "nunchukadaptor.h"
#include "containers/button-container.h"

using namespace service::dbus;
using namespace service::interface;
using namespace service::container;

NunchukDispatcher::NunchukDispatcher(QObject *parent)
		: IContainerProcessor(parent) {
	new NunchukAdaptor(this);
}

QList<uint> NunchukDispatcher::nunchukList() const { return {}; }
IContainerProcessor::Type NunchukDispatcher::type() const { return Type::Nunchuk; }

void NunchukDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_key = [this, id, &container]() {
		if (container->source() != IContainer::Source::Nunchuk)
			return;

		emit nunchukButtonDataChanged(id, static_cast<ButtonContainer *>(container.get())->state());
	};

	switch (container->type()) {
		case IContainer::Type::Button: return process_key();
	}
}
