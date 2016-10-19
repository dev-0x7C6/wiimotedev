#pragma once

namespace io {
namespace container {

class InfraredCursor {
public:
	InfraredCursor(double x, double y, double distance, double angle, bool valid = true);
	InfraredCursor(const InfraredCursor &cursor);
	InfraredCursor();

	inline double x() const { return m_x; }
	inline double y() const { return m_y; }
	inline double distance() const { return m_distance; }
	inline double angle() const { return m_angle; }
	inline bool isValid() const { return m_valid; }

	InfraredCursor &operator=(const InfraredCursor &other);

private:
	double m_x;
	double m_y;
	double m_distance;
	double m_angle;
	bool m_valid;
};
}
}
