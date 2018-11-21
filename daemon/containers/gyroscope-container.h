#pragma once

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace container {

class GyroscopeContainer final : public interface::IContainer {
public:
	constexpr explicit GyroscopeContainer(i32 x, i32 y, i32 z)
			: m_x(x)
			, m_y(y)
			, m_z(z) {
	}

	common::enums::Device device() const final { return common::enums::Device::Wiimote; }
	enums::Event event() const final { return enums::Event::Gyroscope; }

	constexpr auto x() const noexcept { return m_x; }
	constexpr auto y() const noexcept { return m_y; }
	constexpr auto z() const noexcept { return m_z; }

private:
	const i32 m_x = 0;
	const i32 m_y = 0;
	const i32 m_z = 0;
};
}
}
