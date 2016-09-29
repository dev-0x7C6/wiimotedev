#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class BalanceBoardDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit BalanceBoardDispatcher(QObject *parent = nullptr);
	virtual ~BalanceBoardDispatcher() = default;

	virtual enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> balanceBoardList();

signals:
	void balanceBoardConnected(uint id);
	void balanceBoardDataChanged(uint id, int top_left, int top_right, int bottom_left, int bottom_right);
	void balanceBoardDisconnected(uint id);

private:
	QSet<uint> m_ids;
};
}
}
