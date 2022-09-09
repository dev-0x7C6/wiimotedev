#pragma once

#include <QList>
#include <externals/common/types.hpp>

#include <cmath>
#include <optional>
#include <containers/structs.hpp>

namespace dae {
namespace functional {

class VirtualCursorProcessor {
public:
	auto calculate(const dae::container::ir_points &ir_points) -> dae::container::vcursor;

	constexpr auto previous() const noexcept -> dae::container::vcursor {
		return m_previous;
	}

private:
	std::array<dae::container::point, 2> last_points{};
	dae::container::vcursor m_previous{};
	bool m_wait_for_2points{true};
	bool m_was_abs_x_sorted{false};
};
}
}
