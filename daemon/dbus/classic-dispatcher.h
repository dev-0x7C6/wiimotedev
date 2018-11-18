#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class ClassicDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ClassicDispatcher(EventCallback &&eventCallback);

	virtual enums::Adaptor type() const override;
	virtual void process(const common::enums::Device device, const u32 id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> list() const;

private:
	QSet<uint> m_ids;

signals:
	void buttonDataChanged(uint id, qulonglong mask);
	void connected(uint id);
	void disconnected(uint id);
	void stickDataChanged(uint id, int lx, int ly, int rx, int ry);
};
}
}
