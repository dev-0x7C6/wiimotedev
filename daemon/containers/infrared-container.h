#pragma once

#include <cstdint>
#include <array>

#include "interfaces/icontainer.h"

struct xwii_event;

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
	explicit InfraredContainer(const IrPoints &points);
	explicit InfraredContainer(const xwii_event &event);

	virtual Type type() const override;
	virtual Source source() const override;

	const IrPoints &points() const;

private:
	IrPoints m_points;
};
}
}
