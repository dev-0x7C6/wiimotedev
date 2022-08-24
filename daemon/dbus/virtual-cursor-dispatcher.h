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

	enums::Adaptor type() const final;
	void process(common::enums::Device device, u32 id, const dae::container::structs::event &ev) final;

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
