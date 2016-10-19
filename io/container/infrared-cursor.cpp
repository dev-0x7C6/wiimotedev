#include "infrared-cursor.h"

using namespace io::container;

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

InfraredCursor &InfraredCursor::operator=(const InfraredCursor &other) {
	m_x = other.m_x;
	m_y = other.m_y;
	m_distance = other.m_distance;
	m_angle = other.m_angle;
	m_valid = other.m_valid;
	return *this;
}
