#pragma once

#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class ClassicDispatcher final : public interface::IContainerProcessor {
public:
	explicit ClassicDispatcher(QObject *parent = nullptr);

	QList<uint> classicList() const;

	virtual enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;
};
}
}
