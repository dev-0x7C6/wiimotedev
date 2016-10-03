#pragma once

#include <atomic>
#include "interfaces/iid-manager.h"

namespace service {
namespace controller {

class WiimoteIdManager final : public interface::IIdManager {
public:
	explicit WiimoteIdManager() = default;
	virtual ~WiimoteIdManager() = default;

	virtual uint32_t take(const enums::Device device) override;
	virtual uint32_t debt(const enums::Device device, const uint32_t index) override;

private:
	std::array<functional::UniqueId<std::atomic<bool>, 128>, static_cast<std::size_t>(enums::Device::Last)> m_uniqueId;
};
}
}
