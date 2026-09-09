#include "virtual-cursor-processor.h"
#include "containers/structs.hpp"

#include <algorithm>
#include <ranges>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <spdlog/spdlog.h>

using namespace dae::functional;
using namespace dae::container;

namespace debug::cursor {
constexpr auto visible = false;
}

constexpr auto hide(vcursor v) -> vcursor {
	v.visible = false;
	return v;
};

constexpr auto swap(std::array<dae::container::vc_point, 2> p) {
	std::swap(p.front(), p.back());
	return p;
}

auto VirtualCursorProcessor::input(const dae::container::gyro &gyro) noexcept -> void {
	m_gyro = gyro;
}

auto VirtualCursorProcessor::input(const dae::container::accdata &acc) noexcept -> void {
	m_acc = acc;
}

auto VirtualCursorProcessor::stage1_heuristic_prefferable_points(const ir_points &raw_ir_points) -> std::optional<std::array<vc_point, 2>> {
	const auto ir = reorder(raw_ir_points);
	const auto cnt = count(ir);

	std::array<vc_point, 2> p{};

	auto process_2points = [&] {
		for (auto index : {0, 1})
			p[index] = to_point(ir[index]);

		return last_points = p;
	};

	auto process_1point = [&] {
		p[0] = to_point(ir[0]);

		const auto d1 = distance(p[0], last_points->at(0));
		const auto d2 = distance(p[0], last_points->at(1));

		const auto p0 = d1 < d2 ? 0 : 1;
		const auto p1 = d1 < d2 ? 1 : 0;

		const vc_point approximation{
			.x = p[0].x - last_points->at(p0).x,
			.y = p[0].y - last_points->at(p0).y,
		};

		p[1] = last_points->at(p1) + approximation;

		if (d1 > d2)
			std::swap(p[0], p[1]);

		return last_points = p;
	};

	switch (m_last_point_count = cnt) {
		case 2: return process_2points();
		case 1:
			if (last_points)
				return process_1point();
	}

	return {};
}

auto VirtualCursorProcessor::stage2_accelerometer_correction(std::array<container::vc_point, 2> p) -> std::array<container::vc_point, 2> {
	const auto diff = p[1] - p[0];
	const auto ir_roll = std::atan2(diff.y, diff.x);
	const auto acc_roll = m_acc ? m_acc.value().angles.roll() : ir_roll;
	const auto angle_difference = std::abs(angle_degree_distance(degree(ir_roll), acc_roll));

	if (m_last_inverted && m_tracking_score > 200)
		return swap(p);

	m_last_inverted = angle_difference > 90.0;

	if (angle_difference > 90.0)
		return swap(p);

	return p;
}

auto VirtualCursorProcessor::calculate(const dae::container::ir_points &ir_points) -> vcursor {
	const auto probe = stage1_heuristic_prefferable_points(ir_points);

	if (!probe) {
		m_tracking_score = 0;
		return hide(m_previous);
	}

	const auto cnt = count(ir_points);

	if (cnt) {
		if (m_last_point_count == cnt)
			m_tracking_score++;
		else
			m_tracking_score = 0;
	}

	const auto p = stage2_accelerometer_correction(probe.value());

	const auto diff = p[1] - p[0]; // diffrence in x axis and y axis
	const auto centered = center(p[0], p[1]); // actual coordinates for virtual cursor
	const auto roll = std::atan2(diff.y, diff.x);

	constexpr auto ir_camera_max_px = vc_point{
		.x = 1024.0,
		.y = 768.0,
	};

	constexpr auto ir_camera_center_px = ir_camera_max_px / 2.0;

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

	const auto rotation_matrix = Eigen::Rotation2D(-roll);
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
		.roll = degree(roll),
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
		spdlog::debug(" ---------------------------");
		//		spdlog::debug("  angle difference:");
		//		spdlog::debug("          [roll]: {:+0.2f}°", angle_difference);
		spdlog::debug("  tracking score:");
		spdlog::debug("        [points]: {}", m_tracking_score);
	}

	m_last_point_count = count(ir_points);
	m_previous = vc;
	return vc;
}
