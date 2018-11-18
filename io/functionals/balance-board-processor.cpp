#include "balance-board-processor.h"

#include <numeric>

using namespace io::functional;

BalanceBoardProcessor::BalanceBoardProcessor(balanceboard_data &&data, i32 min, i32 max)
		: m_data(std::move(data))
		, m_min(min)
		, m_max(max)

{}

i32 BalanceBoardProcessor::calculate(double x, double y) const {
	auto p = std::max(std::min(x / y, 2.0), 0.0);
	auto q = std::max(std::min(y / x, 2.0), 0.0);

	if (p >= 1.0)
		return static_cast<i32>((p - 1.0) * m_max);
	else
		return static_cast<i32>((q - 1.0) * m_min);
}

bool BalanceBoardProcessor::isValid() const { return std::accumulate(m_data.cbegin(), m_data.cend(), 0) > 600; }

i32 BalanceBoardProcessor::x() const {
	const auto l = m_data.at(0) + m_data.at(2) + 1;
	const auto r = m_data.at(1) + m_data.at(3) + 1;
	return calculate(l, r);
}

i32 BalanceBoardProcessor::y() const {
	const auto t = m_data.at(0) + m_data.at(1) + 1;
	const auto b = m_data.at(2) + m_data.at(3) + 1;
	return calculate(t, b);
}
