#pragma once

#include "interfaces/icontainer-processor.h"
#include "containers/structs.hpp"

namespace dae {
namespace dbus {

class ClassicDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ClassicDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(u32 id, const dae::container::event &ev) final;

public:
	QList<uint> list() const;

signals:
	void buttonDataChanged(uint id, qulonglong mask);
	void connectionChanged(uint id, bool connected);
	void stickDataChanged(uint id, int lx, int ly, int rx, int ry);
};
}
}
