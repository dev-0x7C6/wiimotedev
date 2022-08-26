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
	void process(u32 id, const dae::container::event &ev) final;
};
}
}
