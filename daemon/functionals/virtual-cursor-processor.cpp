#include "virtual-cursor-processor.h"

#include <numbers>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
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

	constexpr auto ir_camera_center_px = ir_camera_max_px / 2.0;

	const auto diff = p[1] - p[0]; // diffrence in x axis and y axis
	const auto centered = tools::center(p[0], p[1]); // actual cordinates for virtual cursor
	const auto angle = std::atan2(diff.y, diff.x);
	const auto x = centered.x;
	const auto y = centered.y;

	Eigen::Matrix<double, 2, 1> coordinates{
		{x},
		{y},
	};

	Eigen::Matrix<double, 2, 1> coordinates_center{
		{-ir_camera_center_px.x},
		{-ir_camera_center_px.y},
	};

	const auto rotation_matrix = Eigen::Rotation2D(-angle);
	const auto rotate_coordinates = rotation_matrix * coordinates;
	const auto rotate_from_center = rotation_matrix * coordinates_center;
	const auto compute = rotate_coordinates + rotate_from_center;

	m_x = compute.x();
	m_y = compute.y() * -1.0;

	m_distance = tools::distance(p[0], p[1]);

	constexpr auto sensorbar_width = 24.00; // cm
	constexpr auto sensorbar_one_side_ir_width = 4.00; // cm
	constexpr auto sensorbar_centered_ir_distance = sensorbar_width - sensorbar_one_side_ir_width;
	constexpr auto sensorbar_delta_correction = 0.985; // correction from testing in field
	const auto d = ir_camera_max_px.x / sensorbar_centered_ir_distance;
	const auto real_distance = (ir_camera_max_px.x / m_distance) * d / (sensorbar_delta_correction * 2.0);
	const auto syntetic_x_distance = (m_x / ir_camera_max_px.x) * real_distance;
	const auto syntetic_y_distance = (m_y / ir_camera_max_px.y) * real_distance;

	m_yaw = tools::degree(std::atan2(syntetic_x_distance, real_distance));
	m_pitch = tools::degree(std::atan2(syntetic_y_distance, real_distance));
	m_roll = tools::degree(angle);

	spdlog::debug("virtual cursor:");
	spdlog::debug(" ---------------------------");
	spdlog::debug("  coordinates:");
	spdlog::debug("             [x]: {:+0.2f}px", m_x);
	spdlog::debug("             [y]: {:+0.2f}px", m_y);
	spdlog::debug(" ---------------------------");
	spdlog::debug("  angles:");
	spdlog::debug("         yaw [x]: {:+0.2f}°", m_yaw);
	spdlog::debug("        roll [y]: {:+0.2f}°", m_roll);
	spdlog::debug("       pitch [z]: {:+0.2f}°", m_pitch);
	spdlog::debug(" ---------------------------");
	spdlog::debug("  distance:");
	spdlog::debug("           point: {:+0.2f}px", tools::distance(p[0], p[1]));
	spdlog::debug("            real: {:+0.2f}cm", real_distance);
	spdlog::debug("                : {:+0.2f}m", real_distance / 100.0);
	spdlog::debug(" ---------------------------");
	spdlog::debug("  pointing from center:");
	spdlog::debug("             [x]: {:+0.2f}cm", syntetic_x_distance);
	spdlog::debug("             [y]: {:+0.2f}cm", syntetic_y_distance);

	m_distance = real_distance;

	return true;
}
