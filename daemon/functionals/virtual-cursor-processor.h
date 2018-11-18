#pragma once

#include <QList>
#include <cmath>
#include <externals/common/types.hpp>

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
	double _distance(i16 p[2], i16 q[2]);

private:
	double m_x = 0;
	double m_y = 0;
	double m_distance = 0;
	double m_angle[2]{0, 0};

private:
	bool m_visible = false;
	int m_index = 0;
	i16 m_ctable[2][2];
	i16 m_ltable[2][2];
	int m_calibrationPoint[2];
};
}
}
