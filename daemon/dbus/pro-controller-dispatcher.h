#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class ProControllerDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ProControllerDispatcher(QObject *parent = nullptr);
	virtual ~ProControllerDispatcher() = default;

	virtual enums::Adaptor type() const override;
	virtual void process(const common::enums::Device device, const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> list();

signals:
	void buttonDataChanged(uint id, qulonglong mask);
	void connected(uint id);
	void disconnected(uint id);
	void stickDataChanged(uint id, int lx, int ly, int rx, int ry);

private:
	QSet<uint> m_ids;
};
}
}
