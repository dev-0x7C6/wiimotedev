#pragma once

#include <QSet>

#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class VirtualCursorDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit VirtualCursorDispatcher(QObject *parent = nullptr);
	virtual ~VirtualCursorDispatcher() = default;

	virtual enums::Adaptor type() const override;
	virtual void process(const common::enums::Device device, const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

	QList<uint> list() const;

private:
	QSet<uint> m_ids;

signals:
	void dataChanged(uint id, double x, double y, double l, double a);
	void found(uint id);
	void lost(uint id);
};
}
}
