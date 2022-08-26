#pragma once

#include <memory>
#include <map>

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
	void process(u32 id, const dae::container::event &ev) final;

public:
	QList<uint> list() const;

private:
	std::map<uint, std::unique_ptr<functional::VirtualCursorProcessor>> m_processors;

signals:
	void dataChanged(uint id, double x, double y, double l, double a);
	void found(uint id);
	void lost(uint id);
};
}
}
