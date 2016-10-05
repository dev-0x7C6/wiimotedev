#pragma once

#include <atomic>

#include "controllers/wiimote-scanner-thread.h"
#include "interfaces/icontainer-processor.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace core {

class WiimotedevCore final : public QObject {
	Q_OBJECT
public:
	explicit WiimotedevCore(QObject *parent = nullptr);
	virtual ~WiimotedevCore() = default;

protected:
	virtual void timerEvent(QTimerEvent *event) override;

private:
	dae::controller::WiimoteScannerThread m_scanner;
	std::list<std::unique_ptr<dae::interface::IWiimote>> m_devices;
	std::list<std::unique_ptr<dae::interface::IContainerProcessor>> m_adaptors;
};
}
}
