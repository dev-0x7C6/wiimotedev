#include "wiimote-id-manager.h"

using namespace service::controller;

uint32_t WiimoteIdManager::take(const enums::Device device) {
	return m_uniqueId.at(static_cast<std::underlying_type<decltype(device)>::type>(device)).take();
}

uint32_t WiimoteIdManager::debt(const enums::Device device, const uint32_t index) {
	return m_uniqueId.at(static_cast<std::underlying_type<decltype(device)>::type>(device)).debt(index);
}
