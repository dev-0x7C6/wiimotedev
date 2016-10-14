#pragma once

#include <cmath>
#include <QList>

namespace dae {
namespace functional {

class VirtualCursorProcessor {
public:
	explicit VirtualCursorProcessor();
	bool calculate(QList<QPair<int, int>> &points, double roll);

	bool isVisible() const { return m_visible; }
	double distance() const { return m_distance; }
	double angle() const { return m_angle[m_index]; }
	double x() const { return m_x; }
	double y() const { return m_y; }

private:
	double angleDiff(double a, double b);
	double _distance(int16_t p[2], int16_t q[2]);

private:
	double m_x = 0;
	double m_y = 0;
	double m_distance = 0;
	double m_angle[2]{0, 0};

private:
	bool m_visible = false;
	int m_index = 0;
	int16_t m_ctable[2][2];
	int16_t m_ltable[2][2];
	int m_calibrationPoint[2];
};
}
}
