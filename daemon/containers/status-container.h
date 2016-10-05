#pragma once

#include "interfaces/icontainer.h"
#include "common/enums/device.h"

namespace dae {
namespace container {

class StatusContainer final : public interface::IContainer {
public:
	enum class State {
		Connected,
		Disconnected
	};

	explicit StatusContainer(const common::enums::Device device, const State state);

	virtual enums::Event event() const override;
	virtual common::enums::Device device() const override;
	State state() const;

private:
	const common::enums::Device m_source;
	const State m_state;
};
}
}
