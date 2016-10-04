#pragma once

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/wiimotedev"

namespace service {
namespace container {
namespace structs {
struct gyrodata {
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t lowX;
	int32_t lowY;
	int32_t lowZ;
};
}

class GyroscopeContainer final : public interface::IContainer {
public:
	explicit GyroscopeContainer(const struct structs::gyrodata &data);
	virtual ~GyroscopeContainer() = default;

	virtual enums::Device device() const override;
	virtual enums::Event event() const override;

	structs::gyrodata data() const;

private:
	const structs::gyrodata m_data;
};
}
}
