#pragma once

#include <cstdint>
#include <array>

using balanceboard_data = std::array<int32_t, 4>;

namespace io {
namespace functional {

class BalanceBoardProcessor {
public:
	explicit BalanceBoardProcessor(balanceboard_data &&data, int32_t min, int32_t max);

	bool isValid() const;

	int32_t x() const;
	int32_t y() const;

private:
	int32_t calculate(double x, double y) const;

public:
private:
	const balanceboard_data m_data;
	const int32_t m_min;
	const int32_t m_max;
};
}
}
