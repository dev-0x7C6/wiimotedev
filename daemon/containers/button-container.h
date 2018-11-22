#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace dae {
namespace container {

class ButtonContainer final : public interface::IContainer {
public:
	constexpr explicit ButtonContainer(common::enums::Device device, u64 state)
			: m_state(state)
			, m_source(device) {}

	common::enums::Device device() const final { return m_source; }
	enums::Event event() const final { return enums::Event::Button; }

	constexpr auto state() const noexcept { return m_state; }
	constexpr void setState(u64 state) { m_state = state; }

private:
	u64 m_state{0};
	common::enums::Device m_source;
};
}
}
