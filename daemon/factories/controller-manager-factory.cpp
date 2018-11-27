#include "controller-manager-factory.h"

#include "api/xwiimote-manager.h"
#include "api/xwiimote-monitor.h"

using dae::api::XWiimoteManager;
using dae::interface::IWiimote;
using dae::interface::IWiimoteManager;

std::unique_ptr<IWiimoteManager> dae::factory::make_controller_manager(interface::IWiimote::Api type) noexcept {
	switch (type) {
		case IWiimote::Api::XWiimote: return std::make_unique<XWiimoteManager>();
	}
	return nullptr;
}
