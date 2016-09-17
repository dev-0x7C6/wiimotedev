#pragma once

#include "interfaces/icontainer.h"

namespace daemon {
namespace container {

class ButtonContainer final : public interface::IContainer {
public:
	explicit ButtonContainer() = default;
	virtual ~ButtonContainer() = default;

	virtual Source source() const override;
	virtual Type type() const override;
};
}
}
