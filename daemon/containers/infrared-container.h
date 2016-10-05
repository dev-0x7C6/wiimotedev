#pragma once

#include <cstdint>
#include <array>

#include "interfaces/icontainer.h"
#include "include/wiimotedev/wiimotedev"

struct xwii_event;

namespace service {
namespace container {
namespace structs {
struct irpoint {
	int16_t size;
	uint16_t x;
	uint16_t y;
};
}

using IrPoints = std::array<structs::irpoint, 4>; // max 4 ir

class InfraredContainer final : public interface::IContainer {
public:
	explicit InfraredContainer(const IrPoints &points);

	virtual enums::Device device() const override;
	virtual enums::Event event() const override;

	const IrPoints &points() const;

private:
	IrPoints m_points;
};
}
}
