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
	auto input(const dae::container::gyro &gyro) noexcept -> void;
	auto input(const dae::container::accdata &acc) noexcept -> void;
	constexpr auto previous() const noexcept { return m_previous; }

	auto calculate(const dae::container::ir_points &ir_points) -> dae::container::vcursor;

private:
	bool m_last_inverted{false};
	std::size_t m_last_point_count{};
	std::uint64_t m_tracking_score{};
	std::optional<dae::container::gyro> m_gyro;
	std::optional<dae::container::accdata> m_acc;

	std::array<dae::container::point, 2> last_points{};
	dae::container::vcursor m_previous{};
	bool m_wait_for_2points{true};
	bool m_was_abs_x_sorted{false};
};
}
}
