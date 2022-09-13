#include "ir.hpp"
#include "dbus/interfaces/wiimote.h"
#include "dbus/interfaces/virtualcursor.h"

#include <QDBusConnection>

#include <include/wiimotedev/wiimotedev>

VirtualCursor::VirtualCursor(QObject *parent)
		: QObject(parent) {
	for (auto &&bus : {QDBusConnection::systemBus(), QDBusConnection::sessionBus()}) {
		auto wiimote = new org::wiimotedev::wiimote("org.wiimotedev.daemon", "/wiimote", bus, this);
		auto cursor = new org::wiimotedev::virtualcursor("org.wiimotedev.daemon", "/virtualcursor", bus, this);
		connect(wiimote, &org::wiimotedev::wiimote::infraredDataChanged, this, &VirtualCursor::infraredDataChanged);
		connect(wiimote, &org::wiimotedev::wiimote::buttonDataChanged, this, &VirtualCursor::buttonDataChanged);
		connect(cursor, &org::wiimotedev::virtualcursor::dataChanged, this, &VirtualCursor::dataChanged);
	}
}

void VirtualCursor::infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	auto is_visible = [](auto x, auto y) {
		return x != 1023 && y != 1023;
	};

	if (!m_model.contains(id))
		m_model[id] = {};

	auto &&vc = m_model[id];
	vc.insert("id", static_cast<int>(id));
	vc.insert("x1", x1);
	vc.insert("y1", y1);
	vc.insert("x2", x2);
	vc.insert("y2", y2);
	vc.insert("x3", x3);
	vc.insert("y3", y3);
	vc.insert("x4", x4);
	vc.insert("y4", y4);
	vc.insert("v1", is_visible(x1, y1));
	vc.insert("v2", is_visible(x2, y2));
	vc.insert("v3", is_visible(x3, y3));
	vc.insert("v4", is_visible(x4, y4));

	emit modelChanged(model());
}

void VirtualCursor::dataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible) {
	if (!m_model.contains(id))
		m_model[id] = {};

	auto &&vc = m_model[id];
	vc.insert("id", static_cast<int>(id));
	vc.insert("x", x);
	vc.insert("y", y);
	vc.insert("yaw", yaw);
	vc.insert("roll", roll);
	vc.insert("pitch", pitch);
	vc.insert("distance", distance_cm);
	vc.insert("visible", visible);

	emit modelChanged(model());
}

void VirtualCursor::buttonDataChanged(uint id, qulonglong mask) {
	if (!m_model.contains(id))
		m_model[id] = {};

	using namespace wiimotedev;

	auto &&vc = m_model[id];
	vc.insert("press", (mask & WIIMOTEDEV_BTN_A) == WIIMOTEDEV_BTN_A && (mask & WIIMOTEDEV_BTN_B) == WIIMOTEDEV_BTN_B);

	emit modelChanged(model());
}

auto VirtualCursor::model() const -> QJsonArray {
	QJsonArray arr;
	for (auto &&ir : m_model)
		arr.append(ir);

	return arr;
}
