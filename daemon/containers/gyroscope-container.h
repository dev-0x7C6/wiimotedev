#pragma once

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

namespace dae {
namespace container {

class GyroscopeContainer final : public interface::IContainer {
public:
	explicit GyroscopeContainer(i32 x, i32 y, i32 z);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	inline i32 x() const { return m_x; }
	inline i32 y() const { return m_y; }
	inline i32 z() const { return m_z; }

private:
	const i32 m_x = 0;
	const i32 m_y = 0;
	const i32 m_z = 0;
};
}
}
