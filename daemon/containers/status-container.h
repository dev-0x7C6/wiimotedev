#pragma once

#include "interfaces/icontainer.h"
#include "enums/device-type.h"

namespace service {
namespace container {

class StatusContainer final : public interface::IContainer {
public:
	enum class State {
		Connected,
		Disconnected
	};

	explicit StatusContainer(const enums::Device deviceType, const State state);

	virtual Type type() const override;
	virtual enums::Device deviceType() const override;
	State state() const;

private:
	const enums::Device m_source;
	const State m_state;
};
}
}
