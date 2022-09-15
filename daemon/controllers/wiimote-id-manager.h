#pragma once

#include <atomic>
#include <map>

#include "interfaces/iid-manager.h"
#include <externals/common/raii/raii-value-assigment.hpp>
#include <externals/common/types.hpp>

namespace dae::controller {

class WiimoteIdManager final : public interface::IIdManager {
public:
	explicit WiimoteIdManager() = default;
	~WiimoteIdManager() final = default;

	u32 reserve(common::enums::device device) final;
	u32 release(common::enums::device device, u32 index) final;

private:
	std::array<functional::UniqueId<std::atomic<bool>, 128>, static_cast<std::size_t>(common::enums::devices.size())> m_uniqueId;
};
}
