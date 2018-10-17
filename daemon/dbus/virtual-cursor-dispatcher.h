#pragma once

#include <memory>
#include <map>
#include <QSet>

#include "interfaces/icontainer-processor.h"

namespace dae {
namespace functional {
class VirtualCursorProcessor;
}

namespace dbus {

class VirtualCursorDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit VirtualCursorDispatcher(EventCallback &&eventCallback);
	~VirtualCursorDispatcher() final;

	virtual enums::Adaptor type() const override;
	virtual void process(const common::enums::Device device, const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> list() const;

private:
	QSet<uint> m_ids;
	std::map<uint, std::unique_ptr<functional::VirtualCursorProcessor>> m_processors;

signals:
	void dataChanged(uint id, double x, double y, double l, double a);
	void found(uint id);
	void lost(uint id);
};
}
}
