#pragma once

#include <memory>

struct xwii_monitor;

namespace daemon {
namespace api {
namespace helper {

class XWiimoteMonitor final {
public:
	explicit XWiimoteMonitor();
	virtual ~XWiimoteMonitor();

	bool isValid() const;
	std::string dequeue();

private:
	xwii_monitor *m_monitor;
	int m_fd;
};
}
}
}
