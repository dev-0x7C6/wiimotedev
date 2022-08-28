#include "balanceboard.hpp"
#include "dbus/interfaces/balanceboard.h"

#include <QDBusConnection>

BalanceBoard::BalanceBoard(QObject *parent)
		: QObject(parent) {
	auto iface_system = new org::wiimotedev::balanceboard("org.wiimotedev.daemon", "/balanceboard", QDBusConnection::systemBus(), this);
	auto iface_session = new org::wiimotedev::balanceboard("org.wiimotedev.daemon", "/balanceboard", QDBusConnection::sessionBus(), this);
	connect(iface_system, &org::wiimotedev::balanceboard::dataChanged, this, &BalanceBoard::balanceBoardDataChanged);
	connect(iface_session, &org::wiimotedev::balanceboard::dataChanged, this, &BalanceBoard::balanceBoardDataChanged);
}

void BalanceBoard::balanceBoardDataChanged(uint id, int top_left, int top_right, int bottom_left, int bottom_right) {
	m_topLeft = top_left > 200 ? top_left : 0;
	m_topRight = top_right > 200 ? top_right : 0;
	m_bottomLeft = bottom_left > 200 ? bottom_left : 0;
	m_bottomRight = bottom_right > 200 ? bottom_right : 0;

	emit topLeftChanged(m_topLeft);
	emit topRightChanged(m_topRight);
	emit bottomLeftChanged(m_bottomLeft);
	emit bottomRightChanged(m_bottomRight);
}

int BalanceBoard::bottomRight() const {
	return m_bottomRight;
}

int BalanceBoard::bottomLeft() const {
	return m_bottomLeft;
}

int BalanceBoard::topRight() const {
	return m_topRight;
}

int BalanceBoard::topLeft() const {
	return m_topLeft;
}
