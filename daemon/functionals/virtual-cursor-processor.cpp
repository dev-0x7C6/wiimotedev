#include "virtual-cursor-processor.h"

#include <numbers>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <spdlog/spdlog.h>

using namespace dae::functional;
using namespace dae::container;

namespace debug::cursor {
constexpr auto visible = true;
}

auto VirtualCursorProcessor::calculate(QList<QPair<int, int>> &points) -> vcursor {
	std::array<point, 2> p;

	auto invalidate = [](vcursor v) -> vcursor {
		v.visible = false;
		return v;
	};

	switch (points.count()) {
		case 4:
			return invalidate(previous);
		case 3:
			return invalidate(previous);
		case 2:
			m_wait_for_2points = false;
			p[0].x = points.at(0).first;
			p[0].y = points.at(0).second;
			p[1].x = points.at(1).first;
			p[1].y = points.at(1).second;
			last_points = p;
			break;
		case 1:
			if (m_wait_for_2points)
				return invalidate(previous);
			{
				p[0].x = points.at(0).first;
				p[0].y = points.at(0).second;

				const auto d1 = distance(p[0], last_points[0]);
				const auto d2 = distance(p[0], last_points[1]);

				p[1] = last_points[d1 < d2 ? 1 : 0];
				p[1] = last_points[d1 < d2 ? 1 : 0];
				p[1].x += p[0].x - last_points[d1 > d2 ? 1 : 0].x;
				p[1].y += p[0].y - last_points[d1 > d2 ? 1 : 0].y;

				if (d1 > d2)
					std::swap(p[0], p[1]);

				last_points = p;
				break;
			}
		case 0:
			m_was_abs_x_sorted = false;
			m_wait_for_2points = true;
			return invalidate(previous);
	}

	constexpr auto ir_camera_max_px = point{
		.x = 1024.0,
		.y = 768.0,
	};

	constexpr auto ir_camera_center_px = ir_camera_max_px / 2.0;

	const auto diff = p[1] - p[0]; // diffrence in x axis and y axis
	const auto centered = center(p[0], p[1]); // actual cordinates for virtual cursor
	const auto angle = std::atan2(diff.y, diff.x);

	Eigen::Matrix<double, 2, 1> coordinates{
		{centered.x},
		{centered.y},
	};

	Eigen::Matrix<double, 2, 1> coordinates_center{
		{-ir_camera_center_px.x},
		{-ir_camera_center_px.y},
	};

	constexpr auto sensorbar_width = 24.00; // cm
	constexpr auto sensorbar_one_side_ir_width = 4.00; // cm
	constexpr auto sensorbar_centered_ir_distance = sensorbar_width - sensorbar_one_side_ir_width;
	constexpr auto sensorbar_delta_correction = 0.985; // correction from testing in field

	const auto rotation_matrix = Eigen::Rotation2D(-angle);
	const auto rotate_coordinates = rotation_matrix * coordinates;
	const auto rotate_from_center = rotation_matrix * coordinates_center;
	const auto compute = rotate_coordinates + rotate_from_center;

	const auto x = compute.x() * -1.0;
	const auto y = compute.y();

	const auto d = ir_camera_max_px.x / sensorbar_centered_ir_distance;
	const auto distance_ = distance(p[0], p[1]);
	const auto real_distance = (ir_camera_max_px.x / distance_) * d / (sensorbar_delta_correction * 2.0);
	const auto syntetic_x_distance = (x / ir_camera_max_px.x) * real_distance * -1.0;
	const auto syntetic_y_distance = (y / ir_camera_max_px.y) * real_distance;

	vcursor vc{
		.x = x,
		.y = y,
		.distance = real_distance,
		.yaw = degree(std::atan2(syntetic_x_distance, real_distance)),
		.roll = degree(angle),
		.pitch = degree(std::atan2(syntetic_y_distance, real_distance)),
		.visible = true,
	};

	if constexpr (debug::cursor::visible) {
		spdlog::debug("virtual cursor:");
		spdlog::debug(" ---------------------------");
		spdlog::debug("  coordinates:");
		spdlog::debug("             [x]: {:+0.2f}px", vc.x);
		spdlog::debug("             [y]: {:+0.2f}px", vc.y);
		spdlog::debug(" ---------------------------");
		spdlog::debug("  angles:");
		spdlog::debug("         yaw [x]: {:+0.2f}°", vc.yaw);
		spdlog::debug("        roll [y]: {:+0.2f}°", vc.roll);
		spdlog::debug("       pitch [z]: {:+0.2f}°", vc.pitch);
		spdlog::debug(" ---------------------------");
		spdlog::debug("  distance:");
		spdlog::debug("           point: {:+0.2f}px", distance_);
		spdlog::debug("            real: {:+0.2f}cm", real_distance);
		spdlog::debug("                : {:+0.2f}m", real_distance / 100.0);
		spdlog::debug(" ---------------------------");
		spdlog::debug("  pointing from center:");
		spdlog::debug("             [x]: {:+0.2f}cm", syntetic_x_distance);
		spdlog::debug("             [y]: {:+0.2f}cm", syntetic_y_distance);
	}

	previous = vc;
	return vc;
}
