#pragma once

#include <atomic>
#include "interfaces/iid-manager.h"

namespace dae::controller {

class WiimoteIdManager final : public interface::IIdManager {
public:
	explicit WiimoteIdManager() = default;
	~WiimoteIdManager() override = default;

	virtual uint32_t take(const common::enums::Device device) override;
	virtual uint32_t debt(const common::enums::Device device, const uint32_t index) override;

private:
	std::array<functional::UniqueId<std::atomic<bool>, 128>, static_cast<std::size_t>(common::enums::Device::Last)> m_uniqueId;
};

}
