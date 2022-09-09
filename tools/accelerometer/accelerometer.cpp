#include "accelerometer.hpp"
#include "dbus/interfaces/wiimote.h"

#include <QDBusConnection>

Accelerometer::Accelerometer(QObject *parent)
		: QObject(parent) {
	for (auto &&bus : {QDBusConnection::systemBus(), QDBusConnection::sessionBus()}) {
		auto wiimote = new org::wiimotedev::wiimote("org.wiimotedev.daemon", "/wiimote", bus, this);
		connect(wiimote, &org::wiimotedev::wiimote::accelerometerDataChanged, this, &Accelerometer::accelerometerDataChanged);
	}
}

void Accelerometer::accelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll) {
	m_data = {
		{"raw_x", x},
		{"raw_y", y},
		{"raw_z", z},
		{"pitch", pitch},
		{"roll", roll},
	};
	emit dataChanged(m_data);
}

auto Accelerometer::data() const -> QJsonObject {
	return m_data;
}
