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
	explicit AccelerometerContainer(const common::enums::Device device, const structs::accdata &data);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	const structs::accdata &data() const;

private:
	const common::enums::Device m_source;
	structs::accdata m_data;
};
}
}
