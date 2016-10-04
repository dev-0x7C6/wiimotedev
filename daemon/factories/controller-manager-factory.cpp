#include "controller-manager-factory.h"

#include "api/xwiimote-manager.h"
#include "api/xwiimote-monitor.h"
#include "interfaces/iwiimote-api.h"

using namespace service::api;
using namespace service::factory;
using namespace service::interface;

std::unique_ptr<IWiimoteManager> ControllerManagerFactory::create(const IWiimote::Api type) {
	switch (type) {
		case IWiimote::Api::XWiimote: return std::make_unique<XWiimoteManager>();
	}
	return nullptr;
}
