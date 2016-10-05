#include "stick-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::enums;

StickContainer::StickContainer(const Device device, int32_t lx, int32_t ly, int32_t rx, int32_t ry)
		: m_device(device)
		, m_lx(lx)
		, m_ly(ly)
		, m_rx(rx)
		, m_ry(ry) {
}

Device StickContainer::device() const { return m_device; }
Event StickContainer::event() const { return Event::Stick; }

int32_t StickContainer::lx() const { return m_lx; }
int32_t StickContainer::ly() const { return m_ly; }
int32_t StickContainer::rx() const { return m_rx; }
int32_t StickContainer::ry() const { return m_ry; }
