#pragma once

#include <memory>
#include <unordered_map>

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

private:
	std::unordered_map<uint, std::unique_ptr<functional::VirtualCursorProcessor>> m_processors;

signals:
	void dataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible);
	void visibilityChanged(uint id, bool visible);
};
}
}
