#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class ProControllerDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ProControllerDispatcher(QObject *parent = nullptr);

	virtual common::enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> procontrollerList();

signals:
	void procontrollerButtonDataChanged(uint id, qulonglong mask);
	void procontrollerConnected(uint id);
	void procontrollerDisconnected(uint id);
	void procontrollerStickDataChanged(uint id, int lx, int ly, int rx, int ry);

private:
	QSet<uint> m_ids;
};
}
}
