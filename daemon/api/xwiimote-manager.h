#pragma once

#include <memory>
#include "interfaces/iwiimote-manager.h"

namespace daemon {
namespace api {

namespace helper {
class XWiimoteMonitor;
}

class XWiimoteManager final : public interface::IWiimoteManager {
public:
	explicit XWiimoteManager();
	virtual ~XWiimoteManager() = default;

	virtual interface::IWiimote::Api api() const override { return interface::IWiimote::Api::XWiimote; }
	virtual std::unique_ptr<interface::IWiimote> connect() override;

private:
	std::unique_ptr<helper::XWiimoteMonitor> m_monitor;
};
}
}