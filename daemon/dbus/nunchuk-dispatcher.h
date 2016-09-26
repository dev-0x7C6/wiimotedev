#pragma once

#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class NunchukDispatcher final : public interface::IContainerProcessor {
public:
	explicit NunchukDispatcher(QObject *parent = nullptr);

	QList<uint> nunchukList() const;

	virtual Type type() const;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;
};
}
}
