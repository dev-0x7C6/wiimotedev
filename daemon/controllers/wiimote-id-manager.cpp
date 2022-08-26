#include "wiimote-id-manager.h"

using namespace dae::controller;

u32 WiimoteIdManager::reserve(const common::enums::device device) {
	return m_uniqueId.at(static_cast<std::underlying_type<decltype(device)>::type>(device)).reserve();
}

u32 WiimoteIdManager::release(const common::enums::device device, const u32 index) {
	return m_uniqueId.at(static_cast<std::underlying_type<decltype(device)>::type>(device)).release(index);
}
