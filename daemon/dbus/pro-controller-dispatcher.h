#pragma once

#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class ProControllerDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ProControllerDispatcher(QObject *parent = nullptr);

	virtual Type type() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

	QList<uint> procontrollerList();

signals:
	void procontrollerButtonDataChanged(uint id, qulonglong mask);
	void procontrollerConnected(uint id);
	void procontrollerDisconnected(uint id);
	void procontrollerStickDataChanged(uint id, int lx, int ly, int rx, int ry);
};
}
}
