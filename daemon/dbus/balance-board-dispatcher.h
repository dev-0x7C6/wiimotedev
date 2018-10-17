#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class BalanceBoardDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit BalanceBoardDispatcher(EventCallback &&eventCallback);

	virtual enums::Adaptor type() const override;
	virtual void process(const common::enums::Device device, const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> list();

signals:
	void connected(uint id);
	void dataChanged(uint id, int top_left, int top_right, int bottom_left, int bottom_right);
	void disconnected(uint id);

private:
	QSet<uint> m_ids;
};
}
}
