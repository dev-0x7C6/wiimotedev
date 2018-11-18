#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace dae {
namespace container {

class ButtonContainer final : public interface::IContainer {
public:
	explicit ButtonContainer(const common::enums::Device device, const u64 state);

	virtual common::enums::Device device() const override;
	virtual enums::Event event() const override;

	u64 state() const;
	void setState(const u64 &state);

private:
	u64 m_state;
	common::enums::Device m_source;
};
}
}
