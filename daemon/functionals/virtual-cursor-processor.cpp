#include "virtual-cursor-processor.h"

#include <numbers>

using namespace dae::functional;

namespace tools {

constexpr auto distance(const point &p1, const point &p2) noexcept -> double {
	const auto dx = std::pow(p2.x - p1.x, 2);
	const auto dy = std::pow(p2.y - p2.y, 2);
	return std::sqrt(dx + dy);
}

constexpr auto distance_xy(const point &p1, const point &p2) noexcept -> point {
	return {p1.x - p2.x, p1.y - p2.y};
}

constexpr auto abs(const point &p) noexcept -> point {
	return {std::abs(p.x), std::abs(p.y)};
}

constexpr auto center(const point &p1, const point &p2) noexcept -> point {
	return {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
}

constexpr auto degree(const double radian) noexcept -> double {
	return radian * (180.0 / std::numbers::pi);
}

}

static bool wait_for_2points{true};
static bool was_abs_x_sorted{false};

bool VirtualCursorProcessor::calculate(QList<QPair<int, int>> &points, double roll) {
	std::array<point, 2> p;

	switch (points.count()) {
		case 4:
			return false;
		case 3:
			return false;
		case 2:
			wait_for_2points = false;
			p[0].x = points.at(0).first;
			p[0].y = points.at(0).second;
			p[1].x = points.at(1).first;
			p[1].y = points.at(1).second;
			last_points = p;
			m_visible = true;
			break;
		case 1:
			if (wait_for_2points)
				return false;
			{
				p[0].x = points.at(0).first;
				p[0].y = points.at(0).second;

				const auto d1 = tools::distance(p[0], last_points[0]);
				const auto d2 = tools::distance(p[0], last_points[1]);

				p[1] = last_points[d1 < d2 ? 1 : 0];
				p[1] = last_points[d1 < d2 ? 1 : 0];
				p[1].x += p[0].x - last_points[d1 > d2 ? 1 : 0].x;
				p[1].y += p[0].y - last_points[d1 > d2 ? 1 : 0].y;

				if (d1 > d2)
					std::swap(p[0], p[1]);

				last_points = p;

				m_visible = true;
				break;
			}
		case 0:
			was_abs_x_sorted = false;
			wait_for_2points = true;
			return false;
	}

	constexpr auto ir_mx = 1024.0; // max ir resolution x
	constexpr auto ir_my = 768.0; // max ir resolution y
	constexpr auto ir_cx = ir_mx / 2; // center x for ir
	constexpr auto ir_cy = ir_my / 2; // center y for ir

	const auto distance = tools::distance_xy(p[0], p[1]); // distances in x axis and y axis
	const auto centered = tools::center(p[0], p[1]); // actual cordinates for virtual cursor
	const auto angle = std::atan2(distance.y, distance.x);
	const auto sin_angle = std::sin(-angle);
	const auto cos_angle = std::cos(angle);
	const auto x = centered.x;
	const auto y = centered.y;

	m_x = (ir_mx - (x * cos_angle - y * sin_angle + ir_cx * (1 - cos_angle) + ir_cy * sin_angle));
	m_y = (x * sin_angle + y * cos_angle - ir_cx * sin_angle + ir_cy * (1 - cos_angle));

	m_x = ir_cx - m_x;
	m_y = ir_cy - m_y;
	m_angle = tools::degree(angle);

	spdlog::debug("x: {}, y: {}", m_x, m_y);
	spdlog::debug("angle: {}", angle);
	return true;
}
