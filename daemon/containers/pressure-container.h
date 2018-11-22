#pragma once

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace container {
namespace structs {
struct pressdata {
	i32 tl;
	i32 tr;
	i32 bl;
	i32 br;
};
}

class PressureContainer final : public interface::IContainer {
public:
	constexpr explicit PressureContainer(const structs::pressdata &data)
			: m_data(data) {}

	common::enums::Device device() const final { return common::enums::Device::BalanceBoard; }
	enums::Event event() const final { enums::Event::Pressure; };

	const auto &data() const noexcept { return m_data; }

private:
	const structs::pressdata m_data;
};
}
}
