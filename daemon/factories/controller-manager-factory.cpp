#include "controller-manager-factory.h"

#include "api/xwiimote-manager.h"
#include "api/xwiimote-monitor.h"
#include "interfaces/iwiimote-api.h"

using namespace dae::api;
using namespace dae::factory;
using namespace dae::interface;

std::unique_ptr<IWiimoteManager> dae::factory::make_controller_manager(interface::IWiimote::Api type) noexcept {
	switch (type) {
		case IWiimote::Api::XWiimote: return std::make_unique<XWiimoteManager>();
	}
	return nullptr;
}
