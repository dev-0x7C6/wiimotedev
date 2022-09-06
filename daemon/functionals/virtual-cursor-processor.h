#pragma once

#include <QList>
#include <externals/common/types.hpp>

#include <cmath>
#include <optional>

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

struct vcursor {
	double x{};
	double y{};
	double distance{};
	double yaw{};
	double roll{};
	double pitch{};
};

class VirtualCursorProcessor {
public:
	auto calculate(QList<QPair<int, int>> &points) -> std::optional<vcursor>;

private:
	std::array<point, 2> last_points{};
	bool m_visible = false;
	bool m_wait_for_2points{true};
	bool m_was_abs_x_sorted{false};
};
}
}
