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
	explicit PressureContainer(const structs::pressdata &data);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	const structs::pressdata &data() const;

private:
	const structs::pressdata m_data;
};
}
}
