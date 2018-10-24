#include "eioinfraredmouse.h"
#include <cmath>
#include <QCursor>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>
#include <iostream>
#include <cassert>

using namespace io;
using namespace io::container;
using namespace io::interface;
using namespace io::emulation;

EIOInfraredMouse::EIOInfraredMouse(EventDevice &device, QObject *parent)
		: QThread(parent)
		, m_device(device) {
}

EIOInfraredMouse::~EIOInfraredMouse() {
	interrupt();
	wait();
}

void EIOInfraredMouse::dbusVirtualCursorPosition(uint32_t id, double x, double y, double distance, double angle) {
	if (id != m_id) return;

	setCursor({x, y, distance, angle});
}

void EIOInfraredMouse::dbusVirtualCursorLost(uint32_t id) {
	if (id != m_id) return;

	m_mutex.lock();
	InfraredCursor local = m_cursor;
	m_mutex.unlock();
	setCursor({local.x(), local.y(), local.distance(), local.angle(), false});
}

void EIOInfraredMouse::run() {
	InfraredCursor current;
	InfraredCursor last;

	while (!m_interrupted) {
		m_mutex.lock();
		current = m_cursor;
		m_mutex.unlock();

		if (!current.isValid()) {
			current = last;
			m_timeout++;
		} else
			m_timeout = 0;

		if (!last.isValid())
			last = current;

		m_position[XAxis] = current.x();
		m_position[YAxis] = current.y();
		m_delta[XAxis] = current.x() - last.x();
		m_delta[YAxis] = current.y() - last.y();
		m_size[XAxis] = 0x400 - m_config.deadzoneX();
		m_size[YAxis] = 0x300 - m_config.deadzoneY();

		switch (m_mode) {
			case Relative:
				processRelativeMouse();
				break;

			case Absolute:
				processAbsoluteMouse();
				break;

			default:
				break;
		}

		m_tick.wait(1000);
		last = current;
	}
}

void EIOInfraredMouse::processAbsoluteMouse() {
	if (m_delta[XAxis] == 0 && m_delta[YAxis] == 0)
		return;

	QRect rect = QApplication::desktop()->screenGeometry(0);
	double x = -m_position[XAxis] + 0x200;
	double y = -m_position[YAxis] + 0x180;
	const double w = rect.width();
	const double h = rect.height();
	x *= (w / static_cast<double>(0x400));
	y *= (h / static_cast<double>(0x300));
	QCursor::setPos(x, y);
}

void EIOInfraredMouse::processRelativeMouse() {
	if (m_timeout > m_config.accTimeout())
		return;

	auto abs = [](const double x) {
		return (x < 0) ? -x : x;
	};
	double x = -m_delta[XAxis];
	double y = -m_delta[YAxis];
	m_acc[XAxis] += x;
	m_acc[YAxis] += y;
	m_acc[XAxis] -= static_cast<int32_t>(x);
	m_acc[YAxis] -= static_cast<int32_t>(y);
	auto calc = [&](const int8_t axis) {
		const int32_t shift = static_cast<int32_t>(m_acc[axis]);
		m_acc[axis] -= shift;
		return (m_position[axis] > 0) ? shift : -shift;
	};

	if (m_config.accEnabled()) {
		if (m_position[XAxis] > m_config.deadzoneX() || m_position[XAxis] < -m_config.deadzoneX())
			m_acc[XAxis] += pow(abs(m_position[XAxis] / m_size[XAxis] * m_config.accMultiX()), m_config.accPowX());

		if (m_position[YAxis] > m_config.deadzoneY() || m_position[YAxis] < -m_config.deadzoneY())
			m_acc[YAxis] += pow(abs(m_position[YAxis] / m_size[YAxis] * m_config.accMultiY()), m_config.accPowY());
	}

	x -= calc(XAxis);
	y -= calc(YAxis);

	if (x != 0 || y != 0)
		m_device.moveMousePointer(x, y);
}

void EIOInfraredMouse::setCursor(InfraredCursor &&cursor) {
	QMutexLocker locker(&m_mutex);
	m_cursor = cursor;
}

uint32_t EIOInfraredMouse::id() const {
	return m_id;
}

void EIOInfraredMouse::setId(const uint32_t id) {
	m_id = id;
}

void EIOInfraredMouse::interrupt() {
	m_interrupted = true;
}
