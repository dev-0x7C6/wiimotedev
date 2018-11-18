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
	enums::Adaptor type() const override;
	void process(const common::enums::Device device, const u32 id, const std::unique_ptr<interface::IContainer> &container) override;
};
}
}
