#include "controller-manager-factory.h"

#include "api/cwiid-manager.h"
#include "api/xwiimote-manager.h"
#include "api/xwiimote-monitor.h"
#include "interfaces/iwiimote-api.h"

using namespace daemon::api;
using namespace daemon::factory;
using namespace daemon::interface;

std::unique_ptr<IWiimoteManager> ControllerManagerFactory::create(const IWiimote::Api type) {
	switch (type) {
		case IWiimote::Api::Cwiid: return std::make_unique<CwiidManager>();
		case IWiimote::Api::XWiimote: return std::make_unique<XWiimoteManager>();
	}
	return nullptr;
}
