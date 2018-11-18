#include "stick-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::enums;

StickContainer::StickContainer(const Device device, i32 lx, i32 ly, i32 rx, i32 ry)
		: m_device(device)
		, m_lx(lx)
		, m_ly(ly)
		, m_rx(rx)
		, m_ry(ry) {
}

Device StickContainer::device() const { return m_device; }
Event StickContainer::event() const { return Event::Stick; }

i32 StickContainer::lx() const { return m_lx; }
i32 StickContainer::ly() const { return m_ly; }
i32 StickContainer::rx() const { return m_rx; }
i32 StickContainer::ry() const { return m_ry; }
