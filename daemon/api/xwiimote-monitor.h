#pragma once

#include <memory>

struct xwii_monitor;

namespace daemon {
namespace api {

class XWiimoteMonitor final {
public:
	explicit XWiimoteMonitor();
	virtual ~XWiimoteMonitor();

	bool isValid() const;
	std::string get();

private:
	xwii_monitor *m_monitor;
	int m_fd;
};
}
}
