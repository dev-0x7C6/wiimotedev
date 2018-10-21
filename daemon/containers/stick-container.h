#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace dae {
namespace container {

class StickContainer final : public interface::IContainer {
public:
	explicit StickContainer(const common::enums::Device device,
		int32_t lx, int32_t ly, int32_t rx, int32_t ry);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	int32_t lx() const;
	int32_t ly() const;
	int32_t rx() const;
	int32_t ry() const;

private:
	const common::enums::Device m_device;
	const int32_t m_lx;
	const int32_t m_ly;
	const int32_t m_rx;
	const int32_t m_ry;
};
}
}
