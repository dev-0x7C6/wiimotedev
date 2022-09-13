#include "ir.hpp"
#include "dbus/interfaces/virtualcursor.h"

#include <QDBusConnection>
#include <QScreen>
#include <QCursor>
#include <QGuiApplication>

VirtualCursor::VirtualCursor(QObject *parent)
		: QObject(parent) {
	for (auto &&bus : {QDBusConnection::systemBus(), QDBusConnection::sessionBus()}) {
		auto cursor = new org::wiimotedev::virtualcursor("org.wiimotedev.daemon", "/virtualcursor", bus, this);
		connect(cursor, &org::wiimotedev::virtualcursor::dataChanged, this, &VirtualCursor::dataChanged);
	}
}

void VirtualCursor::dataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible) {
	auto screen = QGuiApplication::screens().first()->geometry();
	x += 1024.0 / 2.0;
	y += 768.0 / 2.0;
	auto rx = screen.width() * (std::min(x, 1024.0) / 1024.0);
	auto ry = screen.height() * (std::min(y, 768.0) / 768.0);
	QCursor::setPos(rx, ry);
}
