#pragma once

#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class ProControllerDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ProControllerDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(u32 id, const dae::container::event &ev) final;

signals:
	void buttonDataChanged(uint id, qulonglong mask);
	void connectionChanged(uint id, bool connected);
	void stickDataChanged(uint id, int lx, int ly, int rx, int ry);
};
}
}
