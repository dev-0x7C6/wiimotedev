#pragma once

#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class BalanceBoardDispatcher final : public interface::IContainerProcessor {
public:
	explicit BalanceBoardDispatcher(QObject *parent = nullptr);

	virtual Type type() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> balanceBoardList();
};
}
}
