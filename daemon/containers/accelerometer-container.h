#pragma once

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace container {
namespace structs {
struct accdata {
	u8 x;
	u8 y;
	u8 z;
	double pitch;
	double roll;
};
}

class AccelerometerContainer final : public interface::IContainer {
public:
	constexpr explicit AccelerometerContainer(const common::enums::Device device, const structs::accdata &data)
			: m_source(device)
			, m_data(data) {
	}

	common::enums::Device device() const final { return m_source; }
	enums::Event event() const override { return enums::Event::Accelerometer; }

	const auto &data() const noexcept { return m_data; }

private:
	const common::enums::Device m_source;
	structs::accdata m_data;
};
}
}
