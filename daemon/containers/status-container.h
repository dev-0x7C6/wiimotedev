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

	constexpr explicit StatusContainer(const common::enums::Device device, const State state)
			: m_source(device)
			, m_state(state) {
	}

	enums::Event event() const final { return enums::Event::Status; }
	common::enums::Device device() const final { return m_source; }
	auto state() const noexcept { return m_state; }

private:
	const common::enums::Device m_source;
	const State m_state;
};
}
}
