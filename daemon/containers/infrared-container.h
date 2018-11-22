#pragma once

#include <cstdint>
#include <array>

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

struct xwii_event;

namespace dae {
namespace container {
namespace structs {
struct irpoint {
	i16 size;
	u16 x;
	u16 y;
};
}

using IrPoints = std::array<structs::irpoint, 4>; // max 4 ir

class InfraredContainer final : public interface::IContainer {
public:
	constexpr explicit InfraredContainer(const IrPoints &points)
			: m_points(points) {}

	common::enums::Device device() const final { return common::enums::Device::Wiimote; }
	enums::Event event() const final { return enums::Event::Infrared; };

	const auto &points() const noexcept { return m_points; };

private:
	IrPoints m_points;
};
}
}
