#include "wiimote-connector.h"

#include "factories/controller-manager-factory.h"

using namespace daemon::controller;
using namespace daemon::factory;
using namespace daemon::interface;

WiimoteConnector::WiimoteConnector(const IWiimote::Api api)
		: m_manager(ControllerManagerFactory::create(api)) {
}

void WiimoteConnector::process() {
	while (true) {
		auto controller = m_manager->connect();

		if (controller) {
			//controller->
			m_controllers.emplace_back(std::move(controller));
		}
	}
}

//UniqueId<256> unique;

//while (!m_interrupted) {
//	auto controller = manager->connect();

//	if (controller) {
//		const auto id = unique.take();
//		controller->setId(id);
//		devices.emplace_back(std::move(controller));
//	}

//	for (const auto &device : devices) {
//		const auto event = device->process();
//		if (!event)
//			continue;

//		switch (event->type()) {
//			case IContainer::Type::Infrared: {
//				const auto &points = static_cast<InfraredContainer *>(event.get())->points();

//				QList<struct irpoint> ir;
//				for (std::size_t i = 0; i < points.size(); ++i) {
//					struct irpoint t;
//					t.x = points[i].x;
//					t.y = points[i].y;
//					t.size = points[i].size;
//					ir.append(t);
//				}

//				m_events->dbusWiimoteInfrared(device->id(), ir);
//			} break;

//			case IContainer::Type::Accelerometer: {
//				const auto &data = static_cast<AccelerometerContainer *>(event.get())->data();
//				m_events->dbusWiimoteAcc(device->id(), data);
//			} break;
//		}
//	}

//	std::this_thread::sleep_for(1ms);
//}
