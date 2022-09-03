#pragma once

#include <array>
#include <cstdint>
#include <utility>
#include <variant>
#include <vector>

#include "common/enums/device.h"

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u64 = std::uint64_t;

using s16 = std::int16_t;
using s32 = std::int32_t;

namespace dae::container {
struct accdata {
	float x;
	float y;
	float z;
	float pitch;
	float roll;
};

constexpr accdata operator+(const accdata &lhs, const accdata &rhs) noexcept {
	accdata ret;
	ret.x = lhs.x + rhs.x;
	ret.y = lhs.y + rhs.y;
	ret.z = lhs.z + rhs.z;
	ret.pitch = (lhs.pitch + rhs.pitch) / 2.0;
	ret.roll = (lhs.roll + rhs.roll) / 2.0;
	return ret;
}

constexpr accdata operator/(const accdata &lhs, const double v) noexcept {
	accdata ret;
	ret.x = lhs.x / v;
	ret.y = lhs.y / v;
	ret.z = lhs.z / v;
	return ret;
}

struct gyro {
	double x{};
	double y{};
	double z{};

	constexpr auto operator<=>(const gyro &lhs) const noexcept = default;
};

constexpr gyro operator+=(gyro &lhs, const gyro &rhs) noexcept {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

constexpr gyro operator-=(gyro &lhs, const gyro &rhs) noexcept {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

constexpr gyro operator*=(gyro &lhs, const double v) noexcept {
	lhs.x *= v;
	lhs.y *= v;
	lhs.z *= v;
	return lhs;
}

constexpr gyro operator/=(gyro &lhs, const double v) noexcept {
	lhs.x /= v;
	lhs.y /= v;
	lhs.z /= v;
	return lhs;
}

constexpr gyro operator+(const gyro &lhs, const gyro &rhs) noexcept {
	gyro ret;
	ret.x = lhs.x + rhs.x;
	ret.y = lhs.y + rhs.y;
	ret.z = lhs.z + rhs.z;
	return ret;
}

constexpr gyro operator*(const gyro &lhs, const double v) noexcept {
	gyro ret;
	ret.x = lhs.x * v;
	ret.y = lhs.y * v;
	ret.z = lhs.z * v;
	return ret;
}

constexpr gyro operator/(const gyro &lhs, const double v) noexcept {
	gyro ret;
	ret.x = lhs.x / v;
	ret.y = lhs.y / v;
	ret.z = lhs.z / v;
	return ret;
}

struct ir_point {
	s16 size;
	u16 x;
	u16 y;
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

using ir_points = std::array<ir_point, 4>;
using stick_pair = std::pair<stick, stick>;

using data = std::variant<std::monostate, accdata, gyro, ir_points, stick, stick_pair, pressure, button, status>;
using event = std::pair<common::enums::device, data>;
using events = std::vector<event>;

}
