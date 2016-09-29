#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace service {
namespace container {

class ButtonContainer final : public interface::IContainer {
public:
	explicit ButtonContainer(const enums::Device device, const uint64_t state);
	virtual ~ButtonContainer() = default;

	virtual enums::Device device() const override;
	virtual enums::Event event() const override;

	uint64_t state() const;
	void setState(const uint64_t &state);

private:
	uint64_t m_state;
	enums::Device m_source;
};
}
}
