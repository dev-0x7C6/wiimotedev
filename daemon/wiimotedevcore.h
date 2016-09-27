#pragma once

#include <atomic>

#include "controllers/wiimote-scanner-thread.h"
#include "interfaces/icontainer-processor.h"
#include "linux/usr/include/wiimotedev/consts.h"

namespace service {
namespace core {

class WiimotedevCore final : public QObject {
	Q_OBJECT
public:
	explicit WiimotedevCore(QObject *parent = nullptr);
	virtual ~WiimotedevCore() = default;

protected:
	virtual void timerEvent(QTimerEvent *event) override;

protected:
	void run();

private:
	service::controller::WiimoteScannerThread m_scanner;
	std::list<std::unique_ptr<service::interface::IWiimote>> m_devices;
	std::list<std::unique_ptr<service::interface::IContainerProcessor>> m_adaptors;
};
}
}
