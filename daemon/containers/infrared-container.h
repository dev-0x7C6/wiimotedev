#pragma once

#include <cstdint>
#include <array>

#include "interfaces/icontainer.h"

namespace daemon {
namespace container {
namespace structs {
struct ir {
	int32_t x : 16;
	int32_t y : 16;
	int32_t size : 16;
	int32_t valid : 16;
};
}

using IrPoints = std::array<structs::ir, 4>; // max 4 ir

class InfraredContainer final : public interface::IContainer {
public:
	using IContainer::IContainer;

	virtual Type type() const override { return Type::Infrared; }

	const IrPoints &points() const;
	void setPoints(const IrPoints &points);

private:
	IrPoints m_points;
};
}
}
