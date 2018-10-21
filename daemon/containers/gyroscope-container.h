#pragma once

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace container {

class GyroscopeContainer final : public interface::IContainer {
public:
	explicit GyroscopeContainer(int32_t x, int32_t y, int32_t z);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	inline int32_t x() const { return m_x; }
	inline int32_t y() const { return m_y; }
	inline int32_t z() const { return m_z; }

private:
	const int32_t m_x = 0;
	const int32_t m_y = 0;
	const int32_t m_z = 0;
};
}
}
