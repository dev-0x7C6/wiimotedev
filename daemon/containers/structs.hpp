#pragma once

#include <array>
#include <cstdint>
#include <utility>
#include <variant>
#include <vector>
#include <cmath>

#include "common/enums/device.h"

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u64 = std::uint64_t;

using s16 = std::int16_t;
using s32 = std::int32_t;

namespace dae::container {

struct axis3d {
	double x{};
	double y{};
	double z{};

	constexpr auto yaw() const noexcept { return x; }
	constexpr auto roll() const noexcept { return y; }
	constexpr auto pitch() const noexcept { return z; }

	constexpr auto operator<=>(const axis3d &lhs) const noexcept = default;
};

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

constexpr auto angle_degree_distance(double lhs, double rhs) noexcept -> double {
	auto angle = std::fmod(rhs - lhs, 360.0);
	if (angle < -180.0) {
		angle += 360.0;
	} else if (angle > 179.0)
		angle -= 360.0;
	return angle;
}

struct vcursor {
	double x{};
	double y{};
	double distance{};
	double yaw{};
	double roll{};
	double pitch{};
	bool visible{false};
};

constexpr axis3d operator+=(axis3d &lhs, const axis3d &rhs) noexcept {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

constexpr axis3d operator-=(axis3d &lhs, const axis3d &rhs) noexcept {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

constexpr axis3d operator*=(axis3d &lhs, const double v) noexcept {
	lhs.x *= v;
	lhs.y *= v;
	lhs.z *= v;
	return lhs;
}

constexpr axis3d operator/=(axis3d &lhs, const double v) noexcept {
	lhs.x /= v;
	lhs.y /= v;
	lhs.z /= v;
	return lhs;
}

constexpr axis3d operator+(const axis3d &lhs, const axis3d &rhs) noexcept {
	axis3d ret;
	ret.x = lhs.x + rhs.x;
	ret.y = lhs.y + rhs.y;
	ret.z = lhs.z + rhs.z;
	return ret;
}

constexpr axis3d operator*(const axis3d &lhs, const double v) noexcept {
	axis3d ret;
	ret.x = lhs.x * v;
	ret.y = lhs.y * v;
	ret.z = lhs.z * v;
	return ret;
}

constexpr axis3d operator/(const axis3d &lhs, const double v) noexcept {
	axis3d ret;
	ret.x = lhs.x / v;
	ret.y = lhs.y / v;
	ret.z = lhs.z / v;
	return ret;
}

struct accdata {
	axis3d raw{};
	axis3d angles{};

	constexpr auto operator<=>(const accdata &lhs) const noexcept = default;
};

struct gyro {
	axis3d axies{};
	axis3d angles{};

	constexpr auto operator<=>(const gyro &lhs) const noexcept = default;
};

struct ir_point {
	u16 x{};
	u16 y{};
	bool valid{false};

	constexpr auto operator<=>(const ir_point &lhs) const noexcept = default;
};

struct stick {
	s32 x;
	s32 y;
};

struct pressure {
	std::pair<s32, s32> top;
	std::pair<s32, s32> bottom;
};

struct button {
	u64 states;
};

struct status {
	bool is_connected;
};

struct reconfigure {
};

using ir_points = std::array<ir_point, 4>;
using stick_pair = std::pair<stick, stick>;

using data = std::variant<std::monostate, reconfigure, accdata, gyro, ir_points, stick, stick_pair, pressure, button, status>;
using event = std::pair<common::enums::device, data>;
using events = std::vector<event>;

}
