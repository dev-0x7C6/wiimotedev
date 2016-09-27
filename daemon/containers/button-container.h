#pragma once

#include <cstdint>
#include "interfaces/icontainer.h"

namespace service {
namespace container {

class ButtonContainer final : public interface::IContainer {
public:
	explicit ButtonContainer(const Source source, const uint64_t state);
	virtual ~ButtonContainer() = default;

	virtual Source source() const override;
	virtual Type type() const override;

	uint64_t state() const;
	void setState(const uint64_t &state);

private:
	uint64_t m_state;
	Source m_source;
};
}
}
