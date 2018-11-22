#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace dae {
namespace container {

class StickContainer final : public interface::IContainer {
public:
	explicit StickContainer(const common::enums::Device device,
		const i32 lx, const i32 ly, const i32 rx, const i32 ry)
			: m_device(device)
			, m_lx(lx)
			, m_ly(ly)
			, m_rx(rx)
			, m_ry(ry) {}

	common::enums::Device device() const final { return m_device; }
	enums::Event event() const final { return enums::Event::Stick; }

	constexpr auto lx() const noexcept { return m_lx; }
	constexpr auto ly() const noexcept { return m_ly; }
	constexpr auto rx() const noexcept { return m_rx; }
	constexpr auto ry() const noexcept { return m_ry; }

private:
	const common::enums::Device m_device;
	const i32 m_lx;
	const i32 m_ly;
	const i32 m_rx;
	const i32 m_ry;
};
}
}
