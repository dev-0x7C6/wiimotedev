#pragma once

#include <array>
#include <cstdint>
#include <vector>

enum AxisType {
	X,
	Y,
};

enum class Stick : int32_t {
	Dpad,
	Stick,
	LStick,
	RStick,
};

struct Axis {
	constexpr Axis(int32_t axis, int32_t max, int32_t min, int32_t raw_max, int32_t raw_min)
			: axis(axis)
			, max(max)
			, min(min)
			, raw_max(raw_max)
			, raw_min(raw_min) {}

	int32_t axis;
	int32_t max;
	int32_t min;
	int32_t raw_max;
	int32_t raw_min;
	int32_t value = 0;
};

struct AxisPair {
public:
	constexpr AxisPair(Stick type, std::array<Axis, 2> mapping)
			: type(type)
			, pair(mapping) {}

	void center();
	void setValue(int32_t x, int32_t y);

public:
	Stick type;
	std::array<Axis, 2> pair;
	int32_t m_invertXAxis = false;
	int32_t m_invertYAxis = true;
};

struct ButtonMapping {
	constexpr ButtonMapping(uint64_t input, uint64_t output)
			: input(input)
			, output(output) {}
	uint64_t input;
	uint64_t output;
};

using ButtonMap = std::vector<ButtonMapping>;
using AxisMap = std::vector<AxisPair>;
