#pragma once

#include <cstdint>
#include <array>

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/consts.h"

struct xwii_event;

namespace daemon {
namespace container {

using IrPoints = std::array<irpoint, 4>; // max 4 ir

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
