#pragma once

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace container {
namespace structs {
struct pressdata {
	int32_t tl;
	int32_t tr;
	int32_t bl;
	int32_t br;
};
}

class PressureContainer final : public interface::IContainer {
public:
	explicit PressureContainer(const structs::pressdata &data);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	const structs::pressdata &data() const;

private:
	const structs::pressdata m_data;
};
}
}
