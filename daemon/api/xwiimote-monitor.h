#pragma once

#include <memory>
#include <string>

struct xwii_monitor;

namespace dae {
namespace api {
namespace helper {

class XWiimoteMonitor final {
public:
	explicit XWiimoteMonitor();
	~XWiimoteMonitor() noexcept;

	bool isValid() const;
	std::string dequeue();

private:
	xwii_monitor *m_monitor{nullptr};
};
}
}
}
