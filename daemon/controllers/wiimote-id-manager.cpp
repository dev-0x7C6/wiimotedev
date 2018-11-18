#include "wiimote-id-manager.h"

using namespace dae::controller;

u32 WiimoteIdManager::take(const common::enums::Device device) {
	return m_uniqueId.at(static_cast<std::underlying_type<decltype(device)>::type>(device)).take();
}

u32 WiimoteIdManager::debt(const common::enums::Device device, const u32 index) {
	return m_uniqueId.at(static_cast<std::underlying_type<decltype(device)>::type>(device)).debt(index);
}
