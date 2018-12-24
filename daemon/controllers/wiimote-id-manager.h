#pragma once

#include <atomic>
#include <map>

#include "interfaces/iid-manager.h"
#include <externals/common/raii/raii-value-assigment.hpp>
#include <externals/common/types.hpp>

using raii_atomic_assigment = raii_value_assigment<std::atomic_bool &, true, false>;

class unique_id_locker {
public:
	unique_id_locker(raii_atomic_assigment &&lock, u32 id)
			: m_lock(std::move(lock))
			, m_id(id) {}

	constexpr auto id() const noexcept { return m_id; }

private:
	raii_atomic_assigment m_lock;
	const u32 m_id{};
};

class UniqueIdReservation {
public:
	unique_id_locker take() {
		for (std::size_t i = 0; i < m_reserved.size(); ++i) {
			auto &place = m_reserved[i];
			if (place)
				return unique_id_locker(raii_atomic_assigment(place), i);
		}
	}

private:
	std::array<std::atomic_bool, 128> m_reserved{};
};

namespace dae::controller {

class WiimoteIdManager final : public interface::IIdManager {
public:
	explicit WiimoteIdManager() = default;
	~WiimoteIdManager() final = default;

	u32 reserve(common::enums::Device device) final;
	u32 release(common::enums::Device device, u32 index) final;

private:
	std::array<functional::UniqueId<std::atomic<bool>, 128>, static_cast<std::size_t>(common::enums::Device::Last)> m_uniqueId;
};
}
