#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace dae {
namespace container {

class StickContainer final : public interface::IContainer {
public:
	explicit StickContainer(const common::enums::Device device,
		i32 lx, i32 ly, i32 rx, i32 ry);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	i32 lx() const;
	i32 ly() const;
	i32 rx() const;
	i32 ry() const;

private:
	const common::enums::Device m_device;
	const i32 m_lx;
	const i32 m_ly;
	const i32 m_rx;
	const i32 m_ry;
};
}
}
