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

namespace dae::container::structs {
struct accdata {
	u8 x;
	u8 y;
	u8 z;
	double pitch;
	double roll;
};

struct gyro {
	s32 x;
	s32 y;
	s32 z;
};

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
using event = std::pair<common::enums::Device, data>;
using events = std::vector<event>;

}
