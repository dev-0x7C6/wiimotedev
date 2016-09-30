#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class ClassicDispatcher final : public interface::IContainerProcessor {
public:
	explicit ClassicDispatcher(QObject *parent = nullptr);
	virtual ~ClassicDispatcher() = default;

	virtual enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> classicList() const;

private:
	QSet<uint> m_ids;
};
}
}
