#include "virtual-cursor-processor.h"

#include <numbers>

#include <spdlog/spdlog.h>

using namespace dae::functional;

namespace tools {

constexpr auto distance(const point &p1, const point &p2) noexcept -> double {
	const auto dx = std::pow(std::abs(p2.x - p1.x), 2);
	const auto dy = std::pow(std::abs(p2.y - p1.y), 2);
	return std::sqrt(dx + dy);
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

bool VirtualCursorProcessor::calculate(QList<QPair<int, int>> &points, double roll) {
	std::array<point, 2> p;

	switch (points.count()) {
		case 4:
			return false;
		case 3:
			return false;
		case 2:
			m_wait_for_2points = false;
			p[0].x = points.at(0).first;
			p[0].y = points.at(0).second;
			p[1].x = points.at(1).first;
			p[1].y = points.at(1).second;
			last_points = p;
			m_visible = true;
			break;
		case 1:
			if (m_wait_for_2points)
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
			m_was_abs_x_sorted = false;
			m_wait_for_2points = true;
			return false;
	}

	constexpr auto ir_camera_max_px = point{
		.x = 1024.0,
		.y = 768.0,
	};
	constexpr auto ir_cx = ir_camera_max_px.x / 2; // center x for ir
	constexpr auto ir_cy = ir_camera_max_px.y / 2; // center y for ir

	const auto diff = p[0] - p[1]; // diffrence in x axis and y axis
	const auto centered = tools::center(p[0], p[1]); // actual cordinates for virtual cursor
	const auto angle = std::atan2(diff.y, diff.x);
	const auto sin_angle = std::sin(-angle);
	const auto cos_angle = std::cos(angle);
	const auto x = centered.x;
	const auto y = centered.y;

	m_x = x * cos_angle - y * sin_angle + ir_cx * (1.0 - cos_angle) + ir_cy * sin_angle;
	m_y = x * sin_angle + y * cos_angle - ir_cx * sin_angle + ir_cy * (1.0 - cos_angle);

	// x offseting
	m_x = ir_camera_max_px.x - m_x;

	m_x = ir_cx - m_x;
	m_y = ir_cy - m_y;
	m_angle = tools::degree(angle);

	m_distance = tools::distance(p[0], p[1]);

	constexpr auto sensorbar_width = 24.00; // cm
	constexpr auto sensorbar_one_side_ir_width = 4.00; // cm
	constexpr auto sensorbar_centered_ir_distance = sensorbar_width - sensorbar_one_side_ir_width;
	constexpr auto sensorbar_delta_correction = 0.985; // correction from testing in field
	const auto d = ir_camera_max_px.x / sensorbar_centered_ir_distance;
	const auto real_distance = (ir_camera_max_px.x / m_distance) * d / (sensorbar_delta_correction * 2.0);

	spdlog::info("distance");
	spdlog::info("    point: {:+0.2f}px", tools::distance(p[0], p[1]));
	spdlog::info("     real: {:+0.2f}cm", real_distance);
	spdlog::info("         : {:+0.2f}m", real_distance / 100.0);

	m_distance = real_distance;

	return true;
}
