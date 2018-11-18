#pragma once

#include <cstdint>
#include <array>

#include <externals/common/types.hpp>

using balanceboard_data = std::array<i32, 4>;

namespace io {
namespace functional {

class BalanceBoardProcessor {
public:
	explicit BalanceBoardProcessor(balanceboard_data &&data, i32 min, i32 max);

	bool isValid() const;

	i32 x() const;
	i32 y() const;

private:
	i32 calculate(double x, double y) const;

public:
private:
	const balanceboard_data m_data;
	const i32 m_min;
	const i32 m_max;
};
}
}
