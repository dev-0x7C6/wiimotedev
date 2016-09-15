#include "controller-manager-factory.h"
#include "api/cwiid-manager.h"
#include "api/xwiimote-manager.h"
#include "api/xwiimote-monitor.h"

using namespace daemon::api;
using namespace daemon::factory;
using namespace daemon::interface;

std::unique_ptr<IWiimoteManager> ControllerManagerFactory::create(const ApiType type) {
	switch (type) {
		case ApiType::Cwiid: return std::make_unique<CwiidManager>();
		case ApiType::XWiimote: return std::make_unique<XWiimoteManager>();
	}
	return nullptr;
}
