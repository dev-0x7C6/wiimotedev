#include "wiimote-dispatcher.h"

#include <QDBusConnection>

#include "containers/accelerometer-container.h"
#include "containers/gyroscope-container.h"
#include "containers/infrared-container.h"
#include "wiimoteadaptor.h"

using namespace service::container;
using namespace service::dbus;
using namespace service::interface;

WiimoteDispatcher::WiimoteDispatcher(QObject *parent)
		: IContainerProcessor(parent) {

	new WiimoteAdaptor(this);
}

IContainerProcessor::Type WiimoteDispatcher::type() const {
	return Type::Wiimote;
}

void WiimoteDispatcher::process(const uint32_t id, const std::unique_ptr<IContainer> &container) {
	auto process_ir = [this, id, &container]() -> void {
		const auto &ir = static_cast<InfraredContainer *>(container.get())->points();
		emit wiimoteInfraredDataChanged(id, ir[0].x, ir[0].y, ir[1].x, ir[1].y, ir[2].x, ir[2].y, ir[3].x, ir[3].y);
	};

	auto process_acc = [this, id, &container]() -> void {
		const auto &data = static_cast<AccelerometerContainer *>(container.get())->data();
		emit wiimoteAccelerometerDataChanged(id, data.x, data.y, data.z, data.pitch, data.roll);
	};

	auto process_gyro = [this, id, &container]() -> void {
		const auto &data = static_cast<GyroscopeContainer *>(container.get())->data();
		emit wiimoteGyroscopeDataChanged(id, data.x, data.y, data.z, data.lowX, data.lowY, data.lowZ);
	};

	switch (container->type()) {
		case IContainer::Type::Infrared: return process_ir();
		case IContainer::Type::Accelerometer: return process_acc();
		case IContainer::Type::Gyroscope: return process_gyro();
	}
}

QList<uint> WiimoteDispatcher::wiimoteList() const {
	return {};
}
