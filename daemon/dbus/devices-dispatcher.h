#pragma once

#include <QObject>
#include <QStringList>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {
class DevicesDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit DevicesDispatcher(EventCallback &&eventCallback);

	QStringList list() { return {}; }

protected:
	enums::Adaptor type() const final;
	void process(common::enums::Device device, u32 id, const std::unique_ptr<interface::IContainer> &container) final;
};
}
}
