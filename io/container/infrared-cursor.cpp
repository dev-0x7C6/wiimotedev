#include "infrared-cursor.h"

InfraredCursor::InfraredCursor(double x, double y, double distance, double angle, bool valid)
		: m_x(x)
		, m_y(y)
		, m_distance(distance)
		, m_angle(angle)
		, m_valid(valid) {
}

InfraredCursor::InfraredCursor(const InfraredCursor &cursor) {
	*this = cursor;
}

InfraredCursor::InfraredCursor()
		: m_x(0)
		, m_y(0)
		, m_distance(0)
		, m_angle(0)
		, m_valid(false) {
}

double InfraredCursor::x() const {
	return m_x;
}

double InfraredCursor::y() const {
	return m_y;
}

double InfraredCursor::distance() const {
	return m_distance;
}

double InfraredCursor::angle() const {
	return m_angle;
}

bool InfraredCursor::isValid() const {
	return m_valid;
}

InfraredCursor &InfraredCursor::operator=(const InfraredCursor &other) {
	m_x = other.m_x;
	m_y = other.m_y;
	m_distance = other.m_distance;
	m_angle = other.m_angle;
	m_valid = other.m_valid;
	return *this;
}
