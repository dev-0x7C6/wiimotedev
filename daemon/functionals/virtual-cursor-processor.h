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
	auto calculate(QList<QPair<int, int>> &points) -> dae::container::vcursor;

private:
	std::array<dae::container::point, 2> last_points{};
	dae::container::vcursor previous;
	bool m_visible = false;
	bool m_wait_for_2points{true};
	bool m_was_abs_x_sorted{false};
};
}
}
