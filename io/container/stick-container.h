#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <externals/common/types.hpp>

namespace io {
namespace container {

enum AxisType {
	X,
	Y,
};

enum class Stick : i32 {
	Dpad,
	Stick,
	LStick,
	RStick,
};

struct Axis {
	constexpr Axis(i32 axis, i32 max, i32 min, i32 raw_max, i32 raw_min)
			: axis(axis)
			, max(max)
			, min(min)
			, raw_max(raw_max)
			, raw_min(raw_min) {}

	constexpr Axis(i32 axis, i32 max, i32 min)
			: axis(axis)
			, max(max)
			, min(min)
			, raw_max(max)
			, raw_min(min) {}

	i32 axis;
	i32 max;
	i32 min;
	i32 raw_max;
	i32 raw_min;
	i32 value = 0;
};

struct AxisPair {
public:
	constexpr AxisPair(Stick type, std::array<Axis, 2> mapping)
			: type(type)
			, pair(mapping) {}

	void center();
	void setValue(i32 x, i32 y);

public:
	Stick type;
	std::array<Axis, 2> pair;
	i32 m_invertXAxis = false;
	i32 m_invertYAxis = true;
};

struct ButtonMapping {
	constexpr ButtonMapping(u64 input, u64 output)
			: input(input)
			, output(output) {}
	u64 input;
	u64 output;
};

using ButtonMap = std::vector<ButtonMapping>;
using AxisMap = std::vector<AxisPair>;
}
}
