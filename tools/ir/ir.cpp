#include "ir.hpp"
#include "dbus/interfaces/wiimote.h"
#include "dbus/interfaces/virtualcursor.h"

#include <QDBusConnection>

VirtualCursor::VirtualCursor(QObject *parent)
		: QObject(parent) {
	for (auto &&bus : {QDBusConnection::systemBus(), QDBusConnection::sessionBus()}) {
		auto wiimote = new org::wiimotedev::wiimote("org.wiimotedev.daemon", "/wiimote", bus, this);
		auto cursor = new org::wiimotedev::virtualcursor("org.wiimotedev.daemon", "/virtualcursor", bus, this);
		connect(wiimote, &org::wiimotedev::wiimote::infraredDataChanged, this, &VirtualCursor::infraredDataChanged);
		connect(cursor, &org::wiimotedev::virtualcursor::dataChanged, this, &VirtualCursor::dataChanged);
	}
}

void VirtualCursor::infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	auto is_visible = [](auto x, auto y) {
		return x != 1023 && y != 1023;
	};

	m_ir = {
		{"x1", x1},
		{"y1", y1},
		{"x2", x2},
		{"y2", y2},
		{"x3", x3},
		{"y3", y3},
		{"x4", x4},
		{"y4", y4},
		{"v1", is_visible(x1, y1)},
		{"v2", is_visible(x2, y2)},
		{"v3", is_visible(x3, y3)},
		{"v4", is_visible(x4, y4)},
	};
	emit irChanged(m_ir);
}

void VirtualCursor::dataChanged(uint id, double x, double y, double l, double a) {
	QPointF p(x, y);
	m_cursor = p;
	m_angle = a;
	emit cursorChanged(p);
	emit angleChanged(a);
}

QPointF VirtualCursor::cursor() const {
	return m_cursor;
}

auto VirtualCursor::ir() const -> QJsonObject {
	return m_ir;
}
