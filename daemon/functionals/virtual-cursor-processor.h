#pragma once

#include <QList>
#include <cmath>
#include <externals/common/types.hpp>

namespace dae {
namespace functional {

struct point {
	double x{};
	double y{};

	constexpr auto operator<=>(const point &v) const noexcept = default;
};

constexpr point operator/(const point &lhs, const double v) {
	point ret;
	ret.x = lhs.x / v;
	ret.y = lhs.y / v;
	return ret;
}

constexpr point operator-(const point &lhs, const point &rhs) {
	point ret;
	ret.x = lhs.x - rhs.x;
	ret.y = lhs.y - rhs.y;
	return ret;
}

class VirtualCursorProcessor {
public:
	bool calculate(QList<QPair<int, int>> &points, double roll);

	bool isVisible() const { return m_visible; }
	double distance() const { return m_distance; }
	double angle() const { return m_angle; }
	double x() const { return m_x; }
	double y() const { return m_y; }

private:
	double m_x{};
	double m_y{};
	double m_distance{};
	double m_angle{};
	double m_yaw{};

private:
	std::array<point, 2> last_points{};
	bool m_visible = false;
	bool m_wait_for_2points{true};
	bool m_was_abs_x_sorted{false};
};
}
}
