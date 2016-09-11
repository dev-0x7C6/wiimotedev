#pragma once

class InfraredCursor {
public:
	InfraredCursor(double x, double y, double distance, double angle, bool valid = true);
	InfraredCursor(const InfraredCursor &cursor);
	InfraredCursor();

	double x() const;
	double y() const;
	double distance() const;
	double angle() const;
	bool isValid() const;

	InfraredCursor &operator=(const InfraredCursor &other);

private:
	double m_x;
	double m_y;
	double m_distance;
	double m_angle;
	bool m_valid;
};
