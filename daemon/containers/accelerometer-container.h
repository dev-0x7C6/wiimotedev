#pragma once

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/wiimotedev"

namespace service {
namespace container {
namespace structs {
struct accdata {
	uint8_t x;
	uint8_t y;
	uint8_t z;
	double pitch;
	double roll;
};
};

class AccelerometerContainer final : public interface::IContainer {
public:
	explicit AccelerometerContainer(const enums::Device device, const structs::accdata &data);
	virtual ~AccelerometerContainer() = default;

	virtual enums::Device device() const override;
	virtual enums::Event event() const override;

	const structs::accdata &data() const;

private:
	const enums::Device m_source;
	structs::accdata m_data;
};
}
}
